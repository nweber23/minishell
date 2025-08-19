/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:57:39 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/17 19:43:53 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdlib.h>
#include <string.h>

void	free_split(char **v);

static int	count_parts(const char *s, char c)
{
	int	i;
	int	n;

	i = 0;
	n = 1;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			n++;
		i++;
	}
	return (n);
}

char	**split_colon(const char *s)
{
	char	**v;
	int		n;
	int		i;
	const char	*p;
	const char	*q;

	if (!s)
		return (NULL);
	n = count_parts(s, ':');
	v = (char **)malloc(sizeof(char *) * (n + 1));
	if (!v)
		return (NULL);
	i = 0;
	p = s;
	while (1)
	{
		if (*s == ':' || *s == '\0')
		{
			q = s;
			v[i] = (char *)malloc((q - p) + 1);
			if (!v[i])
				return (free_split(v), NULL);
			memcpy(v[i], p, (size_t)(q - p));
			v[i][q - p] = '\0';
			i++;
			if (*s == '\0')
				break ;
			p = s + 1;
		}
		s++;
	}
	v[i] = NULL;
	return (v);
}

void	free_split(char **v)
{
	int	i;

	if (!v)
		return ;
	i = 0;
	while (v[i])
	{
		free(v[i]);
		i++;
	}
	free(v);
}

char	*join_path(const char *dir, const char *leaf)
{
	size_t	ld;
	size_t	ll;
	char	*res;

	ld = strlen(dir);
	ll = strlen(leaf);
	res = (char *)malloc(ld + 1 + ll + 1);
	if (!res)
		return (NULL);
	memcpy(res, dir, ld);
	res[ld] = '/';
	memcpy(res + ld + 1, leaf, ll);
	res[ld + 1 + ll] = '\0';
	return (res);
}
