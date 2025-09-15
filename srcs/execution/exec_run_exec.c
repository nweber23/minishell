/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:30 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/15 18:58:03 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	free_argv_vec(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	restore_masked_stars(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		if (s[i] == '\a')
			s[i] = '*';
		i++;
	}
}

void	expand_argv_inplace(t_cmd *cmd)
{
	char	**orig;
	char	**expanded;
	int		i;

	if (!cmd)
		return ;
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
		{
			restore_masked_stars(cmd->argv[i]);
			i++;
		}
	}
	orig = cmd->argv;
	expanded = expand_argv_if_needed(orig);
	if (expanded && expanded != orig)
	{
		free_argv_vec(orig);
		cmd->argv = expanded;
	}
}

int	exec_builtin_in_parent(t_shell_data *sh, t_cmd *cmd)
{
	t_fdpack	fd_pack;
	int			exit_status;

	fdpack_init(&fd_pack);
	if (apply_all_redirs(cmd, &fd_pack.in, &fd_pack.out) != 0)
	{
		fd_restore(&fd_pack);
		return (1);
	}
	fd_apply_inout(&fd_pack);
	expand_argv_inplace(cmd);
	exit_status = exec_builtin(sh, cmd->argv);
	fd_restore(&fd_pack);
	return (exit_status);
}
