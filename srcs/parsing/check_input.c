/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:12:36 by nweber            #+#    #+#             */
/*   Updated: 2025/08/26 10:29:11 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	pipe_check(char *str)
{
	int		i;
	bool	double_q;
	bool	singel_q;

	i = 0;
	double_q = false;
	singel_q = false;
	if (str[0] == '|' || str[ft_strlen(str) - 1] == '|')
		return (false);
	while (str[i])
	{
		quote_toggling(str[i], &double_q, &singel_q);
		if (str[i] == '|' && !singel_q && !double_q)
		{
			if (str[i + 1] == '|')
				i++;
			while (is_space(str[++i]))
				;
			if (str[i] == '|')
				return (false);
		}
		i++;
	}
	return (str[i - 1] != '|' || singel_q || double_q);
}

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

bool	redirect_check(char *str)
{
	int	i;
	int	len;

	i = 0;
	while (str[i])
	{
		i = remove_quotes(str, i);
		len = is_redirect(str);
		if (len > 0)
		{
			if (!error_redirect(str, &i, len))
				return (false);
			if (!str[i])
				return (error_message(REDIRECT_MSG), false);
			continue ;
		}
		i++;
	}
	return (true);
}

bool	parenthesis_check(char *str)
{
	int	balance;
	int	i;

	balance = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == ')' && balance == 0)
			return (balance_message(-1), false);
		if (str[i] == '(')
			balance++;
		if (str[i] == ')')
			balance--;
		i++;
	}
	if (balance != 0)
		return (balance_message(balance), false);
	if (balance == 0)
		return (true);
	return (false);
}
