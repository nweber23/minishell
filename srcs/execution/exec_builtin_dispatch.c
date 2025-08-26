/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_dispatch.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:05 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:06 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

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
		return (bi_env(sh));
	if (ft_strcmp(argv[0], "export") == 0)
		return (bi_export(sh, argv + 1));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (bi_unset(sh, argv + 1));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (bi_exit(sh, argv + 1));
	return (1);
}
