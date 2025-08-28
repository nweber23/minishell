/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parenthesis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:42:38 by nweber            #+#    #+#             */
/*   Updated: 2025/08/28 18:20:12 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_parenthesis(char c, int *balance)
{
	if (c == ')')
	{
		(*balance)--;
		if (*balance < 0)
			return (balance_message(-1), false);
	}
	else if (c == '(')
		(*balance)++;
	return (true);
}

bool	parenthesis_check(char *str)
{
	int		balance;
	int		i;
	bool	double_q;
	bool	single_q;

	balance = 0;
	i = 0;
	double_q = false;
	single_q = false;
	while (str[i])
	{
		quote_toggling(str[i], &double_q, &single_q);
		if (!single_q && !double_q
			&& !handle_parenthesis(str[i], &balance))
			return (false);
		i++;
	}
	if (balance != 0)
		return (balance_message(balance), false);
	return (true);
}
