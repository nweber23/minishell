/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_build_tree.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:00:57 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 20:23:57 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	list_token_count(t_list *list_head)
{
	int		count;
	t_list	*node;

	count = 0;
	node = list_head;
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count);
}

static void	fill_token_array(t_token **out, t_list *list_head)
{
	int		index;
	t_list	*node;

	index = 0;
	node = list_head;
	while (node)
	{
		out[index] = (t_token *)node->content;
		index++;
		node = node->next;
	}
}

static t_token	**tokens_to_array(t_list *list_head, int *out_count)
{
	int		count;
	t_token	**array;

	count = list_token_count(list_head);
	array = (t_token **)malloc(sizeof(t_token *) * count);
	if (!array)
		return (NULL);
	fill_token_array(array, list_head);
	*out_count = count;
	return (array);
}

t_node	*build_tree(t_shell_data *sh, t_list *tokens)
{
	t_tokarr	tokarr;
	t_node		*root;

	tokarr.v = tokens_to_array(tokens, &tokarr.n);
	if (!tokarr.v)
		return (NULL);
	tokarr.i = 0;
	root = parse_and_or(sh, &tokarr);
	free(tokarr.v);
	return (root);
}
