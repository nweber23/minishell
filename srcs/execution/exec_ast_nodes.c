/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast_nodes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:01 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:13:50 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_node	*nd_new(t_ndtype t)
{
	t_node	*n;

	n = (t_node *)malloc(sizeof(t_node));
	if (!n)
		return (NULL);
	n->type = t;
	n->left = NULL;
	n->right = NULL;
	n->cmd = NULL;
	return (n);
}

t_redir	*rdr_new(t_rdrtype t, char *word)
{
	t_redir	*r;

	r = (t_redir *)malloc(sizeof(t_redir));
	if (!r)
		return (NULL);
	r->type = t;
	r->word = word;
	r->next = NULL;
	return (r);
}

int	is_lparen(t_token *t)
{
	if (!t)
		return (0);
	if (t->type == PARENTHESIS && t->value && t->value[0] == '(')
		return (1);
	return (0);
}

int	is_rparen(t_token *t)
{
	if (!t)
		return (0);
	if (t->type == PARENTHESIS && t->value && t->value[0] == ')')
		return (1);
	return (0);
}
