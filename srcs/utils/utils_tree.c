/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:30:38 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/12 10:23:45 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	free_redirs(t_redir *redir_head)
{
	t_redir	*next_redir;

	while (redir_head)
	{
		next_redir = redir_head->next;
		free(redir_head->word);
		free(redir_head);
		redir_head = next_redir;
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
	if (node->type == ND_EXEC && node->cmd)
		free_command_node(node->cmd);
	free(node);
}
