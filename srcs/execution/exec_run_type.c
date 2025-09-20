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

static int	launch_group(t_shell_data *sh, t_node *node, int in_fd, int out_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		if (in_fd != -1)
			close(in_fd);
		if (out_fd != -1)
			close(out_fd);
		return (perror("fork"), 1);
	}
	if (pid == 0)
	{
		helper(sh, in_fd, out_fd);
		status = run_node(sh, node->left, 0);
		combine(sh);
		_exit(status);
	}
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
	return (wait_and_status(pid));
}

static int	run_group_node(t_shell_data *sh, t_node *node)
{
	int	in_fd;
	int	out_fd;

	in_fd = -1;
	out_fd = -1;
	if (node->cmd && node->cmd->redirs)
		if (apply_all_redirs(node->cmd, &in_fd, &out_fd) != 0)
			return (1);
	return (launch_group(sh, node, in_fd, out_fd));
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
