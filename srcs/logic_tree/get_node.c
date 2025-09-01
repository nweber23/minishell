/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:40:21 by nweber            #+#    #+#             */
/*   Updated: 2025/09/01 14:21:35 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_sublist(t_list *new)
{

}

void	*get_and_node(t_shell_data *shell, void *left_node, t_list *tokens)
{
	t_list	*new;

	new = NULL;
	new = new_sublist(tokens->next);
	left_node = create_and(shell, left_node, build_tree(shell, new));
	clear_sublist(new);
	return (left_node);
}

void	*get_or_node(t_shell_data *shell, void *left_node, t_list *tokens)
{
	t_list	*new;

	new = NULL;
	new = new_sublist(tokens->next);
	left_node = create_or(shell, left_node, build_tree(shell, new));
	clear_sublist(new);
	return (left_node);
}
