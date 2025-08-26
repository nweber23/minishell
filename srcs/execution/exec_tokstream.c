/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tokstream.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:37 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:38 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

t_token	*peek(t_tokarr *ta)
{
	if (ta->i >= ta->n)
		return (NULL);
	return (ta->v[ta->i]);
}

t_token	*next(t_tokarr *ta)
{
	t_token	*t;

	t = peek(ta);
	if (t)
		ta->i++;
	return (t);
}

int	is_cmd_end(t_token *t)
{
	if (!t)
		return (1);
	if (t->type == PIPE || t->type == AND || t->type == OR)
		return (1);
	if (is_rparen(t))
		return (1);
	return (0);
}
