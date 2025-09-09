/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/07 18:03:53 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	bi_pwd(void)
{
	char	cwd_buffer[4096];
	char	*cwd_result;

	cwd_result = getcwd(cwd_buffer, sizeof(cwd_buffer));
	if (!cwd_result)
		return (perror("pwd"), 1);
	ft_putendl_fd(cwd_result, STDOUT_FILENO);
	return (0);
}
