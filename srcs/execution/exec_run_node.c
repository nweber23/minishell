/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:33 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/20 13:45:47 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	run_exec_node(t_shell_data *sh, t_node *node, int pipe_fds[2], int is_top)
{
	pid_t		pid;
	int			in_fd;
	int			out_fd;
	const char	*name;

	if (node == NULL || node->cmd == NULL)
		return (1);
	name = NULL;
	if (node->cmd->argv != NULL)
		name = node->cmd->argv[0];
	select_inout_from_pair(pipe_fds, &in_fd, &out_fd);
	if (is_top != 0 && name != NULL && is_builtin(name) != 0)
		return (exec_builtin_in_parent(sh, node->cmd));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), close_pair_if_set(pipe_fds), 1);
	if (pid == 0)
	{
		child_exec(sh, node, in_fd, out_fd);
		_exit(127);
	}
	close_pair_if_set(pipe_fds);
	return (wait_and_status(pid));
}
