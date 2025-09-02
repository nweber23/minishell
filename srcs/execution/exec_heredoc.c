/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:14 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 09:56:19 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	hd_open_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), 1);
	return (0);
}

static int	hd_read_until(int write_fd, const char *delimiter)
{
	char	*line;
	size_t	len;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			return (0);
		}
		ft_putendl_fd(line, write_fd);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	return (0);
}

static void	hd_replace_in(int *fdin, int newfd)
{
	if (*fdin != -1)
		close(*fdin);
	*fdin = newfd;
}

int	apply_redirs_heredoc(t_cmd *cmd, int *fdin)
{
	int		pipe_fds[2];
	t_redir	*redir;

	*fdin = -1;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == R_HEREDOC)
		{
			if (hd_open_pipe(pipe_fds))
				return (1);
			if (hd_read_until(pipe_fds[1], redir->word))
			{
				close(pipe_fds[0]);
				close(pipe_fds[1]);
				return (1);
			}
			close(pipe_fds[1]);
			hd_replace_in(fdin, pipe_fds[0]);
		}
		redir = redir->next;
	}
	return (0);
}
