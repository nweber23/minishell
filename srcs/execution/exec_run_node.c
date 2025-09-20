/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:33 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/20 10:22:23 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	should_run_builtin_in_parent(t_node *node, int is_top)
{
	const char	*name;

	if (is_top == 0 || node == NULL || node->cmd == NULL
		|| node->cmd->argv == NULL)
		return (0);
	name = node->cmd->argv[0];
	if (name == NULL)
		return (0);
	return (is_builtin(name) != 0);
}

static pid_t	spawn_child(t_shell_data *sh, t_node *node,
				int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		child_exec(sh, node, in_fd, out_fd);
		child_cleanup_and_exit(sh, node, 127);
	}
	return (pid);
}

static int	handle_fork_failure(int pair[2])
{
	perror("fork");
	close_pair_if_set(pair);
	return (1);
}

int	run_exec_node(t_shell_data *sh, t_node *node, int pipe_fds[2], int is_top)
{
	pid_t	child_pid;
	int		in_fd;
	int		out_fd;

	if (node == NULL)
		return (1);
	if (node->type == ND_PIPE)
		return (run_pipe(sh, node, 0));
	if (node->cmd == NULL)
		return (1);
	select_inout_from_pair(pipe_fds, &in_fd, &out_fd);
	if (should_run_builtin_in_parent(node, is_top))
		return (exec_builtin_in_parent(sh, node->cmd));
	child_pid = spawn_child(sh, node, in_fd, out_fd);
	if (child_pid == -1)
		return (handle_fork_failure(pipe_fds));
	close_pair_if_set(pipe_fds);
	return (wait_and_status(child_pid));
}
