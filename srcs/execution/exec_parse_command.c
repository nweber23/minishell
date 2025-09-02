/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:19 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 09:36:01 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int is_redir_tok(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == INFILE || token->type == OUTFILE)
		return (1);
	if (token->type == APPEND || token->type == HERE_DOC)
		return (1);
	return (0);
}

static int argv_len(char **vector)
{
	int length;

	length = 0;
	if (!vector)
		return (0);
	while (vector[length])
		length++;
	return (length);
}

static int append_word_simple(char ***argv, char *word_copy)
{
	char	**new_vector;
	int	 count;
	int	 index;

	count = argv_len(*argv);
	new_vector = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_vector)
		return (0);
	index = 0;
	while (index < count)
	{
		new_vector[index] = (*argv)[index];
		index++;
	}
	new_vector[index] = word_copy;
	new_vector[index + 1] = NULL;
	free(*argv);
	*argv = new_vector;
	return (1);
}

static int handle_word(t_tokarr *ta, t_cmd *cmd)
{
	t_token *token;
	char	*dup_value;

	token = peek(ta);
	if (!token || token->type != WORD)
		return (0);
	dup_value = ft_strdup(token->value);
	if (!append_word_simple(&cmd->argv, dup_value))
		return (0);
	next(ta);
	return (1);
}

static int handle_redir(t_tokarr *ta, t_cmd *cmd)
{
	t_token	 *op_token;
	t_token	 *arg_token;
	t_rdrtype   rdr_kind;

	op_token = peek(ta);
	if (!op_token)
		return (0);
	next(ta);
	arg_token = peek(ta);
	if (!arg_token || arg_token->type != WORD)
		return (0);
	rdr_kind = map_rdr(op_token->type);
	if (!add_redir(&cmd->redirs, rdr_new(rdr_kind, ft_strdup(arg_token->value))))
		return (0);
	next(ta);
	return (1);
}

static int parse_command_loop(t_tokarr *ta, t_cmd *cmd)
{
	t_token *token;
	int	 success;

	success = 1;
	token = peek(ta);
	while (token && !is_cmd_end(token) && !is_lparen(token))
	{
		if (token->type == WORD)
			success = handle_word(ta, cmd);
		else if (is_redir_tok(token))
			success = handle_redir(ta, cmd);
		else
			break ;
		if (!success)
			return (0);
		token = peek(ta);
	}
	return (1);
}

t_node  *parse_command(t_shell_data *sh, t_tokarr *ta)
{
	t_node  *node;
	t_cmd   *cmd;
	int	 success;

	(void)sh;
	node = nd_new(ND_EXEC);
	if (!node)
		return (NULL);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (free(node), NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	success = parse_command_loop(ta, cmd);
	if (!success)
		return (free(cmd), free(node), NULL);
	node->cmd = cmd;
	return (node);
}
