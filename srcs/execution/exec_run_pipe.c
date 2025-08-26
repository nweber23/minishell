/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:34 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:35 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

int	run_pipe(t_shell_data *sh, t_node *n, int is_top)
{
	int		pfd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;
	int		status_left;
	int		status_right;

	if (pipe(pfd) == -1)
		return (perror("pipe"), 1);
	pid_left = fork();
	if (pid_left == -1)
		return (perror("fork"), 1);
	if (pid_left == 0)
	{
		close(pfd[0]);
		_exit(run_exec_node(sh, n->left, -1, pfd[1], 0));
	}
	pid_right = fork();
	if (pid_right == -1)
		return (perror("fork"), 1);
	if (pid_right == 0)
	{
		close(pfd[1]);
		_exit(run_exec_node(sh, n->right, pfd[0], -1, 0));
	}
	close(pfd[0]);
	close(pfd[1]);
	if (waitpid(pid_left, &status, 0) == -1)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		status_left = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status_left = 128 + WTERMSIG(status);
	else
		status_left = 1;
	if (waitpid(pid_right, &status, 0) == -1)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		status_right = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status_right = 128 + WTERMSIG(status);
	else
		status_right = 1;
	(void)is_top;
	(void)status_left;
	return (status_right);
}
