/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:34 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/15 19:20:21 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	close_redir_placeholders(t_cmd *cmd)
{
	t_redir	*r;

	if (!cmd)
		return ;
	r = cmd->redirs;
	while (r)
	{
		if (r->word && ft_strncmp(r->word, FD_PREFIX, ft_strlen(FD_PREFIX)) == 0
			&& r->quoted_delim > 0)
		{
			close(r->quoted_delim);
			r->quoted_delim = 0;
		}
		r = r->next;
	}
}

static void	child_cleanup_and_exit(t_shell_data *sh, t_node *node, int st)
{
	if (node && node->left && node->left->cmd)
		close_redir_placeholders(node->left->cmd);
	if (node && node->right && node->right->cmd)
		close_redir_placeholders(node->right->cmd);
	combine(sh);
	_exit(st);
}

static pid_t	spawn_left(t_shell_data *sh, t_node *node, int pipe_fds[2])
{
	pid_t	pid;
	int		fds[2];
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pipe_fds[0]);
		fds[1] = pipe_fds[1];
		fds[0] = -1;
		status = run_exec_node(sh, node->left, fds, 0);
		close_pair_if_set(fds);
		child_cleanup_and_exit(sh, node, status);
	}
	return (pid);
}

static pid_t	spawn_right(t_shell_data *sh, t_node *node, int pipe_fds[2])
{
	pid_t	pid;
	int		fds[2];
	int		st;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		if (right_is_redir_only(node->right))
		{
			close(pipe_fds[0]);
			close(pipe_fds[1]);
			child_exec(sh, node->right, -1, -1);
		}
		close(pipe_fds[1]);
		fds[0] = pipe_fds[0];
		fds[1] = -1;
		st = run_exec_node(sh, node->right, fds, 0);
		close_pair_if_set(fds);
		child_cleanup_and_exit(sh, node, st);
	}
	return (pid);
}

int	run_pipe(t_shell_data *sh, t_node *node, int is_top)
{
	int		pipe_fds[2];
	pid_t	lpid;
	pid_t	rpid;
	int		rst;

	(void)is_top;
	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), 1);
	lpid = spawn_left(sh, node, pipe_fds);
	if (lpid == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (1);
	}
	rpid = spawn_right(sh, node, pipe_fds);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	if (rpid == -1)
		return ((void)wait_status(lpid), 1);
	(void)wait_status(lpid);
	rst = wait_status(rpid);
	return (rst);
}
