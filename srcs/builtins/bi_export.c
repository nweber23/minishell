
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 10:55:00 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	valid_key(const char *s)
{
	int	i;

	if (!s || !(ft_isalpha(*s) || *s == '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	set_env_kv(t_shell_data *sh, const char *kv)
{
	t_list	*p;
	size_t	keylen;

	keylen = 0;
	while (kv[keylen] && kv[keylen] != '=')
		keylen++;
	p = sh->env;
	while (p)
	{
		if (ft_strncmp((char *)p->content, kv, keylen) == 0
			&& ((char *)p->content)[keylen] == '=')
		{
			free(p->content);
			p->content = ft_strdup(kv);
			return ;
		}
		p = p->next;
	}
	ft_lstadd_back(&sh->env, ft_lstnew(ft_strdup(kv)));
}

int	bi_export(t_shell_data *sh, char **argv)
{
	int	i;

	if (!argv || !argv[0])
		return (bi_env(sh));
	i = 0;
	while (argv[i])
	{
		if (!valid_key(argv[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
		}
		else
			set_env_kv(sh, argv[i]);
		i++;
	}
	return (0);
}
