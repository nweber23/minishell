/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:12 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/27 10:46:12 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <dirent.h>

static int	listdir_open(DIR **dp)
{
	*dp = opendir(".");
	if (!*dp)
		return (0);
	return (1);
}

static int	listdir_push_visibles(DIR *d, char ***v, int *c)
{
	struct dirent	*de;
	char			*name;

	de = readdir(d);
	while (de != NULL)
	{
		if (de->d_name[0] != '.')
		{
			name = ft_strdup(de->d_name);
			if (!append_word(v, c, name))
				return (0);
		}
		de = readdir(d);
	}
	return (1);
}

static int	listdir_nonhidden(char ***outv, int *outc)
{
	DIR		*d;
	char	**v;
	int		c;

	*outv = NULL;
	*outc = 0;
	if (!listdir_open(&d))
		return (0);
	v = NULL;
	c = 0;
	if (!listdir_push_visibles(d, &v, &c))
	{
		closedir(d);
		return (0);
	}
	closedir(d);
	*outv = v;
	*outc = c;
	return (1);
}

/* ---------- public: simple "*" expansion ---------- */

static int	add_star_expand(char ***res, int *rc)
{
	char	**lst;
	int		cnt;
	int		j;

	if (!listdir_nonhidden(&lst, &cnt))
		return (0);
	j = 0;
	while (j < cnt)
	{
		if (!append_word(res, rc, lst[j]))
			return (ft_free2d(lst), ft_free2d(*res), 0);
		j++;
	}
	free(lst);
	return (1);
}

static int	add_normal_word(char ***res, int *rc, char *word)
{
	char	*cp;

	cp = ft_strdup(word);
	if (!append_word(res, rc, cp))
	{
		ft_free2d(*res);
		return (0);
	}
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
	argc = 0;
	while (argv[argc])
		argc++;
	res = NULL;
	rc = 0;
	i = 0;
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "*") == 0)
		{
			if (!add_star_expand(&res, &rc))
				return (NULL);
		}
		else if (!add_normal_word(&res, &rc, argv[i]))
			return (NULL);
		i++;
	}
	return (res);
}
