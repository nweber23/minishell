/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_andor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:17 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:18 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

t_node	*parse_and_or(t_shell_data *sh, t_tokarr *ta)
{
	t_node	*left;
	t_node	*logic;
	t_token	*t;

	left = parse_pipeline(sh, ta);
	if (!left)
		return (NULL);
	while (1)
	{
		t = peek(ta);
		if (!t || (t->type != AND && t->type != OR))
			break ;
		logic = nd_new(t->type == AND ? ND_AND : ND_OR);
		if (!logic)
			return (left);
		next(ta);
		logic->left = left;
		logic->right = parse_pipeline(sh, ta);
		left = logic;
	}
	return (left);
}
