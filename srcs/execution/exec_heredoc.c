/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:14 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/22 10:59:14 by yyudi            ###   ########.fr       */
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
		{
			if (exit_code(-1) == 130 || (sh && sh->exit_code == 130))
				return (-1);
			return (0);
		}
		if (exit_code(-1) == 130 || (sh && sh->exit_code == 130))
			return (free(line), -1);
		if (hd_is_delim(line, rd->word) == 1)
			return (free(line), 0);
		line = hd_expand_if_needed(sh, line, rd->quoted_delim);
		if (hd_write_line(wfd, line) != 0)
			return (free(line), -1);
		free(line);
	}
}

int	heredoc_to_fd(t_redir *r)
{
	int				pfd[2];
	pid_t			pid;
	t_shell_data	*sh;

	sh = global_shell(NULL, 1);
	if (hd_open_pipe(pfd) != 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(pfd[0]);
		close(pfd[1]);
		return (-1);
	}
	if (pid == 0)
		hd_child_proc(sh, r, pfd);
	return (hd_parent_proc(pid, pfd));
}
