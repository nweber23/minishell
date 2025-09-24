/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_group.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:22 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/23 12:22:55 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	parse_one_redir_after_group(t_tokarr *ta, t_cmd **pcmd)
{
	t_token		*op;
	t_token		*arg;
	t_rdrtype	kind;

	op = peek(ta);
	if (!is_redir_tok(op))
		return (1);
	(void)next(ta);
	arg = peek(ta);
	if (!arg || arg->type != WORD)
		return (0);
	if (!*pcmd)
	{
		*pcmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
		if (!*pcmd)
			return (0);
	}
	kind = map_rdr(op->type);
	if (!add_redir(&(*pcmd)->redirs, rdr_new(kind, ft_strdup(arg->value))))
		return (0);
	(void)next(ta);
	return (1);
}

t_node	*parse_group(t_shell_data *sh, t_tokarr *ta)
{
	t_node	*t;
	t_token	*s;

	s = peek(ta);
	if (!is_lparen(s))
		return (NULL);
	(void)next(ta);
	t = nd_new(ND_GROUP);
	if (!t)
		return (NULL);
	t->left = parse_and_or(sh, ta);
	if (!t->left)
		return (free(t), NULL);
	s = peek(ta);
	if (!is_rparen(s))
		return (free_tree(t->left), free(t), NULL);
	(void)next(ta);
	s = peek(ta);
	while (s && is_redir_tok(s))
	{
		if (!parse_one_redir_after_group(ta, &t->cmd))
			return (free_tree(t->left), free(t), NULL);
		s = peek(ta);
	}
	return ((void)sh, t);
}
