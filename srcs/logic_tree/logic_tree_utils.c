/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 13:23:04 by nweber            #+#    #+#             */
/*   Updated: 2025/09/02 10:17:15 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	token_check(t_list *node)
{
	return (node && ((t_token *)node->content)->type != AND \
		&& ((t_token *)node->content)->type != OR);
}

void	*build_subtree(t_shell_data *shell, t_list *tokens)
{
	void	*subtree;

	if (!tokens)
		return (NULL);
	subtree = build_binary_tree(shell, tokens);
	if (!subtree)
		error_malloc("build_subtree", shell);
	return (subtree);
}

t_list	*new_token(t_list *temp)
{
	while (token_check(temp))
	{
		if (is_parenthesis(temp))
			temp = remove_parenthesis(temp);
		else
			temp = temp->next;
	}
	return (temp);
}
