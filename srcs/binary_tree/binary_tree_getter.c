/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree_getter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:20:19 by nweber            #+#    #+#             */
/*   Updated: 2025/09/07 17:16:38 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_argv(t_shell_data *shell, t_list *tokens)
{
	int		argc;
	char	**argv;
	int		index;

	index = 0;
	argc = count_args(tokens);
	argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		error_malloc("get_argv", shell);
	while (tokens && ((t_token *)tokens->content)->type != PIPE && index < argc)
	{
		if (!token_check(tokens))
			break ;
		if (tokens && ((t_token *)tokens->content)->type == WORD)
		{
			tokens = word_check(&tokens, argv, &index);
			continue ;
		}
		if (!token_check(tokens->next))
			break ;
		tokens = tokens->next->next;
	}
	argv[argc] = '\0';
	return (argv);
}

t_list	*get_name(t_list *tokens)
{
	t_list	*word;

	word = NULL;
	while (tokens && ((t_token *)tokens->content)->type != PIPE)
	{
		if (tokens && (((t_token *)tokens->content)->type == HERE_DOC \
		|| ((t_token *)tokens->content)->type == INFILE))
		{
			tokens = tokens->next->next;
			continue ;
		}
		if (tokens && (((t_token *)tokens->content)->type == APPEND \
		|| ((t_token *)tokens->content)->type == OUTFILE))
		{
			tokens = tokens->next->next;
			continue ;
		}
		if (tokens && (((t_token *)tokens->content)->type == WORD \
		|| ((t_token *)tokens->content)->state != EXPAND))
			return (tokens);
		tokens = tokens->next;
		if (tokens && ((t_token *)tokens->content)->type == AND)
			break ;
	}
	return (word);
}

t_list	*get_infiles(t_shell_data *shell, t_list *tokens, t_list **infile)
{
	t_infile	*inf;

	inf = NULL;
	while (tokens && ((t_token *)tokens->content)->type != PIPE)
	{
		if (tokens && (((t_token *)tokens->content)->type == HERE_DOC \
		|| ((t_token *)tokens->content)->type == INFILE))
		{
			inf = malloc(sizeof(t_infile));
			if (!infile)
				error_malloc("get_infiles", shell);
			if (((t_token *)tokens->content)->type == INFILE)
				inf->type = INF;
			else
				inf->type = HERE;
			inf->eof = ft_strdup(((t_token *)tokens->content)->value);
			ft_lstadd_back(infile, ft_lstnew(inf));
			tokens = tokens->next->next;
			continue ;
		}
		tokens = tokens->next;
		if (token_check(tokens))
			break ;
	}
	return (tokens);
}

t_list	*get_outfiles(t_shell_data *shell, t_list *tokens, t_list **outfile)
{
	t_outfile	*outf;

	outf = NULL;
	while (tokens && ((t_token *)tokens->content)->type != PIPE)
	{
		if (tokens && (((t_token *)tokens->content)->type == APPEND \
		|| ((t_token *)tokens->content)->type == OUTFILE))
		{
			outf = malloc(sizeof(t_outfile));
			if (!outfile)
				error_malloc("get_outfiles", shell);
			if (((t_token *)tokens->content)->type == OUTFILE)
				outf->type = ADD;
			else
				outf->type = APP;
			outf->name = ft_strdup(((t_token *)tokens->content)->value);
			ft_lstadd_back(outfile, ft_lstnew(outf));
			tokens = tokens->next->next;
			continue ;
		}
		tokens = tokens->next;
		if (token_check(tokens))
			break ;
	}
	return (tokens);
}
