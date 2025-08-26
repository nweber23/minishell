/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_group.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:22 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:23 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

t_node	*parse_group(t_shell_data *sh, t_tokarr *ta)
{
	t_node	*n;
	t_token	*t;

	(void)sh;
	t = peek(ta);
	if (!is_lparen(t))
		return (NULL);
	next(ta);
	n = nd_new(ND_GROUP);
	if (!n)
		return (NULL);
	n->left = parse_and_or(sh, ta);
	t = peek(ta);
	if (!is_rparen(t))
		return (NULL);
	next(ta);
	return (n);
}
