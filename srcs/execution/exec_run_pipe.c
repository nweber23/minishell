/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:34 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/29 16:36:26 by yyudi            ###   ########.fr       */
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

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pfd[0]);
		_exit(run_exec_node(sh, n->left, pfd[1], 0));
	}
	return (pid);
}

static pid_t	fork_right(t_shell_data *sh, t_node *n, int pfd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pfd[1]);
		_exit(run_exec_node(sh, n->right, pfd[0], 0));
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
