/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 16:18:33 by nweber            #+#    #+#             */
/*   Updated: 2025/08/24 20:07:36 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_type(char *str)
{
	if (ft_strcmp(str, "|") == 0)
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

int	remove_spaces(char *str, int i)
{
	while (is_space(str[i]))
		i++;
	return (i);
}

int	remove_quotes(char *str, int i)
{
	if (str[i] && is_quote(str[i]))
	{
		i++;
		while (str[i] && !is_quote(str[i]))
			i++;
	}
	return (i);
}
