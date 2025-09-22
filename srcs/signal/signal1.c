/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:37:19 by nweber            #+#    #+#             */
/*   Updated: 2025/09/22 11:00:16 by yyudi            ###   ########.fr       */
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
	t_shell_data	*sh;

	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		sh = global_shell(NULL, 1);
		if (sh)
			sh->exit_code = 130;
		if (sh && sh->fd > 0)
			close(sh->fd);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	trap_heredoc(void)
{
	signal(SIGINT, break_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	ignore_pipe(int sig)
{
	if (sig == SIGPIPE)
		return ;
}
