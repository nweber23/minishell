/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:48:54 by nweber            #+#    #+#             */
/*   Updated: 2025/09/14 13:00:39 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_user_input(t_shell_data *shell)
{
	if (is_interactive())
	{
		setup_readline_tty_once(shell);
		set_readline_active(1);
		shell->input = readline(shell->cwd);
		set_readline_active(0);
		if (shell->input && shell->input[0] != '\0')
			add_history(shell->input);
	}
	else
		shell->input = read_line_noninteractive();
}

static int	handle_validate_or_eof(t_shell_data *shell, int *code, int *recurse)
{
	if (shell->input && validate_input(shell))
	{
		free_shell(shell);
		*recurse = 1;
		return (1);
	}
	if (shell->input == NULL)
	{
		*code = exit_code(-1);
		if (is_interactive())
			exit_msg();
		free_shell(shell);
		return (1);
	}
	return (0);
}

static int	handle_empty_or_exit(t_shell_data *shell, int *code, int *recurse)
{
	if (!shell->trimmed || shell->trimmed[0] == '\0')
	{
		free_shell(shell);
		*recurse = 1;
		return (1);
	}
	if (!ft_strcmp(shell->trimmed, "exit"))
	{
		if (is_interactive())
			exit_msg();
		free_shell(shell);
		*code = exit_code(-1);
		return (1);
	}
	return (0);
}

static void	prepare_exec(t_shell_data *shell)
{
	lexer(shell, shell->trimmed);
	if (shell->root)
	{
		free_tree(shell->root);
		shell->root = NULL;
	}
	shell->root = build_tree(shell, shell->tokens);
}

int	minishell_loop(t_shell_data *shell, char **envp)
{
	int	code;
	int	recurse;

	init_iteration(shell);
	read_user_input(shell);
	code = 0;
	recurse = 0;
	if (handle_validate_or_eof(shell, &code, &recurse))
	{
		if (recurse)
			return (minishell_loop(shell, envp));
		return (code);
	}
	recurse = 0;
	if (handle_empty_or_exit(shell, &code, &recurse))
	{
		if (recurse)
			return (minishell_loop(shell, envp));
		return (code);
	}
	prepare_exec(shell);
	code = exec_line(shell, shell->root);
	exit_code(code);
	free_shell(shell);
	return (minishell_loop(shell, envp));
}
