/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:34 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/01 10:15:28 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	make_pipe(int pfd[2])
{
	if (pipe(pfd) == -1)
		return (perror("pipe"), 1);
	return (0);
}

static void	close_both(int pfd[2])
{
	close(pfd[0]);
	close(pfd[1]);
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

static pid_t	fork_left(t_shell_data *sh, t_node *n, int pfd[2])
{
	pid_t	pid;
	int		fds_left[2];

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pfd[0]);
		fds_left[0] = -1;
		fds_left[1] = pfd[1];
		_exit(run_exec_node(sh, n->left, fds_left, 0));
	}
	return (pid);
}

static pid_t	fork_right(t_shell_data *sh, t_node *n, int pfd[2])
{
	pid_t	pid;
	int		fds_right[2];

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pfd[1]);
		fds_right[0] = pfd[0];
		fds_right[1] = -1;
		_exit(run_exec_node(sh, n->right, fds_right, 0));
	}
	return (pid);
}

int	run_pipe(t_shell_data *sh, t_node *n, int is_top)
{
	int		pfd[2];
	pid_t	lpid;
	pid_t	rpid;
	int		right_status;

	(void)is_top;
	if (make_pipe(pfd))
		return (1);
	lpid = fork_left(sh, n, pfd);
	if (lpid == -1)
		return (close_both(pfd), 1);
	rpid = fork_right(sh, n, pfd);
	if (rpid == -1)
	{
		close_both(pfd);
		(void)wait_status(lpid);
		return (1);
	}
	close_both(pfd);
	(void)wait_status(lpid);
	right_status = wait_status(rpid);
	return (right_status);
}
