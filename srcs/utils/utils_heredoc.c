/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 21:15:58 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/22 10:59:40 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	hd_open_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
		return (-1);
	return (0);
}

int	hd_loop_tty(t_shell_data *sh, t_redir *rd, int wfd)
{
	int	tty_fd;
	int	saved_stdin;
	int	rc;

	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd < 0)
		return (-1);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin < 0)
	{
		close(tty_fd);
		return (-1);
	}
	if (dup2(tty_fd, STDIN_FILENO) < 0)
	{
		close(tty_fd);
		close(saved_stdin);
		return (-1);
	}
	close(tty_fd);
	rc = hd_loop(sh, rd, wfd);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	return (rc);
}

void	hd_child_proc(t_shell_data *sh, t_redir *r, int *pfd)
{
	int	st;

	trap_heredoc();
	close(pfd[0]);
	if (isatty(STDIN_FILENO))
		st = hd_loop_tty(sh, r, pfd[1]);
	else
		st = hd_loop(sh, r, pfd[1]);
	close(pfd[1]);
	combine(sh);
	exit(st != 0);
}

int	hd_parent_proc(pid_t pid, int *pfd)
{
	int	st;

	close(pfd[1]);
	signal(SIGINT, SIG_IGN);
	if (waitpid(pid, &st, 0) < 0)
	{
		close(pfd[0]);
		init_signals();
		return (-1);
	}
	if (is_interactive())
		interavtive_signals();
	else
		init_signals();
	if ((WIFSIGNALED(st) && WTERMSIG(st) == SIGINT) || (WIFEXITED(st)
			&& WEXITSTATUS(st) != 0))
	{
		exit_code(130);
		close(pfd[0]);
		return (-1);
	}
	return (pfd[0]);
}
