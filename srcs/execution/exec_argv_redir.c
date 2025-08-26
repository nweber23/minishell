/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_argv_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:00:59 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:00 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

int	append_word(char ***argv, int *argc, char *w)
{
	char	**newv;
	int		i;

	newv = (char **)malloc(sizeof(char *) * (*argc + 2));
	if (!newv)
		return (0);
	i = 0;
	while (i < *argc)
	{
		newv[i] = (*argv)[i];
		i++;
	}
	newv[i] = w;
	newv[i + 1] = NULL;
	free(*argv);
	*argv = newv;
	*argc = *argc + 1;
	return (1);
}

int	add_redir(t_redir **lst, t_redir *node)
{
	t_redir	*p;

	if (!node)
		return (0);
	if (!*lst)
	{
		*lst = node;
		return (1);
	}
	p = *lst;
	while (p->next)
		p = p->next;
	p->next = node;
	return (1);
}

t_rdrtype	map_rdr(t_token_type tt)
{
	if (tt == INFILE)
		return (R_IN);
	if (tt == OUTFILE)
		return (R_OUT_TRUNC);
	if (tt == APPEND)
		return (R_OUT_APPEND);
	return (R_HEREDOC);
}
