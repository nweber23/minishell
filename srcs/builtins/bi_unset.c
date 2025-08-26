
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 10:55:00 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	key_match(const char *kv, const char *key)
{
	size_t	i;

	i = 0;
	while (kv[i] && kv[i] != '=')
		i++;
	if (ft_strlen(key) != i)
		return (0);
	if (ft_strncmp(kv, key, i) == 0)
		return (1);
	return (0);
}

int	bi_unset(t_shell_data *sh, char **argv)
{
	t_list	*prev;
	t_list	*cur;
	t_list	*next;
	int		i;

	if (!argv)
		return (0);
	i = 0;
	while (argv[i])
	{
		prev = NULL;
		cur = sh->env;
		while (cur)
		{
			next = cur->next;
			if (key_match((char *)cur->content, argv[i]))
			{
				if (prev)
					prev->next = next;
				else
					sh->env = next;
				free(cur->content);
				free(cur);
				break ;
			}
			prev = cur;
			cur = next;
		}
		i++;
	}
	return (0);
}
