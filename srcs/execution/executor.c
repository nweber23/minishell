/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:56:21 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/19 09:56:46 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static int	create_next_pipe(int i, int n, int next[2])
{
	if (i < n - 1)
	{
		if (pipe(next) < 0)
			return (-1);
	}
	else
	{
		next[0] = -1;
		next[1] = -1;
	}
	return (0);
}

static void	parent_close_and_rotate(int i, int n, int prev[2], int next[2])
{
	if (i > 0)
		close_pair(prev);
	if (i < n - 1)
	{
		prev[0] = next[0];
		prev[1] = next[1];
	}
	else
	{
		prev[0] = -1;
		prev[1] = -1;
	}
}

static int	wait_all(pid_t *pids, int n)
{
	int	i;
	int	st;
	int	last;

	i = 0;
	last = 0;
	while (i < n)
	{
		if (waitpid(pids[i], &st, 0) > 0)
		{
			if (i == n - 1)
			{
				if (WIFEXITED(st))
					last = WEXITSTATUS(st);
				else if (WIFSIGNALED(st))
					last = 128 + WTERMSIG(st);
			}
		}
		i++;
	}
	return (last);
}

static int	run_single_in_parent(t_cmd *c)
{
	int		fd[2];
	int		st;

	if (save_stdio(fd) < 0)
		return (1);
	if (ms_apply_redirs(c) < 0)
	{
		restore_stdio(fd);
		return (1);
	}
	st = run_builtin_parent(c);
	restore_stdio(fd);
	return (st);
}

int	exec_pipeline(t_pipeline *pl)
{
	int		i;
	int		prev[2];
	int		next[2];
	pid_t	*pids;

	if (pl == NULL || pl->count <= 0)
		return (0);
	if (ms_collect_all_heredoc(pl) < 0)
		return (1);
	set_parent_signals_interactive();
	if (pl->count == 1 && pl->cmds[0].is_builtin == 1)
		return (run_single_in_parent(&pl->cmds[0]));
	pids = (pid_t *)malloc(sizeof(pid_t) * pl->count);
	if (pids == NULL)
		return (1);
	prev[0] = -1;
	prev[1] = -1;
	i = 0;
	while (i < pl->count)
	{
		if (create_next_pipe(i, pl->count, next) < 0)
			return (free(pids), 1);
		pids[i] = spawn_child(i, pl->count, prev, next, &pl->cmds[i]);
		if (pids[i] < 0)
			return (free(pids), 1);
		parent_close_and_rotate(i, pl->count, prev, next);
		i++;
	}
	close_pair(prev);
	i = wait_all(pids, pl->count);
	free(pids);
	return (i);
}
