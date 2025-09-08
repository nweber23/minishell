/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 19:58:03 by nweber            #+#    #+#             */
/*   Updated: 2025/09/07 13:08:55 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_shell(t_shell_data *shell)
{
	shell->command_path = NULL;
	shell->cwd = NULL;
	shell->env_array = NULL;
	shell->input = NULL;
	shell->path = NULL;
	shell->root = NULL;
	shell->tokens = NULL;
	shell->trimmed = NULL;
}
