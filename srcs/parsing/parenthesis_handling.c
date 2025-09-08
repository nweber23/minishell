/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_handling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:19:53 by nweber            #+#    #+#             */
/*   Updated: 2025/08/22 16:18:04 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parenthesis_balancing(char *str, int i)
{
	int	balance;

	balance = 1;
	while (str[i] && balance != 0)
	{
		i++;
		if (str[i] == '(')
			balance++;
		if (str[i] == ')')
			balance--;
	}
	return (i);
}

static bool	sub_checker(char *sub)
{
	char	*token;
	int		i;
	int		j;

	token = "|&";
	j = 0;
	i = 0;
	if (!sub || i < 0)
		return (false);
	while (sub[i])
	{
		j = 0;
		while (token[j])
		{
			if (sub[i] == token[j])
				return (true);
			j++;
		}
		i++;
	}
	return (false);
}

static int	closing(t_shell_data *shell, char *str, int i, bool valid)
{
	t_token	*token;
	char	*value;

	if (str[i] == ')' && valid)
	{
		value = ft_strdup(")");
		if (!value)
			error_malloc("closing", shell);
		i++;
		token = malloc(sizeof(t_token));
		if (!token)
		{
			free(value);
			error_malloc("closing", shell);
		}
		token->value = value;
		token->type = PARENTHESIS;
		token->state = GENERAL;
		ft_lstadd_back(&shell->tokens, ft_lstnew(token));
		while (is_space(str[i]))
			i++;
		return (i);
	}
	else
		return (i = remove_spaces(str, i), i + 1);
}

static int	opening(t_shell_data *shell, char *str, int i, bool valid)
{
	t_token	*token;
	char	*value;

	if (valid)
	{
		if (str[i] == '(')
			value = ft_strdup("(");
		if (str[i] == ')')
			value = ft_strdup(")");
		if (!value)
			error_malloc("opening", shell);
		i++;
		token = malloc(sizeof(t_token));
		if (!token)
		{
			free(value);
			error_malloc("opening", shell);
		}
		token->type = PARENTHESIS;
		ft_lstadd_back(&shell->tokens, ft_lstnew(token));
		while (is_space(str[i]))
			i++;
		return (token->value = value, token->state = GENERAL, i);
	}
	return (-1);
}

int	parenthesis_handling(t_shell_data *shell, char *str, int i)
{
	int			start;
	char		*sub;
	static bool	valid = false;

	if (str[i] == ')')
	{
		i = closing(shell, str, i, valid);
		if (i != -1)
			return (i);
	}
	start = i;
	i = parenthesis_balancing(str, i);
	sub = ft_substr(str, start + 1, i - start - 1);
	if (!sub)
		error_malloc("parenthesis_handling", shell);
	valid = sub_checker(sub);
	free(sub);
	i = opening(shell, str, start, valid);
	if (i != -1)
		return (i);
	if (str[start] && str[start] == '(')
		start++;
	while (is_space(str[start]))
		start++;
	return (start);
}
