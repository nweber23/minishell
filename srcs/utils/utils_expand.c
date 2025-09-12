/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:36:06 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/12 10:23:53 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	has_star(const char *pattern)
{
	int	index;

	index = 0;
	while (pattern[index])
	{
		if (pattern[index] == '*')
			return (1);
		index++;
	}
	return (0);
}

int	glob_match(const char *pattern, const char *name)
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

int	should_skip_hidden(const char *pattern, const char *name)
{
	if (name[0] == '.' && pattern[0] != '.')
		return (1);
	return (0);
}
