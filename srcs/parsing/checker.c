/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:53:06 by nweber            #+#    #+#             */
/*   Updated: 2025/08/20 11:05:52 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quote(char c)
{
	if (c == '"' || c == '\'')
		return (true);
	return (false);
}

bool	is_space(char c)
{
	if (c == '\t' || c == ' ' || c == '\r')
		return (true);
	return (false);
}

bool	is_meta(char c)
{
	char	*charset;
	int		i;

	i = 0;
	charset = "<>|()";
	while (charset[i])
	{
		if (charset[i] == c)
			return (true);
		i++;
	}
	return (false);
}

bool	is_expandable(char	*token)
{
	int		i;
	bool	quotes;

	i = 0;
	quotes = false;
	while (is_space(token[i]))
		i++;
	if (token[i] == '\0')
		return (false);
	while (token[i])
	{
		if (is_quote(token[i]))
		{
			quotes = !quotes;
			i++;
			continue ;
		}
		if (token[i] == '$' && (token[i + 1] == '?' || \
			ft_isalnum(token[i + 1])))
			return (true);
		if (!quotes && (is_space(token[i]) || is_meta(token[i])))
			return (false);
		i++;
	}
	return (false);
}
