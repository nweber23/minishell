/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_sentinel.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 16:39:04 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/20 10:10:03 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_name_start(unsigned char c)
{
	if (ft_isalpha((int)c) || c == '_')
		return (1);
	return (0);
}

static int	is_name_char(unsigned char c)
{
	if (ft_isalnum((int)c) || c == '_')
		return (1);
	return (0);
}

static size_t	scan_name(const char *s, size_t i)
{
	while (s[i] != '\0' && is_name_char((unsigned char)s[i]))
		i++;
	return (i);
}

static char	*dup_env_value(t_env *env, const char *name)
{
	char	*val;

	val = env_get(env, (char *)name);
	if (val == NULL)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

char	*exp_env_value_dup(t_shell_data *sh, const char *line, size_t *i)
{
	size_t	end;
	char	*name;
	char	*dup;

	if (line[*i] == '\0')
		return (ft_strdup("$"));
	if ((unsigned char)line[*i] == (unsigned char)0x1D)
	{
		return (ft_strdup(""));
	}
	if (line[*i] == '?')
		return (exp_expand_exit_status(sh, i));
	if (!is_name_start((unsigned char)line[*i]))
		return (ft_strdup("$"));
	end = scan_name(line, *i);
	name = ft_substr(line, *i, end - *i);
	if (name == NULL)
		return (NULL);
	dup = dup_env_value(sh->env, name);
	free(name);
	*i = end;
	return (dup);
}
