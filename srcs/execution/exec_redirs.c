/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:28 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/29 11:29:53 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	open_mode(t_rdrtype t)
{
	if (t == R_IN)
		return (O_RDONLY);
	if (t == R_OUT_TRUNC)
		return (O_CREAT | O_WRONLY | O_TRUNC);
	if (t == R_OUT_APPEND)
		return (O_CREAT | O_WRONLY | O_APPEND);
	return (O_RDONLY);
}

static int	assign_fd(int *slot, int fd)
{
	if (*slot != -1)
		close(*slot);
	*slot = fd;
	return (0);
}

static int	open_redir_fd(t_redir *r, int *out_fd)
{
	int	fd;

	if (r->type == R_IN)
		fd = open(r->word, open_mode(r->type));
	else
		fd = open(r->word, open_mode(r->type), 0644);
	if (fd < 0)
	{
		perror(r->word);
		return (1);
	}
	*out_fd = fd;
	return (0);
}

static void	apply_fd_to_slot(t_redir *r, int fd, int *fdin, int *fdout)
{
	if (r->type == R_IN)
		assign_fd(fdin, fd);
	else
		assign_fd(fdout, fd);
}

int	apply_redirs_files(t_cmd *c, int *fdin, int *fdout)
{
	t_redir	*r;
	int		fd;

	*fdin = -1;
	*fdout = -1;
	r = c->redirs;
	while (r)
	{
		if (r->type != R_HEREDOC)
		{
			if (open_redir_fd(r, &fd))
				return (1);
			apply_fd_to_slot(r, fd, fdin, fdout);
		}
		r = r->next;
	}
	return (0);
}
