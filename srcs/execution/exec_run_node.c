/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:33 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/15 12:04:15 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	wait_and_status(pid_t child_pid)
{
	int	status;

	if (waitpid(child_pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

void	select_inout_from_pair(int pair[2], int *in_fd, int *out_fd)
{
	*in_fd = -1;
	*out_fd = -1;
	if (pair == NULL)
		return ;
	if (pair[0] != -1)
		*in_fd = pair[0];
	if (pair[1] != -1)
		*out_fd = pair[1];
}

int	run_node(t_shell_data *sh, t_node *node, int is_top)
{
	int	status;

	if (node == NULL)
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
	if (root == NULL)
		return (0);
	if (prepare_heredocs_tree(root, sh) != 0)
		return (exit_code(1));
	return (run_node(sh, root, 1));
}

int	run_exec_node(t_shell_data *sh, t_node *node, int pipe_fds[2], int is_top)
{
	pid_t		child_pid;
	const char	*cmd_name;
	int			in_fd;
	int			out_fd;

	if (node == NULL || node->cmd == NULL)
		return (1);
	cmd_name = NULL;
	if (node->cmd->argv != NULL)
		cmd_name = node->cmd->argv[0];
	if ((node->cmd->argv == NULL || node->cmd->argv[0] == NULL) && is_top == 0)
		return (select_inout_from_pair(pipe_fds, &in_fd, &out_fd), \
		child_exec(sh, node, in_fd, out_fd), 0);
	if (is_top != 0 && cmd_name != NULL && is_builtin(cmd_name) != 0)
		return (exec_builtin_in_parent(sh, node->cmd));
	select_inout_from_pair(pipe_fds, &in_fd, &out_fd);
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (child_pid == 0)
		child_exec(sh, node, in_fd, out_fd);
	return (close_pair_if_set(pipe_fds), wait_and_status(child_pid));
}
