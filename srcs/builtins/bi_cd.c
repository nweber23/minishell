
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 10:55:00 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	bi_cd(t_shell_data *sh, char **argv)
{
	(void)sh;
	if (!argv[0])
	{
		ft_putendl_fd("minishell: cd: missing argument", 2);
		return (1);
	}
	if (chdir(argv[0]) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
