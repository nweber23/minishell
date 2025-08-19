/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 16:18:33 by nweber            #+#    #+#             */
/*   Updated: 2025/08/19 18:24:09 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_type(char *str)
{
	if (ft_strcmp(str, "|"))
		return (PIPE);
	else
		return (WORD);
}

void	set_token_position(t_list *list)
{
	int		i;
	t_token	*token;

	i = 0;
	while (list)
	{
		token = (t_token *)list->content;
		if (token)
			token->position = i;
		list = list->next;
		i++;
	}
}

void	free_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	if (token)
	{
		free(token->value);
		free(token);
	}
}
