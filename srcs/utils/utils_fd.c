/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 20:52:23 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/12 10:23:50 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	open_infile(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		perror(path);
	return (fd);
}

int	open_out_trunc(const char *path)
{
	int	fd;

	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		perror(path);
	return (fd);
}

int	open_out_append(const char *path)
{
	int	fd;

	fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
		perror(path);
	return (fd);
}

int	open_heredoc_fd(t_redir *redir)
{
	int	fd;

	fd = heredoc_to_fd(redir);
	return (fd);
}
