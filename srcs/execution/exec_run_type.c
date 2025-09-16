/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_runn_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 14:32:10 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/16 14:32:26 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	run_and_node(t_shell_data *sh, t_node *node)
{
	int	status;

	status = run_node(sh, node->left, 0);
	if (status == 0)
		return (run_node(sh, node->right, 0));
	return (status);
}

static int	run_or_node(t_shell_data *sh, t_node *node)
{
	int	status;

	status = run_node(sh, node->left, 0);
	if (status != 0)
		return (run_node(sh, node->right, 0));
	return (status);
}

static int	run_group_node(t_shell_data *sh, t_node *node)
{
	t_fdpack	p;
	int			status;

	if (node->cmd && node->cmd->redirs)
	{
		fdpack_init(&p);
		if (apply_all_redirs(node->cmd, &p.in, &p.out) != 0)
			return (1);
		fd_apply_inout(&p);
		status = run_node(sh, node->left, 0);
		fd_restore(&p);
		return (status);
	}
	return (run_node(sh, node->left, 0));
}

int	run_node(t_shell_data *sh, t_node *node, int is_top)
{
	if (node == NULL)
		return (1);
	if (node->type == ND_EXEC)
		return (run_exec_node(sh, node, NULL, is_top));
	if (node->type == ND_PIPE)
		return (run_pipe(sh, node, is_top));
	if (node->type == ND_AND)
		return (run_and_node(sh, node));
	if (node->type == ND_OR)
		return (run_or_node(sh, node));
	if (node->type == ND_GROUP)
		return (run_group_node(sh, node));
	return (1);
}
