/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 20:39:42 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/12 10:24:07 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	is_redir_tok(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == INFILE || token->type == OUTFILE)
		return (1);
	if (token->type == APPEND || token->type == HERE_DOC)
		return (1);
	return (0);
}

int	argv_len(char **vector)
{
	int	length;

	length = 0;
	if (!vector)
		return (0);
	while (vector[length])
		length++;
	return (length);
}

void	mask_quoted_stars(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		if (s[i] == '*')
			s[i] = '\a';
		i++;
	}
}

char	*expand_token_value(t_shell_data *sh, t_token *tok)
{
	char	*dup;
	char	*expanded;

	dup = ft_strdup(tok->value);
	if (!dup)
		return (NULL);
	if (tok->state == SINGLE_Q)
		return (dup);
	expanded = expand_line_env(sh, dup);
	if (!expanded)
		return (dup);
	free(dup);
	return (expanded);
}

int	append_word_simple(char ***argv, char *word_copy)
{
	char	**new_vector;
	int		count;
	int		index;

	count = argv_len(*argv);
	new_vector = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_vector)
		return (0);
	index = 0;
	while (index < count)
	{
		new_vector[index] = (*argv)[index];
		index++;
	}
	new_vector[index] = word_copy;
	new_vector[index + 1] = NULL;
	if (*argv)
		free(*argv);
	*argv = new_vector;
	return (1);
}
