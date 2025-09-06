/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:46:28 by nweber            #+#    #+#             */
/*   Updated: 2025/09/06 15:50:05 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_wildcard(char **argv)
{
	int	i;

	i = 0;
	if (!argv || !argv[i])
		return (false);
	while (argv[i])
	{
		if (ft_strchr(argv[i], '*') && (ft_strcmp(argv[i], "*") == 0))
			return (true);
		i++;
	}
	return (false);
}

bool	is_valid_wildcard(char *str)
{
	if (ft_strcmp(str, "*") == 0)
		return (true);
	if (ft_strchr(str, '*'))
		return (true);
	return (false);
}
