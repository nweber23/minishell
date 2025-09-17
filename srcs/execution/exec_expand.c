/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:12 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 19:35:48 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// static int	push_name(char ***dst_vec, int *dst_cnt, const char *name)
// {
// 	char	*dup_name;

// 	dup_name = ft_strdup(name);
// 	if (!dup_name)
// 		return (0);
// 	if (!append_word(dst_vec, dst_cnt, dup_name))
// 	{
// 		free(dup_name);
// 		return (0);
// 	}
// 	return (1);
// }

static int	build_match_list(const char *pattern, char ***out_vec, int *out_cnt)
{
	DIR				*dir_ptr;
	struct dirent	*dir_ent;
	const char		*name_pattern;
	char			*dirpath;
	const char		*slash;
	char			*full_entry;
	size_t			dlen;

	*out_vec = NULL;
	*out_cnt = 0;
	slash = ft_strrchr(pattern, '/');
	if (slash)
	{
		dlen = (size_t)(slash - pattern);
		if (dlen == 0)
			dirpath = ft_strdup("/");
		else
			dirpath = ft_substr(pattern, 0, dlen);
		name_pattern = slash + 1;
	}
	else
	{
		dirpath = ft_strdup(".");
		name_pattern = pattern;
	}
	if (!dirpath)
		return (0);
	if (*name_pattern == '\0')
		name_pattern = "*";
	dir_ptr = opendir(dirpath);
	if (!dir_ptr)
		return (free(dirpath), 0);
	dir_ent = readdir(dir_ptr);
	while (dir_ent)
	{
		if (!should_skip_hidden(name_pattern, dir_ent->d_name)
			&& glob_match(name_pattern, dir_ent->d_name))
		{
			if (ft_strcmp(dirpath, ".") == 0)
				full_entry = ft_strdup(dir_ent->d_name);
			else if (ft_strcmp(dirpath, "/") == 0)
				full_entry = ft_str3var("/", dir_ent->d_name, "");
			else
				full_entry = ft_str3var(dirpath, "/", dir_ent->d_name);
			if (!full_entry || !append_word(out_vec, out_cnt, full_entry))
			{
				if (full_entry)
					free(full_entry);
				closedir(dir_ptr);
				ft_array_free(*out_vec);
				free(dirpath);
				return (0);
			}
		}
		dir_ent = readdir(dir_ptr);
	}
	closedir(dir_ptr);
	free(dirpath);
	return (1);
}

static int	append_copy(char ***dst_vec, int *dst_cnt, const char *word)
{
	char	*dup_word;

	dup_word = ft_strdup(word);
	if (!dup_word)
		return (0);
	if (!append_word(dst_vec, dst_cnt, dup_word))
	{
		free(dup_word);
		return (0);
	}
	return (1);
}

static int	expand_pattern_word(const char *pattern, char ***dst_vec,
		int *dst_cnt)
{
	char	**match_vec;
	int		match_cnt;
	int		index;

	match_vec = NULL;
	match_cnt = 0;
	if (!build_match_list(pattern, &match_vec, &match_cnt))
		return (0);
	if (match_cnt == 0)
	{
		if (!append_copy(dst_vec, dst_cnt, pattern))
			return (ft_array_free(match_vec), 0);
		return (ft_array_free(match_vec), 1);
	}
	index = 0;
	while (index < match_cnt)
	{
		if (!append_word(dst_vec, dst_cnt, match_vec[index]))
			return (ft_array_free(match_vec), ft_array_free(*dst_vec), 0);
		index++;
	}
	free(match_vec);
	return (1);
}

char	**expand_argv_if_needed(char **argv)
{
	int		source_index;
	int		result_count;
	char	**result_vec;

	if (!argv)
		return (NULL);
	source_index = 0;
	result_count = 0;
	result_vec = NULL;
	while (argv[source_index])
	{
		if (has_star(argv[source_index]))
		{
			if (!expand_pattern_word(argv[source_index], &result_vec,
					&result_count))
				return (NULL);
		}
		else if (!append_copy(&result_vec, &result_count, argv[source_index]))
			return (NULL);
		source_index++;
	}
	return (result_vec);
}
