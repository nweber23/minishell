/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expand_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 20:18:40 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/16 16:52:24 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*exp_join_n(char *acc, const char *s, size_t n)
{
	char	*part;
	char	*tmp;

	part = ft_substr(s, 0, n);
	if (part == NULL)
		return (NULL);
	if (acc == NULL)
		return (part);
	tmp = ft_strjoin(acc, part);
	free(acc);
	free(part);
	return (tmp);
}

char	*exp_join(char *acc, const char *s)
{
	char	*tmp;

	if (s == NULL)
		return (acc);
	if (acc == NULL)
		return (ft_strdup(s));
	tmp = ft_strjoin(acc, s);
	free(acc);
	return (tmp);
}

size_t	exp_scan_name(const char *line, size_t pos)
{
	size_t	j;

	j = pos;
	while (line[j] != '\0'
		&& (ft_isalnum((int)line[j]) != 0 || line[j] == '_'))
		j++;
	return (j);
}

char	*exp_status_str(t_shell_data *sh)
{
	return (ft_itoa(sh->exit_code));
}

char	*exp_env_value_dup(t_shell_data *sh, const char *line, size_t *i)
{
	size_t	end;
	char	*name;
	char	*val;
	char	*dup;

	if ((unsigned char)line[*i] == 0x1D)
		return (ft_strdup(""));
	if (line[*i] == '\0'
		|| (ft_isalnum((int)line[*i]) == 0 && line[*i] != '_'))
		return (ft_strdup("$"));
	end = exp_scan_name(line, *i);
	name = ft_substr(line, *i, end - *i);
	if (name == NULL)
		return (NULL);
	val = env_get(sh->env, name);
	free(name);
	if (val == NULL)
		dup = ft_strdup("");
	else
		dup = ft_strdup(val);
	*i = end;
	return (dup);
}