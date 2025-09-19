/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:19 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 20:42:31 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
	t_redir		*rd;

	op_token = peek(ta);
	if (!op_token)
		return (0);
	next(ta);
	arg_token = peek(ta);
	if (!arg_token || arg_token->type != WORD)
		return (0);
	rdr_kind = map_rdr(op_token->type);
	rd = rdr_new(rdr_kind, ft_strdup(arg_token->value));
	if (!rd)
		return (0);
	if (rdr_kind == R_HEREDOC
		&& (arg_token->state == SINGLE_Q || arg_token->state == DOUBLE_Q))
		rd->quoted_delim = 1;
	if (!add_redir(&cmd->redirs, rd))
		return (free(rd->word), free(rd), 0);
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
