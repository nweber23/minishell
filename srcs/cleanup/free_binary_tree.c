/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_binary_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:40:46 by nweber            #+#    #+#             */
/*   Updated: 2025/09/12 13:11:26 by nweber           ###   ########.fr       */
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

static void	free_infile_node(void *content)
{
	t_infile	*in;

	in = (t_infile *)content;
	if (!in)
		return ;
	if (in->eof)
		free(in->eof);
	if (in->name)
		free(in->name);
	free(in);
}

static void	free_outfile_node(void *content)
{
	t_outfile	*out;

	out = (t_outfile *)content;
	if (!out)
		return ;
	if (out->name)
		free(out->name);
	free(out);
}

void	free_exec(t_exec *exec)
{
	if (!exec)
		return ;
	if (exec->argv)
	{
		int i = 0;
		while (exec->argv[i])
		{
			free(exec->argv[i]);
			i++;
		}
		free(exec->argv);
	}
	if (exec->infile)
		ft_lstclear(&exec->infile, free_infile_node);
	if (exec->outfile)
		ft_lstclear(&exec->outfile, free_outfile_node);
	free(exec->command);
	free(exec);
}
