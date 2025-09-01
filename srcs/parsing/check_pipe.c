/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:40:26 by nweber            #+#    #+#             */
/*   Updated: 2025/08/28 17:40:53 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_and(char *str, int *i, bool single_q, bool double_q)
{
	if (str[*i] == '&' && str[*i + 1] == '&' && !single_q && !double_q)
	{
		if (*i == 0 || str[*i + 2] == '&')
			return (false);
		*i += 2;
		while (str[*i] && is_space(str[*i]))
			(*i)++;
		if (!str[*i])
			return (false);
	}
	return (true);
}

static bool	handle_pipe(char *str, int *i, bool single_q, bool double_q)
{
	if (str[*i] == '|' && !single_q && !double_q)
	{
		if (str[*i + 1] == '|')
		{
			if (*i == 0 || str[*i + 2] == '|')
				return (false);
			*i += 2;
			while (str[*i] && is_space(str[*i]))
				(*i)++;
			if (!str[*i])
				return (false);
		}
		else
		{
			(*i)++;
			while (str[*i] && is_space(str[*i]))
				(*i)++;
			if (str[*i] == '|')
				return (false);
		}
	}
	return (true);
}

bool	pipe_check(char *str)
{
	int		i;
	bool	double_q;
	bool	single_q;

	i = 0;
	double_q = false;
	single_q = false;
	if (str[0] == '|' || str[ft_strlen(str) - 1] == '|')
		return (false);
	if ((str[0] == '&' && str[1] == '&' && (!str[2] || is_space(str[2])))
		|| (str[0] == '|' && str[1] == '|' && (!str[2] || is_space(str[2]))))
		return (false);
	while (str[i])
	{
		quote_toggling(str[i], &double_q, &single_q);
		if (!handle_and(str, &i, single_q, double_q)
			|| !handle_pipe(str, &i, single_q, double_q))
			return (false);
		i++;
	}
	return (true);
}
