/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_build_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:00:57 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/02 09:33:01 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static t_token **tokens_to_array(t_list *list_head, int *out_count)
{
	int	 count;
	int	 index;
	t_token **array;
	t_list  *node;

	count = 0;
	node = list_head;
	while (node)
	{
		count++;
		node = node->next;
	}
	array = (t_token **)malloc(sizeof(t_token *) * count);
	if (!array)
		return (NULL);
	index = 0;
	node = list_head;
	while (node)
	{
		array[index] = (t_token *)node->content;
		index++;
		node = node->next;
	}
	*out_count = count;
	return (array);
}

t_node  *build_tree(t_shell_data *sh, t_list *tokens)
{
	t_tokarr	tokarr;
	t_node	  *root;

	tokarr.v = tokens_to_array(tokens, &tokarr.n);
	if (!tokarr.v)
		return (NULL);
	tokarr.i = 0;
	root = parse_and_or(sh, &tokarr);
	free(tokarr.v);
	return (root);
}

static void free_redirs(t_redir *redir_head)
{
	t_redir *next_redir;

	while (redir_head)
	{
		next_redir = redir_head->next;
		free(redir_head->word);
		free(redir_head);
		redir_head = next_redir;
	}
}

void	free_tree(t_node *node)
{
	int arg_index;

	if (!node)
		return ;
	free_tree(node->left);
	free_tree(node->right);
	if (node->type == ND_EXEC && node->cmd)
	{
		arg_index = 0;
		if (node->cmd->argv)
		{
			while (node->cmd->argv[arg_index])
			{
				free(node->cmd->argv[arg_index]);
				arg_index++;
			}
			free(node->cmd->argv);
		}
		free_redirs(node->cmd->redirs);
		free(node->cmd);
	}
	free(node);
}
