/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:14 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/08 20:36:28 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	open_pipe_pair(int p[2])
{
	if (pipe(p) == -1)
		return (-1);
	return (0);
}

static int	write_line_fd(int wfd, const char *s)
{
	ssize_t	n;
	size_t	len;

	if (s == NULL)
		return (0);
	len = ft_strlen(s);
	n = write(wfd, s, len);
	if (n < 0)
		return (-1);
	n = write(wfd, "\n", 1);
	if (n < 0)
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

/* opsional: expand env pada line (hanya jika delim tidak di-quote) */
static char	*expand_line_if_needed(t_shell_data *sh, char *line, int quoted)
{
	char	*expanded;

	if (quoted != 0)
		return (line);
	expanded = expand_line_env(sh, line);
	if (expanded == NULL)
		return (line);
	free(line);
	return (expanded);
}

/* Kembalikan read-end fd untuk dipakai sebagai STDIN command */
int	heredoc_to_fd(t_redir *r)
{
	int				p[2];
	char			*line;
	t_shell_data	*sh;

	if (r == NULL || r->word == NULL)
		return (-1);
	if (open_pipe_pair(p) != 0)
		return (-1);
	trap_heredoc();
	sh = global_shell(NULL, 0);
	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (line_is_delim(line, r->word) == 1)
		{
			free(line);
			break ;
		}
		line = expand_line_if_needed(sh, line, r->quoted_delim);
		if (write_line_fd(p[1], line) != 0)
		{
			free(line);
			close(p[1]);
			close(p[0]);
			return (-1);
		}
		free(line);
	}
	close(p[1]);
	return (p[0]);
}