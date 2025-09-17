/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:41:22 by nweber            #+#    #+#             */
/*   Updated: 2025/08/28 17:41:28 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	quote_check(char *str)
{
	int		i;
	char	current;

	i = 0;
	current = 0;
	while (str[i])
	{
		if (is_quote(str[i]))
		{
			if (current == 0)
				current = str[i];
			else if (current == str[i])
				current = 0;
		}
		i++;
	}
	return (current == 0);
}

bool	quote_position_check(char *str)
{
	int		i;
	char	c;
	bool	found;

	i = 0;
	c = 0;
	found = false;
	while (str[i])
	{
		if (is_quote(str[i]))
		{
			if (!found)
			{
				c = str[i];
				found = true;
			}
			else if (str[i] == c)
			{
				c = 0;
				found = false;
			}
		}
		i++;
	}
	return (!found);
}

int	check_quotes(t_shell_data *shell, char **value, char *str, int i)
{
	if (is_quote(str[i]))
		i = join_quotes(shell, value, str, i);
	else
		i = join_no_quotes(shell, value, str, i);
	return (i);
}
