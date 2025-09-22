/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:00:14 by nweber            #+#    #+#             */
/*   Updated: 2025/09/22 11:20:34 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_quoted_content(t_shell_data *shell, char *str,
		int start_index, int *end_index)
{
	char	quote_char;
	int		i;
	char	*joined;

	quote_char = str[start_index];
	i = start_index + 1;
	while (str[i] && str[i] != quote_char)
		i++;
	if (i == start_index + 1)
		joined = ft_strdup("\x1D");
	else
		joined = ft_substr(str, start_index + 1, i - start_index - 1);
	if (!joined)
		error_malloc("join_quotes", shell);
	*end_index = i;
	return (joined);
}

static void	mask_single_quote_dollars(char *s)
{
	int	k;

	k = 0;
	while (s && s[k])
	{
		if (s[k] == '$')
			s[k] = (char)0x1F;
		k++;
	}
}

static void	append_and_free(t_shell_data *sh, char **val, char *joined)
{
	char	*old;
	char	*tmp;

	old = *val;
	if (old && *old)
	{
		tmp = ft_strjoin(old, "\x1E");
		if (!tmp)
			error_malloc("join_quotes", sh);
		free(old);
		old = tmp;
	}
	*val = ft_strjoin(old, joined);
	free(joined);
	free(old);
	if (!*val)
		error_malloc("join_quotes", sh);
}

int	join_quotes(t_shell_data *shell, char **value, char *str, int i)
{
	char	quote_char;
	char	*joined;
	int		end;

	quote_char = str[i];
	joined = extract_quoted_content(shell, str, i, &end);
	if (quote_char == '\'')
		mask_single_quote_dollars(joined);
	append_and_free(shell, value, joined);
	return (end + 1);
}

int	join_no_quotes(t_shell_data *shell, char **value, char *str, int i)
{
	char	*joined;
	int		start;
	char	*old;

	start = i;
	while (str[i] && !is_space(str[i]) && !is_meta(str[i]) && !is_quote(str[i]))
		i++;
	joined = ft_substr(str, start, i - start);
	if (!joined)
		error_malloc("join_no_quotes", shell);
	old = *value;
	*value = ft_strjoin(old, joined);
	free(joined);
	free(old);
	if (!*value)
		error_malloc("join_no_quotes", shell);
	return (i);
}
