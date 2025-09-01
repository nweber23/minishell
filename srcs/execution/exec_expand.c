/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:12 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/01 09:48:58 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <dirent.h>

static int	has_star(const char *pattern)
{
	int	i;

	i = 0;
	while (pattern[i])
	{
		if (pattern[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

static int	glob_match(const char *pattern, const char *name)
{
	while (*pattern && *name)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			if (!*pattern)
				return (1);
			while (*name)
			{
				if (glob_match(pattern, name))
					return (1);
				name++;
			}
			return (0);
		}
		if (*pattern != *name)
			return (0);
		pattern++;
		name++;
	}
	while (*pattern == '*')
		pattern++;
	return (*pattern == '\0' && *name == '\0');
}

static int	should_skip_hidden(const char *pattern, const char *name)
{
	if (name[0] == '.' && pattern[0] != '.')
		return (1);
	return (0);
}

static int	push_name(char ***dst_vec, int *dst_cnt, const char *name)
{
	char	*dup_name;

	dup_name = ft_strdup(name);
	if (!dup_name)
		return (0);
	if (!append_word(dst_vec, dst_cnt, dup_name))
	{
		free(dup_name);
		return (0);
	}
	return (1);
}

static int	build_match_list(const char *pattern, char ***out_vec, int *out_cnt)
{
	DIR				*dir_ptr;
	struct dirent	*dir_ent;
	char			*entry_name;

	*out_vec = NULL;
	*out_cnt = 0;
	dir_ptr = opendir(".");
	if (!dir_ptr)
		return (0);
	dir_ent = readdir(dir_ptr);
	while (dir_ent)
	{
		entry_name = dir_ent->d_name;
		if (!should_skip_hidden(pattern, entry_name)
			&& glob_match(pattern, entry_name))
		{
			if (!push_name(out_vec, out_cnt, entry_name))
				return (closedir(dir_ptr), ft_array_free(*out_vec), 0);
		}
		dir_ent = readdir(dir_ptr);
	}
	closedir(dir_ptr);
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

static int	expand_pattern_word(const char *pattern,
		char ***dst_vec, int *dst_cnt)
{
	char	**match_vec;
	int		match_cnt;
	int		i;

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
	i = 0;
	while (i < match_cnt)
	{
		if (!append_word(dst_vec, dst_cnt, match_vec[i]))
			return (ft_array_free(match_vec), ft_array_free(*dst_vec), 0);
		i++;
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
			if (!expand_pattern_word(argv[source_index],
					&result_vec, &result_count))
				return (NULL);
		}
		else if (!append_copy(&result_vec, &result_count, argv[source_index]))
			return (NULL);
		source_index++;
	}
	return (result_vec);
}
