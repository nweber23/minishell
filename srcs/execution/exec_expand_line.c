/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 20:40:30 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/08 20:41:26 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	is_name_char(int c)
{
	if (ft_isalnum(c) != 0)
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

static char	*append_chunk(char *acc, const char *s, size_t len)
{
	char	*part;
	char	*tmp;

	part = ft_substr(s, 0, len);
	if (part == NULL)
		return (NULL);
	if (acc == NULL)
		return (part);
	tmp = ft_strjoin(acc, part);
	free(acc);
	free(part);
	return (tmp);
}

static char	*append_str(char *acc, const char *s)
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

static char	*status_to_str(void)
{
	int		code;
	char	*s;

	code = exit_code(-1);
	s = ft_itoa(code);
	return (s);
}

char	*expand_line_env(t_shell_data *sh, char *line)
{
	size_t	i;
	size_t	start;
	char	*out;
	char	*val;
	char	*name;

	if (line == NULL)
		return (NULL);
	out = NULL;
	i = 0;
	start = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
		{
			/* flush chunk sebelum '$' */
			if (i > start)
			{
				out = append_chunk(out, line + start, i - start);
				if (out == NULL)
					return (NULL);
			}
			/* handle "$?" */
			if (line[i + 1] == '?')
			{
				val = status_to_str();
				out = append_str(out, val);
				free(val);
				i = i + 2;
				start = i;
				continue ;
			}
			/* handle "$NAME" */
			i = i + 1;
			if (is_name_char((int)line[i]) == 0)
			{
				/* bukan nama variabel → biarkan '$' literal */
				out = append_str(out, "$");
				start = i;
				continue ;
			}
			name = NULL;
			{
				size_t j;

				j = i;
				while (line[j] != '\0' && is_name_char((int)line[j]) != 0)
					j++;
				name = ft_substr(line, i, j - i);
				if (name == NULL)
					return (NULL);
				val = (char *)env_get(sh->env, name);
				if (val != NULL)
				{
					out = append_str(out, val);
					if (out == NULL)
					{
						free(name);
						return (NULL);
					}
				}
				free(name);
				i = j;
				start = i;
			}
			continue ;
		}
		i++;
	}
	/* flush sisa tail */
	if (i > start)
		out = append_chunk(out, line + start, i - start);
	if (out == NULL)
		out = ft_strdup("");
	return (out);
}
