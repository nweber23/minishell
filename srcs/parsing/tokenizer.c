/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:37:02 by nweber            #+#    #+#             */
/*   Updated: 2025/09/08 19:33:05 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_token(t_shell_data *shell, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i = remove_spaces(str, i);
		if (str[i] == '(' || str[i] == ')')
			i = parenthesis_handling(shell, str, i);
		else if (str[i] == '&' && str[i + 1] == '&')
			i = and_handling(shell, str, i);
		else if (str[i] == '|' && str[i + 1] == '|')
			i = or_handling(shell, str, i);
		else if (str[i] == '|')
			i = pipe_handling(shell, str, i);
		else if (str[i] == '<' || str[i] == '>')
			i = redirect_handling(shell, str, i);
		else
			i = words_handling(shell, str, i);
		set_token_position(shell->tokens);
		i = remove_spaces(str, i);
	}
}

void	lexer(t_shell_data *shell, char *input)
{
	shell->tokens = NULL;
	create_token(shell, input);
}

t_token_state	detect_state(char *str, int start, int end)
{
	bool	started_single;
	bool	started_double;
	bool	expand;

	started_single = (str[start] == '\'');
	started_double = (str[start] == '"');
	expand = scan_for_expand(str, start, end);
	if (started_single && !expand)
		return (SINGLE_Q);
	else if (started_double && !expand)
		return (DOUBLE_Q);
	else if (expand)
		return (EXPAND);
	return (GENERAL);
}
