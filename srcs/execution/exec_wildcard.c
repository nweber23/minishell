/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 17:03:04 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/17 17:05:32 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static char	*build_full_entry(const char *dirpath, const char *name)
{
	char	*full;

	full = NULL;
	if (ft_strcmp(dirpath, ".") == 0)
		full = ft_strdup(name);
	else if (ft_strcmp(dirpath, "/") == 0)
		full = ft_str3var("/", name, "");
	else
		full = ft_str3var(dirpath, "/", name);
	return (full);
}

static char	*get_dirpath(const char *pattern, const char **name_pattern)
{
	const char	*slash;
	size_t		dlen;

	*name_pattern = pattern;
	slash = ft_strrchr(pattern, '/');
	if (slash)
	{
		dlen = (size_t)(slash - pattern);
		if (dlen == 0)
			return (ft_strdup("/"));
		*name_pattern = slash + 1;
		return (ft_substr(pattern, 0, dlen));
	}
	return (ft_strdup("."));
}

static char	**collect_matches(DIR *dirp, const char *dirpath,
		const char *name_pattern, int *match_cnt)
{
	struct dirent	*ent;
	char			**matches;
	char			*full;

	*match_cnt = 0;
	matches = NULL;
	ent = readdir(dirp);
	while (ent)
	{
		if (!should_skip_hidden(name_pattern, ent->d_name)
			&& glob_match(name_pattern, ent->d_name))
		{
			full = build_full_entry(dirpath, ent->d_name);
			if (!full || !append_word(&matches, match_cnt, full))
			{
				if (full)
					free(full);
				ft_array_free(matches);
				return (NULL);
			}
		}
		ent = readdir(dirp);
	}
	return (matches);
}

int	build_match_list(const char *pattern, char ***out_vec, int *out_cnt)
{
	const char	*name_pat;
	char		*dirpath;
	DIR			*dirp;
	char		**matches;
	int			count;

	*out_vec = NULL;
	*out_cnt = 0;
	dirpath = get_dirpath(pattern, &name_pat);
	if (!dirpath)
		return (0);
	if (*name_pat == '\0')
		name_pat = "*";
	dirp = opendir(dirpath);
	if (!dirp)
		return (free(dirpath), 0);
	matches = collect_matches(dirp, dirpath, name_pat, &count);
	closedir(dirp);
	free(dirpath);
	if (!matches)
		return (0);
	*out_vec = matches;
	*out_cnt = count;
	return (1);
}
