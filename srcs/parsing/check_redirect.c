/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:42:03 by nweber            #+#    #+#             */
/*   Updated: 2025/08/28 17:43:23 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_redirect(char *str, int *i, int len)
{
	if (!error_redirect(str, i, len))
		return (false);
	while (str[*i] && is_space(str[*i]))
		(*i)++;
	if (!str[*i])
		return (error_message(REDIRECT_MSG), false);
	return (true);
}

bool	redirect_check(char *str)
{
	int		i;
	bool	double_q;
	bool	single_q;

	i = 0;
	double_q = false;
	single_q = false;
	while (str[i])
	{
		quote_toggling(str[i], &double_q, &single_q);
		if (single_q || double_q)
		{
			i++;
			continue ;
		}
		int len = is_redirect(&str[i]);
		if (len > 0)
		{
			if (!handle_redirect(str, &i, len))
				return (false);
			continue ;
		}
		i++;
	}
	return (true);
}
