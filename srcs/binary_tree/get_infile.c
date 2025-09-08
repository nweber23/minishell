/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_infile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 10:34:27 by nweber            #+#    #+#             */
/*   Updated: 2025/09/08 10:43:36 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_infile_token(t_list *tokens)
{
	if (!tokens)
		return (0);
	if (((t_token *)tokens->content)->type == HERE_DOC)
		return (1);
	if (((t_token *)tokens->content)->type == INFILE)
		return (1);
	return (0);
}

static void	fill_infile_fields(t_shell_data *shell, t_infile *inf,
	t_list *tokens)
{
	if (((t_token *)tokens->content)->type == INFILE)
	{
		inf->type = INF;
		if (tokens->next && ((t_token *)tokens->next->content)->type == WORD)
		{
			inf->name = ft_strdup(((t_token *)tokens->next->content)->value);
			if (!inf->name)
				error_malloc("create_infile", shell);
		}
	}
	else
	{
		inf->type = HERE;
		if (tokens->next && ((t_token *)tokens->next->content)->type == WORD)
		{
			inf->eof = ft_strdup(((t_token *)tokens->next->content)->value);
			if (!inf->eof)
				error_malloc("create_infile", shell);
		}
	}
}

static t_infile	*create_infile(t_shell_data *shell, t_list *tokens)
{
	t_infile	*inf;

	inf = (t_infile *)malloc(sizeof(t_infile));
	if (!inf)
		error_malloc("get_infiles", shell);
	inf->eof = NULL;
	inf->name = NULL;
	fill_infile_fields(shell, inf, tokens);
	return (inf);
}

static t_list	*advance_after_file(t_list *tokens)
{
	if (!tokens)
		return (NULL);
	if (tokens->next)
		tokens = tokens->next->next;
	else
		tokens = NULL;
	return (tokens);
}

t_list	*get_infiles(t_shell_data *shell, t_list *tokens, t_list **infile)
{
	t_infile	*inf;

	inf = NULL;
	while (tokens && ((t_token *)tokens->content)->type != PIPE)
	{
		if (is_infile_token(tokens))
		{
			inf = create_infile(shell, tokens);
			ft_lstadd_back(infile, ft_lstnew(inf));
			tokens = advance_after_file(tokens);
			continue ;
		}
		tokens = tokens->next;
		if (token_check(tokens))
			break ;
	}
	return (tokens);
}
