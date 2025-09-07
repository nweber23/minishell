/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:48:54 by nweber            #+#    #+#             */
/*   Updated: 2025/09/07 17:01:38 by nweber           ###   ########.fr       */
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
		return ;
	}
	if (shell->input == NULL || !ft_strcmp(shell->trimmed, "exit"))
	{
		exit_msg();
		free_shell(shell);
		return ;
	}
	lexer(shell, shell->trimmed);
	// shell->env_array = env_array(shell); // Builtin -> yyudi
	// shell->path = path(shell, envp); // Builtin -> yyudi
	shell->root = build_tree(shell, shell->tokens);
	// execution of ltree
	free_shell(shell);
	end_process(0);
	minishell_loop(shell, envp);
	return ;
}

int	end_process(int value)
{
	static int	code = 0;

	if (value == -1)
		return (code);
	code = value;
	return (code);
}

void	input(t_shell_data *shell)
{
	char	*cwd;
	char	*prompt1;
	char	*prompt2;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		error_malloc("input", shell);
	prompt1 = ft_strjoin("LECK_EIER_SHELL$ ", cwd);
	free(cwd);
	if (!prompt1)
		error_malloc("input", shell);
	prompt2 = ft_strjoin(prompt1, ">");
	free(prompt1);
	if (!prompt2)
		error_malloc("input", shell);
	shell->cwd = prompt2;
}
