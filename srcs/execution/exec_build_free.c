/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_build_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:00:57 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:14:06 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_token	**tokens_to_array(t_list *lst, int *out_n)
{
	int		n;
	int		i;
	t_token	**arr;
	t_list	*p;

	n = 0;
	p = lst;
	while (p)
	{
		n++;
		p = p->next;
	}
	arr = (t_token **)malloc(sizeof(t_token *) * n);
	if (!arr)
		return (NULL);
	i = 0;
	p = lst;
	while (p)
	{
		arr[i] = (t_token *)p->content;
		i++;
		p = p->next;
	}
	*out_n = n;
	return (arr);
}

t_node	*build_tree(t_shell_data *sh, t_list *tokens)
{
	t_tokarr	ta;
	t_node		*root;

	ta.v = tokens_to_array(tokens, &ta.n);
	if (!ta.v)
		return (NULL);
	ta.i = 0;
	root = parse_and_or(sh, &ta);
	free(ta.v);
	return (root);
}

static void	free_redirs(t_redir *r)
{
	t_redir	*n;

	while (r)
	{
		n = r->next;
		free(r->word);
		free(r);
		r = n;
	}
}

void	free_tree(t_node *n)
{
	int	i;

	if (!n)
		return ;
	free_tree(n->left);
	free_tree(n->right);
	if (n->type == ND_EXEC && n->cmd)
	{
		i = 0;
		if (n->cmd->argv)
		{
			while (n->cmd->argv[i])
			{
				free(n->cmd->argv[i]);
				i++;
			}
			free(n->cmd->argv);
		}
		free_redirs(n->cmd->redirs);
		free(n->cmd);
	}
	free(n);
}
