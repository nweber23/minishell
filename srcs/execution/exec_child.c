/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:55:54 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/20 14:01:34 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	child_setup_fds(t_node *node, t_fdpack *fd_pack, int in_fd,
		int out_fd)
{
	int	orig_in;
	int	orig_out;

	orig_in = in_fd;
	orig_out = out_fd;
	fdpack_init(fd_pack);
	if (apply_all_redirs(node->cmd, &fd_pack->in, &fd_pack->out) != 0)
		_exit(1);
	if (fd_pack->in == -1)
		fd_pack->in = orig_in;
	else if (orig_in != -1)
		close(orig_in);
	if (fd_pack->out == -1)
		fd_pack->out = orig_out;
	else if (orig_out != -1)
		close(orig_out);
	apply_dup_and_close(fd_pack->in, STDIN_FILENO);
	apply_dup_and_close(fd_pack->out, STDOUT_FILENO);
}

static void	child_exec_cmd(t_shell_data *sh, t_node *node)
{
	int	exit_status;

	if (node->cmd->argv == NULL || node->cmd->argv[0] == NULL)
	{
		combine(sh);
		_exit(0);
	}
	expand_argv_inplace(node->cmd);
	if (is_builtin(node->cmd->argv[0]) != 0)
	{
		exit_status = exec_builtin(sh, node->cmd->argv);
		combine(sh);
		_exit(exit_status);
	}
	exit_status = exec_external(sh, node->cmd);
	combine(sh);
	_exit(exit_status);
}

void	child_exec(t_shell_data *sh, t_node *node, int in_fd, int out_fd)
{
	t_fdpack	fd_pack;
	int			status;

	cleanup_readline_tty(sh);
	reset_child_signals();
	if (node->type == ND_PIPE)
	{
		if (in_fd >= 0 && dup2(in_fd, STDIN_FILENO) == -1)
			_exit(1);
		if (out_fd >= 0 && dup2(out_fd, STDOUT_FILENO) == -1)
			_exit(1);
		if (in_fd >= 0)
			close(in_fd);
		if (out_fd >= 0)
			close(out_fd);
		status = run_pipe(sh, node, 0);
		_exit(status);
	}
	child_setup_fds(node, &fd_pack, in_fd, out_fd);
	child_exec_cmd(sh, node);
}

