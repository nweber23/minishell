/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/14 13:30:38 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int has_equal(const char *s)
{
	while (*s)
	{
		if (*s == '=')
			return (1);
		s++;
	}
	return (0);
}

static int	set_from_assignment(t_shell_data *sh, const char *arg)
{
	size_t		klen;
	char		*key;
	const char	*val;
	int			err;

	klen = 0;
	while (arg[klen] && arg[klen] != '=')
		klen++;
	key = ft_substr(arg, 0, klen);
	if (!key)
		return (1);
	val = arg + klen + 1;
	err = env_set(sh, key, val);
	if (!err)
		err = export_add_key(sh, key);
	free(key);
	return (err);
}

int	bi_export(t_shell_data *sh, char **argv)
{
	int	i;
	int	any_err;

	if (!argv || !argv[0])
		return (print_export_list(sh));
	i = 0;
	any_err = 0;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			any_err = 1;
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
		}
		else if (has_equal(argv[i]))
			any_err |= (set_from_assignment(sh, argv[i]) != 0);
		else
			any_err |= (export_add_key(sh, argv[i]) != 0);
		i++;
	}
	return (any_err != 0);
}
