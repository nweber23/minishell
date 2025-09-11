/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 17:30:30 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	is_num(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	bi_exit(t_shell_data *shell, char **args)
{
	int	exit_code_value;

	(void)shell;
	if (isatty(STDIN_FILENO))
        ft_putendl_fd("exit", 1);
	if (!args[0])
		exit(0);
	if (!is_num(args[0]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(2);
	}
	if (args[1])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	exit_code_value = ft_atoi(args[0]);
	exit(exit_code_value);
}
