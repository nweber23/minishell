/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_andor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:17 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 09:35:28 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static t_node_type logic_kind(t_token *token)
{
	if (token->type == AND)
		return (ND_AND);
	return (ND_OR);
}

t_node  *parse_and_or(t_shell_data *sh, t_tokarr *ta)
{
	t_node  *left_node;
	t_node  *logic_node;
	t_token *token;

	left_node = parse_pipeline(sh, ta);
	if (!left_node)
		return (NULL);
	while (1)
	{
		token = peek(ta);
		if (!token || (token->type != AND && token->type != OR))
			break ;
		logic_node = nd_new(logic_kind(token));
		if (!logic_node)
			return (left_node);
		next(ta);
		logic_node->left = left_node;
		logic_node->right = parse_pipeline(sh, ta);
		left_node = logic_node;
	}
	return (left_node);
}
