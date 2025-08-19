/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:55:53 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/19 09:56:46 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static int	wire_stdio_for_stage(int i, int n, int prev[2], int next[2])
{
	if (i > 0)
	{
		if (dup2(prev[0], STDIN_FILENO) < 0)
			return (-1);
	}
	if (i < n - 1)
	{
		if (dup2(next[1], STDOUT_FILENO) < 0)
			return (-1);
	}
	return (0);
}

static void	exec_external_or_builtin_child(t_cmd *c)
{
	char	*path;
	char	**envp;

	if (c->is_builtin == 1)
		_exit(run_builtin_child(c));
	envp = ms_envp();
	path = ms_resolve_path(c->argv[0], envp);
	if (path == NULL)
	{
		fprintf(stderr, "minishell: %s: command not found\n", c->argv[0]);
		_exit(127);
	}
	execve(path, c->argv, envp);
	perror("minishell");
	_exit(126);
}

static void	close_unused_in_child(int i, int n, int prev[2], int next[2])
{
	if (i > 0)
		close(prev[1]);
	else
		close_pair(prev);
	if (i < n - 1)
		close(next[0]);
	else
		close_pair(next);
}

pid_t	spawn_child(int i, int n, int prev[2], int next[2], t_cmd *c)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		set_child_signals_default();
		if (wire_stdio_for_stage(i, n, prev, next) < 0)
			_exit(1);
		close_unused_in_child(i, n, prev, next);
		if (ms_apply_redirs(c) < 0)
			_exit(1);
		exec_external_or_builtin_child(c);
	}
	return (pid);
}
