/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:42:03 by nweber            #+#    #+#             */
/*   Updated: 2025/09/08 10:22:48 by nweber           ###   ########.fr       */
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

static bool	skip_quoted_segment(char *str, int *i, bool *double_q,
		bool *single_q)
{
	while (str[++(*i)])
	{
		quote_toggling(str[*i], double_q, single_q);
		if (!*single_q && !*double_q)
			break ;
	}
	if (!str[*i])
		return (false);
	return (true);
}

static int	process_quoted(char *str, int *i, bool *double_q, bool *single_q)
{
	if (!(*single_q) && !(*double_q))
		return (0);
	if (!skip_quoted_segment(str, i, double_q, single_q))
		return (-1);
	return (1);
}

static int	process_index(char *str, int *i, bool *dq, bool *sq)
{
	int	len;
	int	pq;

	pq = process_quoted(str, i, dq, sq);
	if (pq == 1)
		return (1);
	if (pq == -1)
		return (-1);
	len = is_redirect(&str[*i]);
	if (len > 0)
	{
		if (!handle_redirect(str, i, len))
			return (-1);
		return (1);
	}
	return (0);
}

bool	redirect_check(char *str)
{
	int		i;
	bool	double_q;
	bool	single_q;
	int		rc;

	i = -1;
	double_q = false;
	single_q = false;
	while (str[++i])
	{
		quote_toggling(str[i], &double_q, &single_q);
		rc = process_index(str, &i, &double_q, &single_q);
		if (rc == 1)
			continue ;
		if (rc == -1)
			break ;
	}
	return (true);
}
