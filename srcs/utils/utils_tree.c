/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:30:38 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/15 09:22:37 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	free_redirs(t_redir *head)
{
	t_redir	*next;

	while (head)
	{
		next = head->next;
		if (head->word && ft_strncmp(head->word, FD_PREFIX,
				ft_strlen(FD_PREFIX)) == 0 && head->quoted_delim > 0)
		{
			close(head->quoted_delim);
			head->quoted_delim = 0;
		}
		free(head->word);
		free(head);
		head = next;
	}
}

void	free_string_array(char **argv)
{
	int	index;

	if (!argv)
		return ;
	index = 0;
	while (argv[index])
	{
		free(argv[index]);
		index++;
	}
	free(argv);
}

void	free_command_node(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->argv)
	{
		free_string_array(cmd->argv);
		cmd->argv = NULL;
	}
	if (cmd->redirs)
	{
		free_redirs(cmd->redirs);
		cmd->redirs = NULL;
	}
	free(cmd);
}

void	free_subtrees(t_node *node)
{
	if (!node)
		return ;
	if (node->left)
		free_tree(node->left);
	if (node->right)
		free_tree(node->right);
}

void	free_tree(t_node *node)
{
	if (!node)
		return ;
	free_subtrees(node);
	if ((node->type == ND_EXEC || node->type == ND_GROUP) && node->cmd)
	{
		free_command_node(node->cmd);
		node->cmd = NULL;
	}
	free(node);
}
