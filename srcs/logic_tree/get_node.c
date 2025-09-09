/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:40:21 by nweber            #+#    #+#             */
/*   Updated: 2025/09/09 11:21:59 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_sublist(t_list *new)
{
	t_list	*temp;

	temp = new;
	while (new)
	{
		temp = new->next;
		free(new);
		new = temp;
	}
}

t_list	*create_new_sublist(t_list *tokens)
{
	int		balance;
	t_list	*new;
	t_token	*token;

	token = (t_token *)tokens->content;
	new = NULL;
	balance = 1;
	tokens = tokens->next;
	if (tokens)
		token = (t_token *)tokens->content;
	while (balance != 0 && tokens)
	{
		if (tokens)
		{
			token = (t_token *)tokens->content;
			if (token->type == PARENTHESIS && ft_strcmp(token->value, ")") == 0)
				balance--;
			if (token->type == PARENTHESIS && ft_strcmp(token->value, "(") == 0)
				balance++;
		}
		if (balance != 0)
			ft_lstadd_back(&new, ft_lstnew(token));
		tokens = tokens->next;
	}
	return (new);
}

void	*get_and_node(t_shell_data *shell, void *left_node, t_list *tokens)
{
	t_list	*new;

	new = NULL;
	new = create_new_sublist(tokens->next);
	left_node = create_and(shell, left_node, build_tree_legacy(shell, new));
	clear_sublist(new);
	return (left_node);
}

void	*get_or_node(t_shell_data *shell, void *left_node, t_list *tokens)
{
	t_list	*new;

	new = NULL;
	new = create_new_sublist(tokens->next);
	left_node = create_or(shell, left_node, build_tree_legacy(shell, new));
	clear_sublist(new);
	return (left_node);
}

void	*get_pipe_node(t_shell_data *shell, void *left_node, t_list *tokens)
{
	t_list	*new;

	new = NULL;
	new = create_new_sublist(tokens->next);
	left_node = create_pipe(shell, left_node, build_tree_legacy(shell, new));
	clear_sublist(new);
	return (left_node);
}
