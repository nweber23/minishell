/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 11:19:18 by nweber            #+#    #+#             */
/*   Updated: 2025/09/02 10:30:22 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*build_tree(t_shell_data *shell, t_list *tokens)
{
	t_list	*temp;
	void	*root;

	temp = tokens;
	root = NULL;
	while (temp)
	{
		root = insert_nodes(shell, root, temp);
		if (token_check(temp) && !is_parenthesis(temp))
			temp = new_token(temp);
		else
		{
			if (is_parenthesis(temp))
				temp = remove_parenthesis(temp);
			else
			{
				temp = temp->next;
				while (token_check(temp) && !is_parenthesis(temp))
					temp = temp->next;
			}
		}
	}
	return (root);
}

void	*insert_nodes(t_shell_data *shell, void *left_node, t_list *tokens)
{
	t_token	*token;

	token = (t_token *)tokens->content;
	if (token->type == PARENTHESIS)
		return (create_parenthesis(shell, left_node, tokens));
	if (!left_node)
		build_subtree(shell, tokens);
	if (token->type == AND && !is_parenthesis(tokens->next))
		left_node = create_and(shell, left_node, \
			build_subtree(shell, tokens->next));
	else if (token->type == AND && is_parenthesis(tokens->next))
		left_node = get_and_node(shell, left_node, tokens);
	else if (token->type == OR && !is_parenthesis(tokens->next))
		left_node = create_or(shell, left_node, \
			build_subtree(shell, tokens->next));
	else if (token->type == OR && is_parenthesis(tokens->next))
		left_node = get_or_node(shell, left_node, tokens);
	else if (token->type == PIPE && !is_parenthesis(tokens->next))
		left_node = create_pipe(shell, left_node, \
			build_subtree(shell, tokens->next));
	else if (token->type == PIPE && is_parenthesis(tokens->next))
		left_node = get_pipe_node(shell, left_node, tokens);
	return (left_node);
}

void	*create_and(t_shell_data *shell, void *left, void *right)
{
	t_and_point	*node;

	node = malloc(sizeof(t_and_point));
	if (!node)
		error_malloc("create_and", shell);
	node->type = N_AND;
	node->left = left;
	node->right = right;
	return (node);
}

void	*create_or(t_shell_data *shell, void *left, void *right)
{
	t_or_point	*node;

	node = malloc(sizeof(t_or_point));
	if (!node)
		error_malloc("create_or", shell);
	node->type = N_OR;
	node->left = left;
	node->right = right;
	return (node);
}

void	*create_pipe(t_shell_data *shell, void *left, void *right)
{
	t_pipe	*node;

	node = malloc(sizeof(t_pipe));
	if (!node)
		error_malloc("create_pipe", shell);
	node->type = N_PIPE;
	node->left = left;
	node->right = right;
	return (node);
}
