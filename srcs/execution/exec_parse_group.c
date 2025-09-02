/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_group.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:22 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 09:36:23 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_node  *parse_group(t_shell_data *sh, t_tokarr *ta)
{
	t_node  *group_node;
	t_token *token;

	(void)sh;
	token = peek(ta);
	if (!is_lparen(token))
		return (NULL);
	next(ta);
	group_node = nd_new(ND_GROUP);
	if (!group_node)
		return (NULL);
	group_node->left = parse_and_or(sh, ta);
	token = peek(ta);
	if (!is_rparen(token))
		return (NULL);
	next(ta);
	return (group_node);
}
