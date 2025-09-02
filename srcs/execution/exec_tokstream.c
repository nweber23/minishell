/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tokstream.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:37 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 09:40:10 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_token *peek(t_tokarr *ta)
{
	if (ta->i >= ta->n)
		return (NULL);
	return (ta->v[ta->i]);
}

t_token *next(t_tokarr *ta)
{
	t_token *token;

	token = peek(ta);
	if (token)
		ta->i++;
	return (token);
}

int is_cmd_end(t_token *token)
{
	if (!token)
		return (1);
	if (token->type == PIPE || token->type == AND || token->type == OR)
		return (1);
	if (is_rparen(token))
		return (1);
	return (0);
}
