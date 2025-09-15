/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 18:08:32 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/15 09:11:09 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	replace_heredoc_with_procfd(t_redir *r, t_shell_data *sh)
{
	int		fd;
	char	*num;
	char	*path;

	(void)sh;
	fd = heredoc_to_fd(r);
	if (fd < 0)
		return (1);
	r->quoted_delim = fd;
	num = ft_itoa(fd);
	if (num == NULL)
		return (1);
	path = ft_str3var(FD_PREFIX, num, "");
	free(num);
	if (path == NULL)
		return (1);
	free(r->word);
	r->word = path;
	r->type = R_IN;
	return (0);
}

static int	prepare_cmd_heredocs(t_cmd *c, t_shell_data *sh)
{
	t_redir	*rd;

	if (c == NULL)
		return (0);
	rd = c->redirs;
	while (rd != NULL)
	{
		if (rd->type == R_HEREDOC)
		{
			if (replace_heredoc_with_procfd(rd, sh) != 0)
				return (1);
		}
		rd = rd->next;
	}
	return (0);
}

int	prepare_heredocs_tree(t_node *n, t_shell_data *sh)
{
	if (n == NULL)
		return (0);
	if (n->type == ND_EXEC)
		return (prepare_cmd_heredocs(n->cmd, sh));
	if (prepare_heredocs_tree(n->left, sh) != 0)
		return (1);
	if (prepare_heredocs_tree(n->right, sh) != 0)
		return (1);
	return (0);
}
