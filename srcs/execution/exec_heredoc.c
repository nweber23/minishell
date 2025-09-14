/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:14 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/12 17:49:00 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	hd_is_delim(const char *line, const char *delim)
{
	if (line == NULL || delim == NULL)
		return (0);
	if (ft_strcmp(line, delim) == 0)
		return (1);
	return (0);
}

static char	*hd_expand_if_needed(t_shell_data *sh, char *s, int quoted)
{
	char	*exp;

	if (quoted != 0)
		return (s);
	exp = expand_line_env(sh, s);
	if (exp == NULL)
		return (s);
	free(s);
	return (exp);
}

static int	hd_write_line(int wfd, const char *s)
{
	ssize_t	w;
	size_t	len;

	if (s == NULL)
		return (0);
	len = ft_strlen(s);
	w = write(wfd, s, len);
	if (w < 0)
		return (-1);
	w = write(wfd, "\n", 1);
	if (w < 0)
		return (-1);
	return (0);
}

int	hd_loop(t_shell_data *sh, t_redir *rd, int wfd)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (0);
		if (hd_is_delim(line, rd->word) == 1)
		{
			free(line);
			return (0);
		}
		line = hd_expand_if_needed(sh, line, rd->quoted_delim);
		if (hd_write_line(wfd, line) != 0)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
}

int	heredoc_to_fd(t_redir *r)
{
	int				pfd[2];
	int				status;
	t_shell_data	*sh;

	sh = global_shell(NULL, 1);
	if (hd_open_pipe(pfd) != 0)
		return (-1);
	trap_heredoc();
	if (isatty(STDIN_FILENO))
		status = hd_loop_tty(sh, r, pfd[1]);
	else
		status = hd_loop(sh, r, pfd[1]);
	close(pfd[1]);
	if (status != 0)
	{
		close(pfd[0]);
		return (-1);
	}
	return (pfd[0]);
}
