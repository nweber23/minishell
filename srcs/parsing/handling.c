/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:53:56 by nweber            #+#    #+#             */
/*   Updated: 2025/08/19 18:54:38 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	or_handling(t_shell_data *shell, char *str, int i)
{
	t_token	*token;
	char	*value;

	value = ft_strdup("||");
	i += 2;
	if (!value)
		return (error_malloc("or_handling"));
	token = malloc(sizeof(t_token));
	if (!token)
		return (free(value), error_malloc("or_handling"));
	token->value = value;
	token->state = GENERAL;
	token->type = OR;
	ft_lstadd_back(&shell->tokens, ft_lstnew(token));
	while (is_space(str[i]))
		i++;
	return (i);
}

int	and_handling(t_shell_data *shell, char *str, int i)
{
	t_token	*token;
	char	*value;

	value = ft_strdup("&&");
	i += 2;
	if (!value)
		return (error_malloc("and_handling"));
	token = malloc(sizeof(t_token));
	if (!token)
		return (free(value), error_malloc("and_handling"));
	token->value = value;
	token->state = GENERAL;
	token->type = AND;
	ft_lstadd_back(&shell->tokens, ft_lstnew(token));
	while (is_space(str[i]))
		i++;
	return (i);
}

int	pipe_handling(t_shell_data *shell, char *str, int i)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		error_malloc("pipe_handling");
}