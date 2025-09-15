/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:34 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/15 12:25:05 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	close_both(int pipe_fds[2])
{
	close(pipe_fds[0]);
	close(pipe_fds[1]);
}

void	close_redir_placeholders(t_redir *head)
{
	while (head)
	{
		if (head->word && ft_strncmp(head->word, FD_PREFIX,
				ft_strlen(FD_PREFIX)) == 0 && head->quoted_delim > 0)
		{
			close(head->quoted_delim);
			head->quoted_delim = 0;
		}
		head = head->next;
	}
}

static int	make_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), 1);
	fcntl(pipe_fds[0], F_SETFD, FD_CLOEXEC);
	fcntl(pipe_fds[1], F_SETFD, FD_CLOEXEC);
	return (0);
}

static pid_t	fork_left(t_shell_data *sh, t_node *node, int pipe_fds[2])
{
	pid_t	pid;
	int		fds_left[2];
	int		devnull_fd;
	int		st;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pipe_fds[0]);
		fds_left[1] = pipe_fds[1];
		if (right_is_redir_only(node->right))
		{
			devnull_fd = open("/dev/null", O_RDONLY);
			if (devnull_fd == -1)
				_exit(1);
			fds_left[0] = devnull_fd;
		}
		else
			fds_left[0] = -1;
		{
			st = run_exec_node(sh, node->left, fds_left, 0);
			close_pair_if_set(fds_left);
			if (node->left && node->left->cmd)
				close_redir_placeholders(node->left->cmd->redirs);
			if (node->right && node->right->cmd)
				close_redir_placeholders(node->right->cmd->redirs);
			combine(sh);
			_exit(st);
		}
	}
	return (pid);
}

static pid_t	fork_right(t_shell_data *sh, t_node *node, int pipe_fds[2])
{
	pid_t	pid;
	int		fds_right[2];
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
		fds_right[0] = pipe_fds[0];
		fds_right[1] = -1;
		{
			st = run_exec_node(sh, node->right, fds_right, 0);
			close_pair_if_set(fds_right);
			if (node->left && node->left->cmd)
				close_redir_placeholders(node->left->cmd->redirs);
			if (node->right && node->right->cmd)
				close_redir_placeholders(node->right->cmd->redirs);
			combine(sh);
			_exit(st);
		}
	}
	return (pid);
}

int	run_pipe(t_shell_data *sh, t_node *node, int is_top)
{
	int		pipe_fds[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		right_status;

	(void)is_top;
	if (make_pipe(pipe_fds))
		return (1);
	left_pid = fork_left(sh, node, pipe_fds);
	if (left_pid == -1)
		return (close_both(pipe_fds), 1);
	right_pid = fork_right(sh, node, pipe_fds);
	if (right_pid == -1)
	{
		close_both(pipe_fds);
		(void)wait_status(left_pid);
		return (1);
	}
	close_both(pipe_fds);
	(void)wait_status(left_pid);
	right_status = wait_status(right_pid);
	return (right_status);
}
