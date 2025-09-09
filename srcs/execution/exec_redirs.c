/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:28 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/08 20:49:24 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	open_infile(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		perror(path);
	return (fd);
}

static int	open_out_trunc(const char *path)
{
	int	fd;

	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		perror(path);
	return (fd);
}

static int	open_out_append(const char *path)
{
	int	fd;

	fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
		perror(path);
	return (fd);
}

static int	open_heredoc_fd(t_redir *r)
{
	int	fd;

	fd = heredoc_to_fd(r);
	return (fd);
}

static void	assign_new(int *slot, int fd_new)
{
	if (*slot != -1)
		close(*slot);
	*slot = fd_new;
}


int	apply_all_redirs(t_cmd *cmd, int *fdin, int *fdout)
{
	t_redir	*r;
	int		fd;

	*fdin = -1;
	*fdout = -1;
	r = cmd->redirs;
	while (r != NULL)
	{
		if (r->type == R_IN)
		{
			fd = open_infile(r->word);
			if (fd < 0)
				return (1);
			assign_new(fdin, fd);
		}
		else if (r->type == R_HEREDOC)
		{
			fd = open_heredoc_fd(r);
			if (fd < 0)
				return (1);
			assign_new(fdin, fd);
		}
		else if (r->type == R_OUT_TRUNC)
		{
			fd = open_out_trunc(r->word);
			if (fd < 0)
				return (1);
			assign_new(fdout, fd);
		}
		else if (r->type == R_OUT_APPEND)
		{
			fd = open_out_append(r->word);
			if (fd < 0)
				return (1);
			assign_new(fdout, fd);
		}
		r = r->next;
	}
	return (0);
}