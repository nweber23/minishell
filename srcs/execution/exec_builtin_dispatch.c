/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_dispatch.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:05 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/23 11:16:33 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	bi_dot(char **argv)
{
	if (!argv[0])
	{
		ft_putendl_fd("minishell: .: filename argument required", 2);
		ft_putendl_fd(".: usage: . filename [arguments]", 2);
		return (2);
	}
	if (opendir(argv[0]) != NULL)
	{
		ft_putendl_fd("minishell: .: is a directory", 2);
		return (126);
	}
	if (access(argv[0], F_OK) != 0)
	{
		perror("minishell: .");
		return (127);
	}
	ft_putendl_fd("minishell: .: not implemented", 2);
	return (2);
}

int	is_builtin(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp(name, "echo") == 0)
		return (1);
	if (ft_strcmp(name, "pwd") == 0)
		return (1);
	if (ft_strcmp(name, "cd") == 0)
		return (1);
	if (ft_strcmp(name, "env") == 0)
		return (1);
	if (ft_strcmp(name, "export") == 0)
		return (1);
	if (ft_strcmp(name, "unset") == 0)
		return (1);
	if (ft_strcmp(name, "exit") == 0)
		return (1);
	if (ft_strcmp(name, ".") == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_shell_data *sh, char **argv)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (bi_echo(argv + 1));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (bi_pwd());
	if (ft_strcmp(argv[0], "cd") == 0)
		return (bi_cd(sh, argv + 1));
	if (ft_strcmp(argv[0], "env") == 0)
	{
		if (argv[1] != NULL)
			return (127);
		return (bi_env(sh));
	}
	if (ft_strcmp(argv[0], "export") == 0)
		return (bi_export(sh, argv + 1));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (bi_unset(sh, argv + 1));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (bi_exit(sh, argv + 1));
	if (ft_strcmp(argv[0], ".") == 0)
		return (bi_dot(argv + 1));
	return (1);
}
