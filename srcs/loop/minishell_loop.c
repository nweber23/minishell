/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:48:54 by nweber            #+#    #+#             */
/*   Updated: 2025/09/08 15:13:49 by yyudi            ###   ########.fr       */
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
	shell->env_array = env_list_to_array(shell->env);
	shell->root = build_tree(shell, shell->tokens);
	free_shell(shell);
	end_process(exec_line(shell, shell->root));
	minishell_loop(shell, envp);
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
	char	*temp;
	char	*prompt;
	char	*code;

	code = ft_itoa(exit_code(-1));
	cwd = getcwd(NULL, 0);
	prompt = "LECK_EIER$";
	temp = ft_strjoin(prompt, code);
	free(code);
	if (!temp)
		error_malloc("input", shell);
	temp = ft_strjoin(temp, cwd);
	if (!temp)
		error_malloc("input", shell);
	temp = ft_strjoin(temp, ">");
	if (!temp)
		error_malloc("input", shell);
	shell->cwd = temp;
	free(cwd);
}
