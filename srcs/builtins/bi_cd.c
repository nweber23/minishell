
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 10:55:00 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	update_pwd_vars(t_shell_data *sh, const char *old_pwd)
{
	char	new_pwd[4096];
	char	*cwd_ptr;

	cwd_ptr = getcwd(new_pwd, sizeof(new_pwd));
	if (!cwd_ptr)
		return (perror("cd:getcwd"), 1);
	if (env_set(sh, "OLDPWD", old_pwd))
		return (1);
	if (env_set(sh, "PWD", new_pwd))
		return (1);
	return (0);
}

int	bi_cd(t_shell_data *sh, char **argv)
{
	char	old_pwd[4096];
	char	*target_dir;

	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (perror("cd:getcwd"), 1);
	if (!argv || !argv[0] || argv[0][0] == '\0')
	{
		target_dir = env_get(sh->env, "HOME");
		if (!target_dir)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), 1);
		if (chdir(target_dir) == -1)
			return (perror("cd"), 1);
		return (update_pwd_vars(sh, old_pwd));
	}
	if (chdir(argv[0]) == -1)
		return (perror("cd"), 1);
	return (update_pwd_vars(sh, old_pwd));
}
