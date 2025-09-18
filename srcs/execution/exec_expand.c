/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:12 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/17 17:02:22 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
				return (ft_array_free(result_vec), NULL);
		}
		else if (!append_copy(&result_vec, &result_count, argv[source_index]))
			return (ft_array_free(result_vec), NULL);
		source_index++;
	}
	return (result_vec);
}
