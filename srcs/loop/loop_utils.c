/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 19:58:03 by nweber            #+#    #+#             */
/*   Updated: 2025/09/07 18:03:32 by yyudi            ###   ########.fr       */
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
