/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:00:14 by nweber            #+#    #+#             */
/*   Updated: 2025/08/20 11:12:06 by nweber           ###   ########.fr       */
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
}

int	join_no_quotes(t_shell_data *shell, char **value, char *str, int i)
{
}