/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:19 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 18:06:14 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	is_redir_tok(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == INFILE || token->type == OUTFILE)
		return (1);
	if (token->type == APPEND || token->type == HERE_DOC)
		return (1);
	return (0);
}

static int	argv_len(char **vector)
{
	int	length;

	length = 0;
	if (!vector)
		return (0);
	while (vector[length])
		length++;
	return (length);
}

static void	mask_quoted_stars(char *s)
{
	int i = 0;
	if (!s) return;
	while (s[i])
	{
		if (s[i] == '*')
			s[i] = '\a';
		i++;
	}
}

static char	*expand_token_value(t_shell_data *sh, t_token *tok)
{
	char	*dup;
	char	*expanded;

	dup = ft_strdup(tok->value);
	if (!dup)
		return (NULL);
	if (tok->state == SINGLE_Q)
		return (dup);
	expanded = expand_line_env(sh, dup);
	if (!expanded)
		return (dup);
	free(dup);
	return (expanded);
}

static int	append_word_simple(char ***argv, char *word_copy)
{
	char	**new_vector;
	int		count;
	int		index;

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
	if (*argv)
		free(*argv);
	*argv = new_vector;
	return (1);
}

static int	handle_word(t_shell_data *sh, t_tokarr *ta, t_cmd *cmd)
{
    t_token	*token;
    char	*value;

    token = peek(ta);
    if (!token || token->type != WORD)
        return (0);
    value = expand_token_value(sh, token);
    if (!value)
        return (0);
    if (token->state == EXPAND && value[0] == '\0')
    {
        free(value);
        next(ta);
        return (1);
    }
    if (token->state == SINGLE_Q || token->state == DOUBLE_Q)
        mask_quoted_stars(value);
    if (!append_word_simple(&cmd->argv, value))
        return (free(value), 0);
    next(ta);
    return (1);
}

static int	handle_redir(t_tokarr *ta, t_cmd *cmd)
{
	t_token		*op_token;
	t_token		*arg_token;
	t_rdrtype	rdr_kind;

	op_token = peek(ta);
	if (!op_token)
		return (0);
	next(ta);
	arg_token = peek(ta);
	if (!arg_token || arg_token->type != WORD)
		return (0);
	rdr_kind = map_rdr(op_token->type);
	if (!add_redir(&cmd->redirs, rdr_new(rdr_kind,
				ft_strdup(arg_token->value))))
		return (0);
	next(ta);
	return (1);
}

static int	parse_command_loop(t_shell_data *sh, t_tokarr *ta, t_cmd *cmd)
{
    t_token	*token;
    int		success;

    success = 0;
    token = peek(ta);
    while (token && !is_cmd_end(token))
    {
        if (is_redir_tok(token))
            success = handle_redir(ta, cmd);
        else
            success = handle_word(sh, ta, cmd);
        if (!success)
            return (0);
        token = peek(ta);
    }
    return (1);
}

t_node	*parse_command(t_shell_data *sh, t_tokarr *ta)
{
    t_node	*node;

    node = nd_new(ND_EXEC);
    if (!node)
        return (NULL);
    node->cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
    if (!node->cmd)
        return (free(node), NULL);
    if (!parse_command_loop(sh, ta, node->cmd))
        return (free(node->cmd), free(node), NULL);
    return (node);
}
