/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_binary_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:40:46 by nweber            #+#    #+#             */
/*   Updated: 2025/09/08 16:58:08 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_binary(void *root)
{
	t_node_type	node;

	if (!root)
		return ;
	node = *(t_node_type *)root;
	if (node == ND_PIPE)
		free_pipe((t_pipe *)root);
	else if (node == ND_EXEC)
		free_exec((t_exec *)root);
	else if (node == ND_AND)
		free_logic_tree((t_and_point *)root);
	else if (node == ND_OR)
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
