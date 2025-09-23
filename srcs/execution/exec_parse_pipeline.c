/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:23 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/12 18:10:51 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	is_pipe_token(t_token *t)
{
	return (t && t->type == PIPE);
}

static int	advance_and_peek(t_tokarr *ta, t_token **t)
{
	(void)next(ta);
	*t = peek(ta);
	return (*t != NULL);
}

static t_node	*make_pipe_node(t_node *left, t_node *right)
{
	t_node	*n;

	n = nd_new(ND_PIPE);
	if (!n)
		return (left);
	n->left = left;
	n->right = right;
	return (n);
}

t_node	*parse_pipeline(t_shell_data *sh, t_tokarr *ta)
{
	t_node	*left;
	t_node	*right;
	t_token	*t;

	left = parse_factor(sh, ta);
	t = peek(ta);
	while (left && is_pipe_token(t))
	{
		if (!advance_and_peek(ta, &t))
			return (free_tree(left), pipeline_syntax_eof());
		if (!starts_command(t) || is_rparen(t))
			return (free_tree(left), pipeline_syntax_err());
		right = parse_factor(sh, ta);
		if (!right)
			return (free_tree(left), pipeline_syntax_err());
		left = make_pipe_node(left, right);
		t = peek(ta);
	}
	return (left);
}
