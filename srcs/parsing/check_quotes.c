/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:00:14 by nweber            #+#    #+#             */
/*   Updated: 2025/08/22 17:37:07 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(t_shell_data *shell, char **value, char *str, int i)
{
	if (is_quote(str[i]))
		i = join_quotes(shell, value, str, i);
	else
		i = join_no_quotes(shell, value, str, i);
	return (i);
}

int	join_quotes(t_shell_data *shell, char **value, char *str, int i)
{
	char	*joined;
	char	quote_char;
	int		start;

	quote_char = str[i];
	start = i++;
	while (str[i] && str[i] != quote_char)
		i++;
	joined = ft_substr(str, start + 1, i - start - 1);
	if (!joined)
		error_malloc("join_quotes", shell);
	*value = ft_strjoin(*value, joined);
	free(joined);
	if (!*value)
		error_malloc("join_quotes", shell);
	return (i + 1);
}

int	join_no_quotes(t_shell_data *shell, char **value, char *str, int i)
{
	char	*joined;
	int		start;

	start = i;
	while (str[i] && !is_space(str[i]) && !is_meta(str[i]) && !is_quote(str[i]))
		i++;
	joined = ft_substr(str, start, i - start);
	if (!joined)
		error_malloc("join_no_quotes", shell);
	*value = ft_strjoin(*value, joined);
	free(joined);
	if (!*value)
		error_malloc("join_no_quotes", shell);
	return (i);
}
