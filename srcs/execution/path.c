/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:56:50 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/19 09:56:46 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static int	has_slash(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

static char	*find_env_value(char **envp, const char *key)
{
	size_t	klen;
	int		i;

	klen = strlen(key);
	i = 0;
	while (envp && envp[i])
	{
		if (strncmp(envp[i], key, klen) == 0 && envp[i][klen] == '=')
			return (envp[i] + klen + 1);
		i++;
	}
	return (NULL);
}

static char	*try_in_paths(char **paths, const char *cmd)
{
	int		i;
	char	*try;

	i = 0;
	while (paths && paths[i])
	{
		try = join_path(paths[i], cmd);
		if (try != NULL && access(try, X_OK) == 0)
			return (try);
		free(try);
		i++;
	}
	return (NULL);
}

char	*ms_resolve_path(const char *cmd, char **envp)
{
	char	**paths;
	char	*penv;
	char	*try;

	if (cmd == NULL || cmd[0] == '\0')
		return (NULL);
	if (has_slash(cmd) == 1)
		return (strdup(cmd));
	penv = find_env_value(envp, "PATH");
	if (penv == NULL)
		return (NULL);
	paths = split_colon(penv);
	try = try_in_paths(paths, cmd);
	free_split(paths);
	return (try);
}
