/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:15:08 by nweber            #+#    #+#             */
/*   Updated: 2025/08/29 09:28:02 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	validate_input(t_shell_data *shell)
{
	if (!shell->input || !shell->input[0])
		return (false);
	shell->trimmed = ft_strtrim(shell->input, "\t \n\r");
	if (!shell->trimmed)
		error_malloc("validate_input", shell);
	if (!shell->trimmed[0])
		return (false);
	if (!pipe_check(shell->trimmed))
		return (error_message(PIPE_MSG), exit_code(2), true);
	if (!quote_check(shell->trimmed))
		return (error_message(QUOTE_MSG), exit_code(2), true);
	if (!quote_position_check(shell->trimmed))
		return (error_message(SYNTAX_MSG), exit_code(2), true);
	if (!redirect_check(shell->trimmed))
		return (exit_code(2), true);
	if (!parenthesis_check(shell->trimmed))
		return (exit_code(2), true);
	return (false);
}

bool	error_redirect(char *str, int *i, int len)
{
	char	*error_msg;
	size_t	slen;
	int		pos;

	if (!str || !i)
		return (false);
	slen = ft_strlen(str);
	pos = *i;
	if ((size_t)pos < slen)
	{
		size_t	peek = (size_t)pos + (size_t)len;
		if (peek < slen && str[peek] == '|')
			return (error_message(PIPE_MSG), false);
	}
	*i += len;
	while ((size_t)(*i) < slen && is_space(str[*i]))
		(*i)++;
	if ((size_t)(*i) >= slen)
		return (error_message(REDIRECT_MSG), false);
	if (is_meta(str[*i]))
	{
		if (str[*i] == '>' && str[*i + 1] && str[*i + 1] == '>')
			error_msg = "syntax error near unexpected token `>>'";
		else if (str[*i] == '<' && str[*i + 1] && str[*i + 1] == '<')
			error_msg = "syntax error near unexpected token `<<'";
		else if (str[*i] == '<')
			error_msg = "syntax error near unexpected token `<'";
		else if (str[*i] == '>')
			error_msg = "syntax error near unexpected token `>'";
		else
			error_msg = "syntax error near unexpected token `newline'";
		return (error_message(error_msg), false);
	}
	return (true);
}
