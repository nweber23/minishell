/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:58:51 by nweber            #+#    #+#             */
/*   Updated: 2025/08/22 14:56:51 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_malloc(char *function, t_shell_data *shell)
{
	ft_putstr_fd("Error: Memory allocation failed in ", STDERR_FILENO);
	ft_putendl_fd(function, STDERR_FILENO);
	free_env(shell->env);
	free_shell(shell);
	exit(1);
}
