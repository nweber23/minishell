/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_outfile.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 10:34:40 by nweber            #+#    #+#             */
/*   Updated: 2025/09/08 10:38:24 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_outfile_token(t_list *tokens)
{
	if (!tokens)
		return (0);
	if (((t_token *)tokens->content)->type == APPEND)
		return (1);
	if (((t_token *)tokens->content)->type == OUTFILE)
		return (1);
	return (0);
}

static t_outfile	*create_outfile(t_shell_data *shell, t_list *tokens)
{
	t_outfile	*outf;

	outf = (t_outfile *)malloc(sizeof(t_outfile));
	if (!outf)
		error_malloc("get_outfiles", shell);
	outf->name = NULL;
	if (((t_token *)tokens->content)->type == OUTFILE)
		outf->type = ADD;
	else
		outf->type = APP;
	if (tokens->next && ((t_token *)tokens->next->content)->type == WORD)
	{
		outf->name = ft_strdup(((t_token *)tokens->next->content)->value);
		if (!outf->name)
			error_malloc ("create_outfile", shell);
	}
	return (outf);
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

t_list	*get_outfiles(t_shell_data *shell, t_list *tokens, t_list **outfile)
{
	t_outfile	*outf;

	outf = NULL;
	while (tokens && ((t_token *)tokens->content)->type != PIPE)
	{
		if (is_outfile_token(tokens))
		{
			outf = create_outfile(shell, tokens);
			ft_lstadd_back(outfile, ft_lstnew(outf));
			tokens = advance_after_file(tokens);
			continue ;
		}
		tokens = tokens->next;
		if (token_check(tokens))
			break ;
	}
	return (tokens);
}
