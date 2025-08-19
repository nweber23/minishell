/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:56:39 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/19 09:56:46 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_READLINE
# include <readline/readline.h>
#endif

static int	collect_one(const char *delim)
{
	int		p[2];
	char	*line;

	if (pipe(p) < 0)
		return (-1);
	while (1)
	{
#ifdef HAVE_READLINE
		line = readline("> ");
#else
		(void)delim;
		line = NULL;
#endif
		if (line == NULL)
			break ;
		if (strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write(p[1], line, strlen(line));
		write(p[1], "\n", 1);
		free(line);
	}
	close(p[1]);
	return (p[0]);
}

int	ms_collect_all_heredoc(t_pipeline *pl)
{
	int	i;
	int	k;
	int	fd;

	i = 0;
	while (i < pl->count)
	{
		k = 0;
		while (k < pl->cmds[i].redir_count)
		{
			if (pl->cmds[i].redirs[k].type == R_HDOC)
			{
				fd = collect_one(pl->cmds[i].redirs[k].arg);
				if (fd < 0)
					return (-1);
				pl->cmds[i].redirs[k].hdoc_fd = fd;
			}
			k++;
		}
		i++;
	}
	return (0);
}
