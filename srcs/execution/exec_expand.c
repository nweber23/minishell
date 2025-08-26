/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:12 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:13 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"
#include <dirent.h>

static int	listdir_nonhidden(char ***outv, int *outc)
{
	DIR				*d;
	struct dirent	*de;
	char			**v;
	int				c;

	d = opendir(".");
	if (!d)
		return (0);
	v = NULL;
	c = 0;
	while ((de = readdir(d)) != NULL)
	{
		if (de->d_name[0] != '.')
		{
			if (!append_word(&v, &c, ft_strdup(de->d_name)))
			{
				closedir(d);
				return (0);
			}
		}
	}
	closedir(d);
	*outv = v;
	*outc = c;
	return (1);
}

char	**expand_argv_if_needed(char **argv)
{
	int		i;
	int		argc;
	char	**res;
	int		rc;

	if (!argv)
		return (NULL);
	i = 0;
	argc = 0;
	while (argv[argc])
		argc++;
	res = NULL;
	rc = 0;
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "*") == 0)
		{
			char **lst;
			int   cnt;
			if (!listdir_nonhidden(&lst, &cnt))
				return (ft_free2d(res), NULL);
			int j = 0;
			while (j < cnt)
			{
				if (!append_word(&res, &rc, lst[j]))
					return (ft_free2d(lst), ft_free2d(res), NULL);
				j++;
			}
			free(lst);
		}
		else
		{
			if (!append_word(&res, &rc, ft_strdup(argv[i])))
				return (ft_free2d(res), NULL);
		}
		i++;
	}
	return (res);
}
