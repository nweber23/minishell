/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:23 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:24 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

t_node	*parse_factor(t_shell_data *sh, t_tokarr *ta)
{
	t_token	*t;

	t = peek(ta);
	if (is_lparen(t))
		return (parse_group(sh, ta));
	return (parse_command(sh, ta));
}

t_node	*parse_pipeline(t_shell_data *sh, t_tokarr *ta)
{
	t_node	*left;
	t_node	*pipe;
	t_token	*t;

	left = parse_factor(sh, ta);
	if (!left)
		return (NULL);
	while (1)
	{
		t = peek(ta);
		if (!t || t->type != PIPE)
			break ;
		next(ta);
		pipe = nd_new(ND_PIPE);
		if (!pipe)
			return (left);
		pipe->left = left;
		pipe->right = parse_factor(sh, ta);
		left = pipe;
	}
	return (left);
}
