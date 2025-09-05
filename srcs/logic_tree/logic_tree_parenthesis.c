/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_tree_parenthesis.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 18:38:48 by nweber            #+#    #+#             */
/*   Updated: 2025/09/02 10:36:55 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_parenthesis(t_list *node)
{
	return (node && ((t_token *)node->content)->type != PARENTHESIS);
}

t_list	*remove_parenthesis(t_list *temp)
{
	int		balance;
	t_token	*token;

	temp = temp->next;
	balance = 1;
	if (temp)
		token = (t_token *)temp->content;
	while (temp && balance != 0)
	{
		if (temp)
		{
			token = (t_token *)temp->content;
			if (token->type == PARENTHESIS && *token->value == ')')
				balance--;
			if (token->type == PARENTHESIS && *token->value == '(')
				balance++;
		}
		if (balance != 0)
			temp = temp->next;
	}
	if (temp)
		temp = temp->next;
	return (temp);
}

void	*create_parenthesis(t_shell_data *shell,
	void *left_node, t_list *tokens)
{
	t_list	*new;
	t_token	*token;

	new = create_new_sublist(tokens);
	token = (t_token *)tokens->content;
	if (!left_node)
		left_node = build_tree(shell, new);
	if (token->type == AND)
		left_node = create_and(shell, left_node, build_tree(shell, new));
	if (token->type == OR)
		left_node = create_or(shell, left_node, build_tree(shell, new));
	clear_sublist(new);
	return (left_node);
}
