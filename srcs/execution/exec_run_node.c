/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:33 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:34 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

int	run_node(t_shell_data *sh, t_node *n, int is_top)
{
	int	status;

	if (!n)
		return (1);
	if (n->type == ND_EXEC)
		return (run_exec_node(sh, n, -1, -1, is_top));
	if (n->type == ND_PIPE)
		return (run_pipe(sh, n, is_top));
	if (n->type == ND_AND)
	{
		status = run_node(sh, n->left, 0);
		if (status == 0)
			return (run_node(sh, n->right, 0));
		return (status);
	}
	if (n->type == ND_OR)
	{
		status = run_node(sh, n->left, 0);
		if (status != 0)
			return (run_node(sh, n->right, 0));
		return (status);
	}
	if (n->type == ND_GROUP)
		return (run_node(sh, n->left, 0));
	return (1);
}

int	exec_line(t_shell_data *sh, t_node *root)
{
	int	status;

	status = run_node(sh, root, 1);
	exit_code(status);
	return (status);
}
