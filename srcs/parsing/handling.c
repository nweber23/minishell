/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:53:56 by nweber            #+#    #+#             */
/*   Updated: 2025/08/22 14:31:35 by nweber           ###   ########.fr       */
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
		error_malloc("or_handling", shell);
	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(value);
		error_malloc("or_handling", shell);
	}
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
		error_malloc("and_handling", shell);
	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(value);
		error_malloc("and_handling", shell);
	}
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
		error_malloc("pipe_handling", shell);
	token->value = &str[i];
	token->type = PIPE;
	token->state = GENERAL;
	ft_lstadd_back(&shell->tokens, ft_lstnew(token));
	while (is_space(str[i]))
		i++;
	return (i);
}

int	redirect_handling(t_shell_data *shell, char *str, int i)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		error_malloc("redirect_handling", shell);
	if (str[i] == '>' && str[i + 1] == '>')
		i = append(shell, token, str, i);
	else if (str[i] == '<' && str[i + 1] == '<')
		i = heredoc(shell, token, str, i);
	else
		i = simple(shell, token, str, i);
	token->state = GENERAL;
	ft_lstadd_back(&shell->tokens, ft_lstnew(token));
	while (is_space(str[i]))
		i++;
	return (i);
}

int	words_handling(t_shell_data *shell, char *str, int i)
{
	t_token	*token;
	char	*value;

	token = malloc(sizeof(t_token));
	if (!token)
		error_malloc("words_handling", shell);
	while (str[i] && !is_space(str[i]) && !is_meta(str[i]))
		i = check_quotes(shell, &value, str, i);
	token->value = value;
	if (!token->value)
		error_malloc("words_handling", shell);
	token->type = WORD;
	token->state = GENERAL;
	ft_lstadd_back(&shell->tokens, ft_lstnew(token));
	while (is_space(str[i]))
		i++;
	return (i);
}
