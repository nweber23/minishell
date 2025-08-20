/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:37:02 by nweber            #+#    #+#             */
/*   Updated: 2025/08/20 10:09:28 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_token(t_shell_data *shell, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '(' || str[i] == ')')
			i = parenthesis_handling(shell, str, i);
		else if (str[i] == '|')
			i = pipe_handling(shell, str, i);
		else if (str[i] == '<' || str[i] == '>')
			i = redirect_handling(shell, str, i);
		else if (str[i] == '&' && str[i + 1] == '&')
			i = and_handling(shell, str, i);
		else if (str[i] == '|' && str[i + 1] == '|')
			i = or_handling(shell, str, i);
		else
			i = words_handling(shell, str, i);
		set_token_position(shell->tokens);
	}
}
