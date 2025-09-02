/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_argv_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:00:59 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 09:31:29 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int append_word(char ***argv, int *argc, char *word)
{
	char	**new_vector;
	int	 index;

	new_vector = (char **)malloc(sizeof(char *) * (*argc + 2));
	if (!new_vector)
		return (0);
	index = 0;
	while (index < *argc)
	{
		new_vector[index] = (*argv)[index];
		index++;
	}
	new_vector[index] = word;
	new_vector[index + 1] = NULL;
	free(*argv);
	*argv = new_vector;
	*argc = *argc + 1;
	return (1);
}

int add_redir(t_redir **list_head, t_redir *new_node)
{
	t_redir *tail;

	if (!new_node)
		return (0);
	if (!*list_head)
	{
		*list_head = new_node;
		return (1);
	}
	tail = *list_head;
	while (tail->next)
		tail = tail->next;
	tail->next = new_node;
	return (1);
}

t_rdrtype map_rdr(t_token_type token_type)
{
	if (token_type == INFILE)
		return (R_IN);
	if (token_type == OUTFILE)
		return (R_OUT_TRUNC);
	if (token_type == APPEND)
		return (R_OUT_APPEND);
	return (R_HEREDOC);
}
