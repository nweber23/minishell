/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:37:19 by nweber            #+#    #+#             */
/*   Updated: 2025/09/07 16:11:56 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell_data	*global_shell(t_shell_data *shell, int check)
{
	static t_shell_data	*global = NULL;

	if (check)
		return (global);
	global = shell;
	return (global);
}

void	break_heredoc(int sig)
{
	const t_shell_data	*shell = global_shell(NULL, 1);

	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		close(shell->fd);
		free_env(shell->env);
		free_shell((t_shell_data *)shell);
		exit_code(130);
		exit(130);
	}
}

void	trap_heredoc(void)
{
	signal(SIGINT, break_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
