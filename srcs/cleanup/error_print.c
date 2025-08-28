/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:10:31 by nweber            #+#    #+#             */
/*   Updated: 2025/08/28 18:20:20 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	error_message(char *str)
{
	if (str)
		ft_putendl_fd(str, 2);
	return (true);
}

void	balance_message(int balance)
{
	if (balance < 0)
		ft_putendl_fd(CLOSE_MSG, 2);
	else if (balance > 0)
		ft_putendl_fd(OPEN_MSG, 2);
}
