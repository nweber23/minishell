/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:26 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:27 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

static char	*join_path(const char *a, const char *b)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(a, "/");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, b);
	free(tmp);
	return (res);
}

char	*find_in_path(t_shell_data *sh, const char *cmd)
{
	char		*path;
	char		**split;
	char		*candidate;
	int			i;

	(void)sh;
	path = getenv("PATH");
	if (!path)
		return (NULL);
	split = ft_split(path, ':');
	if (!split)
		return (NULL);
	i = 0;
	while (split[i])
	{
		candidate = join_path(split[i], cmd);
		if (candidate && access(candidate, X_OK) == 0)
			return (ft_free2d(split), candidate);
		free(candidate);
		i++;
	}
	ft_free2d(split);
	return (NULL);
}
