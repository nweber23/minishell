/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 10:37:29 by nweber            #+#    #+#             */
/*   Updated: 2025/09/12 14:26:10 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_env_list(t_shell_data *sh, char **envp)
{
	int		i;
	char	*eq;
	size_t	klen;
	char	*key;
	char	*val;

	i = 0;
	sh->env = NULL;
	while (envp && envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			klen = (size_t)(eq - envp[i]);
			key = ft_substr(envp[i], 0, klen);
			val = ft_strdup(eq + 1);
			if (!key || !val || env_set(sh, key, val) != 0)
				error_malloc("create_env_list", sh);
			free(key);
			free(val);
		}
		i++;
	}
}

static void	bump_shlvl(t_shell_data *sh)
{
	const char	*cur;
	int			val;
	char		*s;

	cur = env_get(sh->env, "SHLVL");
	if (cur == NULL)
		val = 0;
	else
		val = ft_atoi(cur);
	s = ft_itoa(val + 1);
	if (!s)
		error_malloc("bump_shlvl", sh);
	if (env_set(sh, "SHLVL", s) != 0)
		error_malloc("bump_shlvl:env_set", sh);
	free(s);
}

int	main(int ac, char **av, char **envp)
{
	t_shell_data	sh;
	int				ret;

	(void)av;
	ft_memset(&sh, 0, sizeof(sh));
	if (ac > 1)
	{
		ft_putendl_fd("i dont want arguments", 2);
		return (EXIT_FAILURE);
	}
	create_env_list(&sh, envp);
	bump_shlvl(&sh);
	ret = minishell_loop(&sh, envp);
	free_shell(&sh);
	free_env(sh.env);
	cleanup_readline_tty(&sh);
	rl_clear_history();
	return (ret);
}
