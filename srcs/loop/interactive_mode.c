/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 11:18:50 by nweber            #+#    #+#             */
/*   Updated: 2025/09/12 11:20:17 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_interactive(void)
{
	return (isatty(STDIN_FILENO) != 0);
}

char	*read_line_noninteractive(void)
{
	char	buffer[4096];
	char	*s;
	size_t	n;

	if (fgets(buffer, sizeof(buffer), stdin) == NULL)
		return (NULL);
	s = ft_strdup(buffer);
	if (!s)
		return (NULL);
	if (s[0] != '\0')
	{
		n = ft_strlen(s);
		if (n > 0 && s[n - 1] == '\n')
			s[n - 1] = '\0';
	}
	return (s);
}

void	setup_readline_tty_once(void)
{
	extern FILE	*rl_outstream;
	static int	done = 0;
	static FILE	*tty = NULL;

	if (done)
		return ;
	done = 1;
	if (is_interactive())
	{
		tty = fopen("/dev/tty", "w");
		if (tty)
			rl_outstream = tty;
		else
			rl_outstream = stderr;
	}
}
