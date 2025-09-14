/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tokstream.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:37 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 21:01:25 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	wait_status(pid_t pid)
{
	int	status;
	int	rc;

	status = 0;
	while (1)
	{
		rc = waitpid(pid, &status, 0);
		if (rc == -1)
		{
			if (errno == EINTR)
				continue ;
			return (1);
		}
		break ;
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

t_token	*peek(t_tokarr *ta)
{
	if (ta->i >= ta->n)
		return (NULL);
	return (ta->v[ta->i]);
}

t_token	*next(t_tokarr *ta)
{
	t_token	*token;

	token = peek(ta);
	if (token)
		ta->i++;
	return (token);
}

int	is_cmd_end(t_token *token)
{
	if (!token)
		return (1);
	if (token->type == PIPE || token->type == AND || token->type == OR)
		return (1);
	if (is_rparen(token))
		return (1);
	return (0);
}
