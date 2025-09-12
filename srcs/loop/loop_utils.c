/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 19:58:03 by nweber            #+#    #+#             */
/*   Updated: 2025/09/12 11:32:42 by nweber           ###   ########.fr       */
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
	shell->tokens = NULL;
	shell->trimmed = NULL;
}

int	end_process(int value)
{
	if (value == -1)
		return (exit_code(-1));
	return (exit_code(value));
}

void	input(t_shell_data *shell)
{
	char	*cwd;
	char	*prompt1;
	char	*prompt2;

	if (!isatty(STDIN_FILENO))
	{
		shell->cwd = NULL;
		return ;
	}
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

void	init_iteration(t_shell_data *shell)
{
	reset_shell(shell);
	global_shell(shell, 0);
	if (is_interactive())
		interavtive_signals();
	else
		init_signals();
	input(shell);
}
