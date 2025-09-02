/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:28 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 10:00:38 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	open_mode(t_rdrtype type)
{
	if (type == R_IN)
		return (O_RDONLY);
	if (type == R_OUT_TRUNC)
		return (O_CREAT | O_WRONLY | O_TRUNC);
	if (type == R_OUT_APPEND)
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

static int	open_redir_fd(t_redir *redir, int *out_fd)
{
	int	fd;

	if (redir->type == R_IN)
		fd = open(redir->word, open_mode(redir->type));
	else
		fd = open(redir->word, open_mode(redir->type), 0644);
	if (fd < 0)
	{
		perror(redir->word);
		return (1);
	}
	*out_fd = fd;
	return (0);
}

static void	apply_fd_to_slot(t_redir *redir, int fd, int *fdin, int *fdout)
{
	if (redir->type == R_IN)
		assign_fd(fdin, fd);
	else
		assign_fd(fdout, fd);
}

int	apply_redirs_files(t_cmd *cmd, int *fdin, int *fdout)
{
	t_redir	*redir;
	int		fd;

	*fdin = -1;
	*fdout = -1;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type != R_HEREDOC)
		{
			if (open_redir_fd(redir, &fd))
				return (1);
			apply_fd_to_slot(redir, fd, fdin, fdout);
		}
		redir = redir->next;
	}
	return (0);
}
