/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_andor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:17 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 09:56:47 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static t_node_type	logic_kind(t_token *token)
{
	if (token->type == AND)
		return (ND_AND);
	return (ND_OR);
}

static int	invalid_rhs(t_tokarr *ta)
{
	t_token	*next_tok;

	next_tok = peek(ta);
	if (!next_tok)
		return (1);
	if (is_rparen(next_tok))
		return (1);
	if (!starts_command(next_tok))
		return (1);
	return (0);
}

static t_node	*join_logic(t_node *left, t_node *right, t_token *op)
{
	t_node	*logic_node;

	logic_node = nd_new(logic_kind(op));
	if (!logic_node)
		return (NULL);
	logic_node->left = left;
	logic_node->right = right;
	return (logic_node);
}

t_node	*parse_and_or(t_shell_data *sh, t_tokarr *ta)
{
	t_node	*left_node;
	t_node	*right_node;
	t_token	*op;
	t_node	*logic_node;

	left_node = parse_pipeline(sh, ta);
	if (!left_node)
		return (NULL);
	while (1)
	{
		op = peek(ta);
		if (!op || (op->type != AND && op->type != OR))
			break ;
		(void)next(ta);
		if (invalid_rhs(ta))
			return (pipeline_syntax_err());
		right_node = parse_pipeline(sh, ta);
		if (!right_node)
			return (NULL);
		logic_node = join_logic(left_node, right_node, op);
		if (!logic_node)
			return (left_node);
		left_node = logic_node;
	}
	return (left_node);
}
