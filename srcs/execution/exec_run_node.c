/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:33 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 10:01:47 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	run_node(t_shell_data *sh, t_node *node, int is_top)
{
	int	status;

	if (!node)
		return (1);
	if (node->type == ND_EXEC)
		return (run_exec_node(sh, node, NULL, is_top));
	if (node->type == ND_PIPE)
		return (run_pipe(sh, node, is_top));
	if (node->type == ND_AND)
	{
		status = run_node(sh, node->left, 0);
		if (status == 0)
			return (run_node(sh, node->right, 0));
		return (status);
	}
	if (node->type == ND_OR)
	{
		status = run_node(sh, node->left, 0);
		if (status != 0)
			return (run_node(sh, node->right, 0));
		return (status);
	}
	if (node->type == ND_GROUP)
		return (run_node(sh, node->left, 0));
	return (1);
}

int	exec_line(t_shell_data *sh, t_node *root)
{
	int	status;

	status = run_node(sh, root, 1);
	exit_code(status);
	return (status);
}
