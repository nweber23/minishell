/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:14 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:15 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

static int	pushline(int wfd, char *line)
{
	ft_putendl_fd(line, wfd);
	return (0);
}

int	apply_redirs_heredoc(t_cmd *c, int *fdin)
{
	int		pfd[2];
	char	*line;
	size_t	len;
	t_redir	*r;

	*fdin = -1;
	r = c->redirs;
	while (r)
	{
		if (r->type != R_HEREDOC)
		{
			r = r->next;
			continue ;
		}
		if (pipe(pfd) == -1)
			return (1);
		while (1)
		{
			line = get_next_line(STDIN_FILENO);
			if (!line)
				break ;
			len = ft_strlen(line);
			if (len > 0 && line[len - 1] == '\n')
				line[len - 1] = '\0';
			if (ft_strcmp(line, r->word) == 0)
			{
				free(line);
				break ;
			}
			pushline(pfd[1], line);
			free(line);
		}
		close(pfd[1]);
		if (*fdin != -1)
			close(*fdin);
		*fdin = pfd[0];
		r = r->next;
	}
	return (0);
}
