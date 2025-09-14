/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 18:02:07 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/12 18:15:17 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	starts_command(t_token *t)
{
	if (!t)
		return (0);
	if (t->type == WORD)
		return (1);
	if (is_lparen(t))
		return (1);
	if (t->type == HERE_DOC || t->type == INFILE
		|| t->type == OUTFILE || t->type == APPEND)
		return (1);
	return (0);
}

t_node	*pipeline_syntax_eof(void)
{
	exit_code(2);
	error_message(SYNTAX_MSG);
	return (NULL);
}

t_node	*pipeline_syntax_err(void)
{
	exit_code(2);
	error_message(SYNTAX_ERROR);
	return (NULL);
}

t_node	*parse_factor(t_shell_data *sh, t_tokarr *ta)
{
	t_token	*t;

	(void)sh;
	t = peek(ta);
	if (is_lparen(t))
		return (parse_group(sh, ta));
	return (parse_command(sh, ta));
}

int	right_is_redir_only(t_node *right)
{
	if (!right || right->type != ND_EXEC || !right->cmd)
		return (0);
	if (right->cmd->redirs == NULL)
		return (0);
	if (right->cmd->argv == NULL || right->cmd->argv[0] == NULL)
		return (1);
	return (0);
}
