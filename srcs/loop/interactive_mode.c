/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 11:18:50 by nweber            #+#    #+#             */
/*   Updated: 2025/09/12 12:59:44 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_interactive(void)
{
	return (isatty(STDIN_FILENO) != 0);
}

char	*read_line_noninteractive(void)
{
	char	*s;
	size_t	n;

	s = get_next_line(STDIN_FILENO);
	if (s == NULL)
		return (NULL);
	if (s[0] != '\0')
	{
		n = ft_strlen(s);
		if (n > 0 && s[n - 1] == '\n')
			s[n - 1] = '\0';
	}
	return (s);
}

void	setup_readline_tty_once(t_shell_data *sh)
{
	extern FILE	*rl_outstream;

	if (!is_interactive())
		return ;
	if (sh->rl_tty != NULL)
		return ;
	sh->rl_tty = NULL;
	rl_outstream = stderr;
}

void	cleanup_readline_tty(t_shell_data *sh)
{
	extern FILE	*rl_outstream;

	if (sh && sh->rl_tty != NULL)
	{
		if (rl_outstream == sh->rl_tty)
			rl_outstream = stdout;
		sh->rl_tty = NULL;
	}
}
