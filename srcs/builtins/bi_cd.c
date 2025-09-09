/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/07 18:03:59 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	update_pwd_vars(t_shell_data *shell, const char *old_pwd)
{
	char	current_directory[4096];
	char	*cwd_result;

	cwd_result = getcwd(current_directory, sizeof(current_directory));
	if (!cwd_result)
		return (perror("cd:getcwd"), 1);
	if (env_set(shell, "OLDPWD", old_pwd))
		return (1);
	if (env_set(shell, "PWD", current_directory))
		return (1);
	return (0);
}

int	bi_cd(t_shell_data *shell, char **args)
{
	char	old_pwd[4096];
	char	*target_directory;

	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (perror("cd:getcwd"), 1);
	if (!args || !args[0] || args[0][0] == '\0')
	{
		target_directory = env_get(shell->env, "HOME");
		if (!target_directory)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), 1);
		if (chdir(target_directory) == -1)
			return (perror("cd"), 1);
		return (update_pwd_vars(shell, old_pwd));
	}
	if (chdir(args[0]) == -1)
		return (perror("cd"), 1);
	return (update_pwd_vars(shell, old_pwd));
}
