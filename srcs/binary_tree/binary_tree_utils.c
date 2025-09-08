/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:29:50 by nweber            #+#    #+#             */
/*   Updated: 2025/09/07 17:14:50 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*remove_direct(t_list *temp)
{
	while (temp && ((t_token *)temp->content)->type != PIPE)
	{
		temp = temp->next;
		if (!token_check(temp))
			break ;
	}
	return (temp);
}

t_list	*remove_goto_next(t_list *temp)
{
	temp = temp->next;
	while (temp && ((t_token *)temp->content)->type != PIPE)
	{
		temp = temp->next;
		if (!token_check(temp))
			break ;
	}
	return (temp);
}

int	count_args(t_list *tokens)
{
	int	count;

	count = 0;
	while (tokens && ((t_token *)tokens->content)->type != PIPE)
	{
		if (tokens && (((t_token *)tokens->content)->type == INFILE \
		|| ((t_token *)tokens->content)->type == HERE_DOC))
			tokens = tokens->next->next;
		if (tokens && (((t_token *)tokens->content)->type == OUTFILE \
		|| ((t_token *)tokens->content)->type == APPEND))
			tokens = tokens->next->next;
		tokens = arg_check(tokens, &count);
		if (token_check(tokens))
			break ;
	}
	return (count);
}

t_list	*word_check(t_list **tokens, char **argv, int *i)
{
	t_token	*tok;

	if (!tokens || !*tokens)
		return (NULL);
	tok = (t_token *)(*tokens)->content;
	if (tok && tok->state == EXPAND && ft_strlen(tok->value) == 0)
		return ((*tokens)->next);
	argv[*i] = tok->value;
	*tokens = (*tokens)->next;
	(*i)++;
	return (*tokens);
}

t_list	*arg_check(t_list *tokens, int *count)
{
	if (tokens && ((t_token *)tokens->content)->type == WORD)
	{
		if (((t_token *)tokens->content)->state == EXPAND \
		&& ft_strlen(((t_token *)tokens->content)->value) == 0)
			return (tokens->next);
		(*count)++;
	}
	return (tokens->next);
}
