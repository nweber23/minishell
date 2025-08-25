/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:15:08 by nweber            #+#    #+#             */
/*   Updated: 2025/08/25 16:52:20 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	validate_input(t_shell_data *shell)
{
	if (!shell->input[0])
		return (true);
	shell->trimmed = ft_strtrim(shell->input, "\t ");
	if (!shell->trimmed)
		error_malloc("validate_input", shell);
	if (!shell->trimmed[0])
		return (true);
	if (!pipe_check(shell->trimmed))
		return (error_message(PIPE_MSG), exit_code(2));
	if (!quote_check(shell->trimmed))
		return (error_message(QUOTE_MSG), exit_code(2));
	if (!quote_position_check(shell->trimmed))
		return (error_message(SYNTAX_MSG), exit_code(2));
	if (!redirect_check(shell->trimmed))
		return (exit_code(2));
	if (!parenthesis_check(shell->trimmed))
		return (exit_code(2));
	return (false);
}