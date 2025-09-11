/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:34 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 17:13:22 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	make_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), 1);
	return (0);
}

static void	close_both(int pipe_fds[2])
{
	close(pipe_fds[0]);
	close(pipe_fds[1]);
}

static int	wait_status(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static pid_t	fork_left(t_shell_data *sh, t_node *node, int pipe_fds[2])
{
	pid_t	pid;
	int		fds_left[2];

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pipe_fds[0]);
		fds_left[0] = -1;
		fds_left[1] = pipe_fds[1];
		_exit(run_exec_node(sh, node->left, fds_left, 0));
	}
	return (pid);
}

static pid_t	fork_right(t_shell_data *sh, t_node *node, int pipe_fds[2])
{
	pid_t	pid;
	int		fds_right[2];

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pipe_fds[1]);
		fds_right[0] = pipe_fds[0];
		fds_right[1] = -1;
		_exit(run_exec_node(sh, node->right, fds_right, 0));
	}
	return (pid);
}

int	run_pipe(t_shell_data *sh, t_node *node, int is_top)
{
	int		pipe_fds[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		right_status;

	(void)is_top;
	if (make_pipe(pipe_fds))
		return (1);
	left_pid = fork_left(sh, node, pipe_fds);
	if (left_pid == -1)
		return (close_both(pipe_fds), 1);
	right_pid = fork_right(sh, node, pipe_fds);
	if (right_pid == -1)
	{
		close_both(pipe_fds);
		(void)wait_status(left_pid);
		return (1);
	}
	close_both(pipe_fds);
	(void)wait_status(left_pid);
	right_status = wait_status(right_pid);
	return (right_status);
}
