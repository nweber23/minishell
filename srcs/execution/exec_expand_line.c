/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expand_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 20:40:30 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 20:35:09 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	exp_seek_dollar(const char *line, size_t *i)
{
	while (line[*i] != '\0' && line[*i] != '$')
		*i = *i + 1;
	return (line[*i] == '$');
}

int	exp_flush_segment(char **out, const char *line, size_t start, size_t end)
{
	if (end > start)
	{
		*out = exp_join_n(*out, line + start, end - start);
		if (*out == NULL)
			return (0);
	}
	return (1);
}

int	exp_expand_at(t_shell_data *sh, char **out, const char *line, size_t *i)
{
	char	*piece;

	if (line[*i] != '$')
		return (0);
	*i = *i + 1;
	if (line[*i] == '?')
	{
		piece = exp_status_str(sh);
		if (piece == NULL)
			return (0);
		*out = exp_join(*out, piece);
		free(piece);
		if (*out == NULL)
			return (0);
		*i = *i + 1;
		return (1);
	}
	piece = exp_env_value_dup(sh, line, i);
	if (piece == NULL)
		return (0);
	*out = exp_join(*out, piece);
	free(piece);
	return (*out != NULL);
}

char	*expand_line_env(t_shell_data *sh, char *line)
{
	char	*out;
	size_t	i;
	size_t	start;

	if (line == NULL)
		return (NULL);
	out = NULL;
	i = 0;
	start = 0;
	while (exp_seek_dollar(line, &i))
	{
		if (exp_flush_segment(&out, line, start, i) == 0)
			return (NULL);
		if (exp_expand_at(sh, &out, line, &i) == 0)
			return (NULL);
		start = i;
	}
	if (exp_flush_segment(&out, line, start, i) == 0)
		return (NULL);
	if (out == NULL)
		out = ft_strdup("");
	return (out);
}
