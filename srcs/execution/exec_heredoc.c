/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:14 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/28 17:26:53 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	hd_open_pipe(int pfd[2])
{
	if (pipe(pfd) == -1)
		return (perror("pipe"), 1);
	return (0);
}

static int	hd_read_until(int wfd, const char *delim)
{
	char	*line;
	size_t	len;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			return (0);
		}
		ft_putendl_fd(line, wfd);
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

int	apply_redirs_heredoc(t_cmd *c, int *fdin)
{
	int		pfd[2];
	t_redir	*r;

	*fdin = -1;
	r = c->redirs;
	while (r)
	{
		if (r->type == R_HEREDOC)
		{
			if (hd_open_pipe(pfd))
				return (1);
			if (hd_read_until(pfd[1], r->word))
			{
				close(pfd[0]);
				close(pfd[1]);
				return (1);
			}
			close(pfd[1]);
			hd_replace_in(fdin, pfd[0]);
		}
		r = r->next;
	}
	return (0);
}
