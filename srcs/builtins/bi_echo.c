
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 10:55:00 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int is_n_option(const char *arg)
{
	int idx;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	idx = 2;
	while (arg[idx] == 'n')
		idx++;
	if (arg[idx] != '\0')
		return (0);
	return (1);
}

int bi_echo(char **args)
{
	int index;
	int print_newline;

	index = 0;
	print_newline = 1;
	while (args[index] && is_n_option(args[index]))
	{
		print_newline = 0;
		index++;
	}
	while (args[index])
	{
		ft_putstr_fd(args[index], STDOUT_FILENO);
		if (args[index + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		index++;
	}
	if (print_newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
