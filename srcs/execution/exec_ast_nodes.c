/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast_nodes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:01 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/14 16:17:33 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_node	*nd_new(t_node_type kind)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = kind;
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	return (node);
}

t_redir	*rdr_new(t_rdrtype kind, char *word)
{
	t_redir	*redir_node;

	redir_node = (t_redir *)malloc(sizeof(t_redir));
	if (!redir_node)
		return (NULL);
	redir_node->type = kind;
	redir_node->word = word;
	redir_node->quoted_delim = 0;
	redir_node->next = NULL;
	return (redir_node);
}

int	is_lparen(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == PARENTHESIS && token->value && token->value[0] == '(')
		return (1);
	return (0);
}

int	is_rparen(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == PARENTHESIS && token->value && token->value[0] == ')')
		return (1);
	return (0);
}
