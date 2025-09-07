/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 17:15:11 by nweber            #+#    #+#             */
/*   Updated: 2025/09/07 16:17:54 by nweber           ###   ########.fr       */
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
			break;
		}
		temp = temp->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	shell;

	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("i dont want arguments", 2);
		exit(EXIT_FAILURE);
	}
	// create_env_list(&shell, envp); // Builtin -> yyudi
	shell_lvl(&shell);
	minishell_loop(&shell, envp);
	free_env(shell.env);
	return (0);
}
