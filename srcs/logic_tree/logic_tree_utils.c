/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 13:23:04 by nweber            #+#    #+#             */
/*   Updated: 2025/08/29 15:11:20 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	token_check(t_list *node)
{
	return (node && ((t_token *)node->content)->type != AND \
		&& ((t_token *)node->content)->type != OR);
}

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
