/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:26 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/23 11:00:01 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	*join_path(const char *prefix, const char *name)
{
	char	*with_slash;
	char	*full_path;

	with_slash = ft_strjoin(prefix, "/");
	if (!with_slash)
		return (NULL);
	full_path = ft_strjoin(with_slash, name);
	free(with_slash);
	return (full_path);
}

char	*find_in_path(t_shell_data *sh, const char *cmd_name)
{
	char	*path_value;
	char	**path_parts;
	char	*candidate_path;
	int		index;

	path_value = env_get(sh->env, "PATH");
	if (!path_value)
		return (NULL);
	path_parts = ft_split(path_value, ':');
	if (!path_parts)
		return (NULL);
	index = 0;
	while (path_parts[index])
	{
		candidate_path = join_path(path_parts[index], cmd_name);
		if (candidate_path && access(candidate_path, X_OK) == 0)
			return (ft_array_free(path_parts), candidate_path);
		free(candidate_path);
		index++;
	}
	ft_array_free(path_parts);
	return (NULL);
}
