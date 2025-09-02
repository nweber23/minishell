/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:23 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 09:59:41 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static t_node	*parse_factor(t_shell_data *sh, t_tokarr *ta)
{
	t_token	*token;

	token = peek(ta);
	if (is_lparen(token))
		return (parse_group(sh, ta));
	return (parse_command(sh, ta));
}

t_node	*parse_pipeline(t_shell_data *sh, t_tokarr *ta)
{
	t_node	*left_node;
	t_node	*pipe_node;
	t_token	*token;

	left_node = parse_factor(sh, ta);
	if (!left_node)
		return (NULL);
	while (1)
	{
		token = peek(ta);
		if (!token || token->type != PIPE)
			break ;
		next(ta);
		pipe_node = nd_new(ND_PIPE);
		if (!pipe_node)
			return (left_node);
		pipe_node->left = left_node;
		pipe_node->right = parse_factor(sh, ta);
		left_node = pipe_node;
	}
	return (left_node);
}
