/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 20:45:02 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 20:46:14 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	print_cmd_not_found(const char *name)
{
	ft_putstr_fd("minishell: ", 2);
	if (name != NULL)
		ft_putstr_fd((char *)name, 2);
	ft_putendl_fd(": command not found", 2);
}

char	*resolve_program_path(t_shell_data *sh, const char *name)
{
	char	*program_path;

	program_path = NULL;
	if (name == NULL)
		return (NULL);
	if (ft_strchr(name, '/'))
		program_path = ft_strdup(name);
	else
		program_path = find_in_path(sh, name);
	return (program_path);
}

void	apply_dup_and_close(int from_fd, int to_fd)
{
	if (from_fd != -1)
	{
		dup2(from_fd, to_fd);
		close(from_fd);
	}
}

void	close_pair_if_set(int fds[2])
{
	if (fds == NULL)
		return ;
	if (fds[0] != -1)
		close(fds[0]);
	if (fds[1] != -1)
		close(fds[1]);
}

int	exec_external(t_shell_data *sh, t_cmd *cmd)
{
	char	*program_path;
	char	**env_array;
	int		err;

	if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL)
		return (0);
	program_path = resolve_program_path(sh, cmd->argv[0]);
	if (program_path == NULL)
		return (print_cmd_not_found(cmd->argv[0]), 127);
	env_array = env_list_to_array(sh->env);
	execve(program_path, cmd->argv, env_array);
	err = errno;
	ft_array_free(env_array);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(err), 2);
	free(program_path);
	if (err == ENOENT)
		return (127);
	return (126);
}
