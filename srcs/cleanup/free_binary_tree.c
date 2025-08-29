/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_binary_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:40:46 by nweber            #+#    #+#             */
/*   Updated: 2025/08/29 16:49:54 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_binary(void *root)
{
	t_node_type *node;

	if (!root)
		return ;
	node = (t_node_type *)root;
	if (node == N_PIPE)
		free_pipe((t_pipe *)root);
	else if (node == N_PIPE)
		free_exec((t_exec *)root);
	else if (node == N_AND)
		free_logic_tree((t_and_point *)root);
	else if (node == N_OR)
		free_logic_tree((t_or_point *)root);
	return ;
}