/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 21:15:58 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/12 10:23:38 by yyudi            ###   ########.fr       */
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
