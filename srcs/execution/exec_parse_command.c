/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:19 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/28 18:25:04 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	is_redir_tok(t_token *t)
{
	if (!t)
		return (0);
	if (t->type == INFILE || t->type == OUTFILE)
		return (1);
	if (t->type == APPEND || t->type == HERE_DOC)
		return (1);
	return (0);
}

static int	argv_len(char **v)
{
	int	i;

	i = 0;
	if (!v)
		return (0);
	while (v[i])
		i++;
	return (i);
}

static int	append_word_simple(char ***argv, char *w)
{
	char	**newv;
	int		n;
	int		i;

	n = argv_len(*argv);
	newv = (char **)malloc(sizeof(char *) * (n + 2));
	if (!newv)
		return (0);
	i = 0;
	while (i < n)
	{
		newv[i] = (*argv)[i];
		i++;
	}
	newv[i] = w;
	newv[i + 1] = NULL;
	free(*argv);
	*argv = newv;
	return (1);
}

static int	handle_word(t_tokarr *ta, t_cmd *cmd)
{
	t_token	*t;
	char	*cp;

	t = peek(ta);
	if (!t || t->type != WORD)
		return (0);
	cp = ft_strdup(t->value);
	if (!append_word_simple(&cmd->argv, cp))
		return (0);
	next(ta);
	return (1);
}

static int	handle_redir(t_tokarr *ta, t_cmd *cmd)
{
	t_token		*op;
	t_token		*arg;
	t_rdrtype	rt;

	op = peek(ta);
	if (!op)
		return (0);
	next(ta);
	arg = peek(ta);
	if (!arg || arg->type != WORD)
		return (0);
	rt = map_rdr(op->type);
	if (!add_redir(&cmd->redirs, rdr_new(rt, ft_strdup(arg->value))))
		return (0);
	next(ta);
	return (1);
}

static int	parse_command_loop(t_tokarr *ta, t_cmd *cmd)
{
	t_token	*t;
	int		ok;

	ok = 1;
	t = peek(ta);
	while (t && !is_cmd_end(t) && !is_lparen(t))
	{
		if (t->type == WORD)
			ok = handle_word(ta, cmd);
		else if (is_redir_tok(t))
			ok = handle_redir(ta, cmd);
		else
			break ;
		if (!ok)
			return (0);
		t = peek(ta);
	}
	return (1);
}

t_node	*parse_command(t_shell_data *sh, t_tokarr *ta)
{
	t_node	*n;
	t_cmd	*cmd;
	int		ok;

	(void)sh;
	n = nd_new(ND_EXEC);
	if (!n)
		return (NULL);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (free(n), NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	ok = parse_command_loop(ta, cmd);
	if (!ok)
		return (free(cmd), free(n), NULL);
	n->cmd = cmd;
	return (n);
}
