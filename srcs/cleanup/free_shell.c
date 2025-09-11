/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 10:20:08 by nweber            #+#    #+#             */
/*   Updated: 2025/09/10 15:41:45 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_shell_cleanup(t_shell_data *shell)
{
	int	i;

	i = -1;
	if (shell->env_array)
	{
		while (shell->env_array[++i])
			free(shell->env_array[i]);
		free(shell->env_array);
		shell->env_array = NULL;
	}
	ft_lstclear(&shell->path, free);
	shell->path = NULL;
	if (shell->cwd)
		free(shell->cwd);
	shell->cwd = NULL;
	if (shell->input)
		free(shell->input);
	shell->input = NULL;
	if (shell->trimmed)
		free(shell->trimmed);
	shell->trimmed = NULL;
	if (shell->command_path)
		free(shell->command_path);
	shell->command_path = NULL;
}

void	free_shell(t_shell_data *shell)
{
	if (shell->root)
	{
		free_tree(shell->root);
		shell->root = NULL;
	}
	ft_lstclear(&shell->tokens, free_token);
	shell->tokens = NULL;
	free_shell_cleanup(shell);
}
