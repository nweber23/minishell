/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:19 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:20 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

t_node	*parse_command(t_shell_data *sh, t_tokarr *ta)
{
	t_node	*n;
	t_cmd	*cmd;
	t_redir	*redirs;
	char	**argv;
	int		argc;
	t_token	*t;

	(void)sh;
	n = nd_new(ND_EXEC);
	if (!n)
		return (NULL);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (free(n), NULL);
	argv = NULL;
	argc = 0;
	redirs = NULL;
	while (1)
	{
		t = peek(ta);
		if (!t || is_cmd_end(t) || is_lparen(t))
			break ;
		if (t->type == WORD)
		{
			if (!append_word(&argv, &argc, ft_strdup(t->value)))
				return (free(cmd), free(n), NULL);
			next(ta);
		}
		else if (t->type == INFILE || t->type == OUTFILE
			|| t->type == APPEND || t->type == HERE_DOC)
		{
			t_token *op = next(ta);
			t_token *arg = peek(ta);
			if (!arg || arg->type != WORD)
				return (free(cmd), free(n), NULL);
			if (!add_redir(&redirs, rdr_new(map_rdr(op->type),
						ft_strdup(arg->value))))
				return (free(cmd), free(n), NULL);
			next(ta);
		}
		else
			break ;
	}
	cmd->argv = argv;
	cmd->redirs = redirs;
	n->cmd = cmd;
	return (n);
}
