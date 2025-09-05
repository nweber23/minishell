/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:29:50 by nweber            #+#    #+#             */
/*   Updated: 2025/09/05 17:14:22 by nweber           ###   ########.fr       */
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
	while (tokens && ((t_token *)tokens)->type != PIPE)
	{
		if (tokens && (((t_token *)tokens)->type == INFILE \
		|| ((t_token *)tokens)->type == HERE_DOC))
			tokens = tokens->next->next;
		if (tokens && (((t_token *)tokens)->type == OUTFILE \
		|| ((t_token *)tokens)->type == APPEND))
			tokens = tokens->next->next;
		tokens = arg_check(tokens, &count);
		if (token_check(tokens))
			break ;
	}
	return (count);
}

t_list	*word_check(t_list **tokens, char **argv, int *i)
{
	if (*tokens && (((t_token *)*tokens)->state == EXPAND \
	&& ft_strlen(((t_token *)*tokens)->value) == 0))
	{
		*tokens = (*tokens)->next;
		return (*tokens);
	}
	argv[*i] = ((t_token *)tokens)->value;
	*tokens = (*tokens)->next;
	(*i)++;
	return (*tokens);
}

t_list	*arg_check(t_list *tokens, int *count)
{
	if (tokens && ((t_token *)tokens)->type == WORD)
	{
		if (((t_token *)tokens)->state == EXPAND \
		&& ft_strlen(((t_token *)tokens)->value) == 0)
			return (tokens->next);
		(*count)++;
	}
	return (tokens->next);
}
