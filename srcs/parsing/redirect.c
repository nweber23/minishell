/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:06:01 by nweber            #+#    #+#             */
/*   Updated: 2025/08/21 14:39:36 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append(t_shell_data *shell, t_token *token, char *str, int i)
{
	token->value = ft_strndup(&str[i], 2);
	if (!token->value)
		error_malloc("append", shell);
	token->type = APPEND;
	return (i + 2);
}

int	heredoc(t_shell_data *shell, t_token *token, char *str, int i)
{
	token->value = ft_strndup(&str[i], 2);
	if (!token->value)
		error_malloc("heredoc", shell);
	token->type = HERE_DOC;
	return (i + 2);
}

int	simple(t_shell_data *shell, t_token *token, char *str, int i)
{
	token->value = ft_strndup(&str[i], 1);
	if (!token->value)
		error_malloc("simple", shell);
	if (str[i] == '<')
		token->type = INFILE;
	else
		token->type = OUTFILE;
	return (i + 1);
}
