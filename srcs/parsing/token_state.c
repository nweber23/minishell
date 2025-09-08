/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:08:44 by nweber            #+#    #+#             */
/*   Updated: 2025/09/08 19:33:21 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*alloc_token_or_exit(char *value, t_shell_data *shell)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(value);
		error_malloc("words_handling", shell);
	}
	return (token);
}

static int	advance_word_end(t_shell_data *shell, char *str,
	int i, char **value)
{
	while (str[i] && !is_space(str[i]) && !is_meta(str[i]))
		i = check_quotes(shell, value, str, i);
	return (i);
}

static bool	scan_for_expand(char *str, int start, int end)
{
	bool	double_q;
	bool	single_q;
	int		j;

	double_q = false;
	single_q = false;
	j = start;
	while (j < end)
	{
		quote_toggling(str[j], &double_q, &single_q);
		if (str[j] == '$' && !single_q && (str[j + 1] == '?'
				|| ft_isalnum((int)str[j + 1])))
			return (true);
		j++;
	}
	return (false);
}

static int	append_token_and_skip(t_shell_data *shell, t_token *token,
	char *str, int i)
{
	token->type = WORD;
	ft_lstadd_back(&shell->tokens, ft_lstnew(token));
	while (is_space(str[i]))
		i++;
	return (i);
}

int	words_handling(t_shell_data *shell, char *str, int i)
{
	t_token	*token;
	char	*value;
	int		start;
	int		end;

	value = get_empty_value(shell);
	token = alloc_token_or_exit(value, shell);
	start = i;
	end = advance_word_end(shell, str, i, &value);
	token->value = value;
	token->state = detect_state(str, start, end);
	i = append_token_and_skip(shell, token, str, end);
	return (i);
}
