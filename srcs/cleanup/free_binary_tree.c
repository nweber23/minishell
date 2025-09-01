/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_binary_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:40:46 by nweber            #+#    #+#             */
/*   Updated: 2025/09/01 10:19:26 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_binary(void *root)
{
	t_node_type	node;

	if (!root)
		return ;
	node = *(t_node_type *)root;
	if (node == N_PIPE)
		free_pipe((t_pipe *)root);
	else if (node == N_EXEC)
		free_exec((t_exec *)root);
	else if (node == N_AND)
		free_logic_tree((t_and_point *)root);
	else if (node == N_OR)
		free_logic_tree((t_or_point *)root);
}

void	free_pipe(t_pipe *pipe)
{
	if (!pipe)
		return ;
	if (pipe->left)
		free_binary(pipe->left);
	if (pipe->right)
		free_binary(pipe->right);
	free(pipe);
}

void	free_exec(t_exec *exec)
{
	if (!exec)
		return ;
	if (exec->command)
		free(exec->command);
	if (exec->argv)
		ft_array_free(exec->argv);
	if (exec->infile)
		ft_lstclear(&exec->infile, free);
	if (exec->outfile)
		ft_lstclear(&exec->outfile, free);
	free(exec);
}
