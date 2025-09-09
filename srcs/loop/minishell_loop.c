/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:48:54 by nweber            #+#    #+#             */
/*   Updated: 2025/09/09 13:28:46 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_loop(t_shell_data *shell, char **envp)
{
	reset_shell(shell);
	interavtive_signals();
	input(shell);
	shell->input = readline(shell->cwd);
	if (shell->input && shell->input[0] != '\0')
		add_history(shell->input);
	if (shell->input && validate_input(shell))
		return (free_shell(shell), minishell_loop(shell, envp));
	if (shell->input == NULL)
		return (exit_msg(), free_shell(shell), 0);
	if (!shell->trimmed || shell->trimmed[0] == '\0')
		return (free_shell(shell), minishell_loop(shell, envp));
	if (!ft_strcmp(shell->trimmed, "exit"))
		return (exit_msg(), free_shell(shell), 0);
	lexer(shell, shell->trimmed);
	shell->env_array = env_list_to_array(shell->env);
	shell->root = build_tree(shell, shell->tokens);
	exec_line(shell, shell->root);
	end_process(0);
	free_shell(shell);
	return (minishell_loop(shell, envp));
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
	prompt1 = ft_strjoin("STARSHELL$ ", cwd);
	free(cwd);
	if (!prompt1)
		error_malloc("input", shell);
	prompt2 = ft_strjoin(prompt1, ">");
	free(prompt1);
	if (!prompt2)
		error_malloc("input", shell);
	shell->cwd = prompt2;
}
