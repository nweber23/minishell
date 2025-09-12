/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:28 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 20:57:01 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	assign_new_fd(int *slot_fd, int new_fd)
{
	if (*slot_fd != -1)
		close(*slot_fd);
	*slot_fd = new_fd;
}

static int	handle_input_redir(t_redir *redir, int *fd_in)
{
	int	fd_new;

	if (redir->type == R_IN)
	{
		fd_new = open_infile(redir->word);
		if (fd_new < 0)
			return (1);
		assign_new_fd(fd_in, fd_new);
		return (0);
	}
	if (redir->type == R_HEREDOC)
	{
		fd_new = heredoc_to_fd(redir);
		if (fd_new < 0)
			return (1);
		assign_new_fd(fd_in, fd_new);
		return (0);
	}
	return (0);
}

static int	handle_output_redir(t_redir *redir, int *fd_out)
{
	int	fd_new;

	if (redir->type == R_OUT_TRUNC)
	{
		fd_new = open_out_trunc(redir->word);
		if (fd_new < 0)
			return (1);
		assign_new_fd(fd_out, fd_new);
		return (0);
	}
	if (redir->type == R_OUT_APPEND)
	{
		fd_new = open_out_append(redir->word);
		if (fd_new < 0)
			return (1);
		assign_new_fd(fd_out, fd_new);
		return (0);
	}
	return (0);
}

static int	apply_one_redir(t_redir *redir, int *fd_in, int *fd_out)
{
	if (handle_input_redir(redir, fd_in) != 0)
		return (1);
	if (handle_output_redir(redir, fd_out) != 0)
		return (1);
	return (0);
}

int	apply_all_redirs(t_cmd *cmd, int *fd_in, int *fd_out)
{
	t_redir	*curr;

	if (fd_in == NULL || fd_out == NULL || cmd == NULL)
		return (1);
	*fd_in = -1;
	*fd_out = -1;
	curr = cmd->redirs;
	while (curr != NULL)
	{
		if (apply_one_redir(curr, fd_in, fd_out) != 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}
