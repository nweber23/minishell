/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:48:54 by nweber            #+#    #+#             */
/*   Updated: 2025/09/06 19:17:10 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_loop(t_shell_data *shell, char **envp)
{
	reset_shell(shell);
	interavtive_signals();
	input(shell);
	shell->input = readline(shell->cwd);
	if (shell->input && shell->input[0] != '\0')
		add_history(shell->input);
	if (shell->input && validate_input(shell))
	{
		free_shell(shell);
		minishell_loop(shell, envp);
	}
	if (shell->input == NULL || !ft_strcmp(shell->trimmed, "exit"))
	{
		exit_msg();
		free_shell(shell);
		return ;
	}
	lexer(shell, shell->trimmed);
	shell->env_array = env_array(shell);
	shell->path = path(shell, envp);
	shell->root = build_tree(shell, shell->tokens);
	// execution of ltree
	free_shell(shell);
	end_process(0);
	minishell_loop(shell, envp);
}
