/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 17:15:11 by nweber            #+#    #+#             */
/*   Updated: 2025/09/08 15:39:25 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_lvl(t_shell_data *shell)
{
	t_list	*temp;
	int		value;
	char	*str_value;

	if (!shell)
		return ;
	temp = shell->env;
	while (temp)
	{
		if (ft_strcmp(((t_env *)temp->content)->value, "SHELL_LVL=") == 0)
		{
			value = ft_atoi(((t_env *)temp->content)->content);
			value++;
			str_value = ft_itoa(value);
			free(((t_env *)temp->content)->content);
			((t_env *)temp->content)->content = str_value;
			break ;
		}
		temp = temp->next;
	}
}

static void create_env_list(t_shell_data *sh, char **envp)
{
    int   i = 0;

    sh->env = NULL;
    while (envp && envp[i])
    {
        char *eq = ft_strchr(envp[i], '=');
        if (eq) {
            size_t klen = (size_t)(eq - envp[i]);
            char  *key  = ft_substr(envp[i], 0, klen);
            char  *val  = ft_strdup(eq + 1);
            if (!key || !val || env_set(sh, key, val) != 0) // utils_env.c
                error_malloc("create_env_list", sh);
            free(key);
            free(val);
        }
        // entri tanpa '=' bisa di-skip
        i++;
    }
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	shell = {0}; // remove {0} when env actually is init

	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("i dont want arguments", 2);
		exit(EXIT_FAILURE);
	}
	create_env_list(&shell, envp);
	shell_lvl(&shell);
	minishell_loop(&shell, envp);
	free_env(shell.env);
	return (0);
}
