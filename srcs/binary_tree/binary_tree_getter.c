/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree_getter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:20:19 by nweber            #+#    #+#             */
/*   Updated: 2025/09/09 10:18:12 by yyudi            ###   ########.fr       */
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
	argv[argc] = NULL;
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
