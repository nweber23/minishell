/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:14 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 20:38:31 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	open_pipe_pair(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
		return (-1);
	return (0);
}

static int	write_line_fd(int wfd, const char *s)
{
	ssize_t	written;
	size_t	len;

	if (s == NULL)
		return (0);
	len = ft_strlen(s);
	written = write(wfd, s, len);
	if (written < 0)
		return (-1);
	written = write(wfd, "\n", 1);
	if (written < 0)
		return (-1);
	return (0);
}

static int	line_is_delim(const char *line, const char *delim)
{
	if (line == NULL || delim == NULL)
		return (0);
	if (ft_strcmp(line, delim) == 0)
		return (1);
	return (0);
}

static char	*expand_line_if_needed(t_shell_data *sh,
									char *line,
									int quoted_limiter)
{
	char	*expanded;

	if (quoted_limiter != 0)
		return (line);
	expanded = expand_line_env(sh, line);
	if (expanded == NULL)
		return (line);
	free(line);
	return (expanded);
}

static int	heredoc_loop(t_shell_data *sh, t_redir *redir, int wfd)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (0);
		if (line_is_delim(line, redir->word) == 1)
		{
			free(line);
			return (0);
		}
		line = expand_line_if_needed(sh, line, redir->quoted_delim);
		if (write_line_fd(wfd, line) != 0)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
}

int	heredoc_to_fd(t_redir *redir)
{
	int				pipe_fd[2];
	t_shell_data	*sh;

	if (redir == NULL || redir->word == NULL)
		return (-1);
	if (open_pipe_pair(pipe_fd) != 0)
		return (-1);
	trap_heredoc();
	sh = global_shell(NULL, 0);
	if (heredoc_loop(sh, redir, pipe_fd[1]) != 0)
	{
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		return (-1);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
