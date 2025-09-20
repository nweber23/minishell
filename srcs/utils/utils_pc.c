/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 10:15:09 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/20 10:15:24 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_and_status(pid_t child_pid)
{
	int	status;

	if (waitpid(child_pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

void	select_inout_from_pair(int pair[2], int *in_fd, int *out_fd)
{
	*in_fd = -1;
	*out_fd = -1;
	if (pair == NULL)
		return ;
	if (pair[0] != -1)
		*in_fd = pair[0];
	if (pair[1] != -1)
		*out_fd = pair[1];
}

int	exec_line(t_shell_data *sh, t_node *root)
{
	int	code;

	if (root == NULL)
	{
		sh->exit_code = 0;
		return (exit_code(0));
	}
	if (prepare_heredocs_tree(root, sh) != 0)
	{
		sh->exit_code = 1;
		return (exit_code(1));
	}
	code = run_node(sh, root, 1);
	sh->exit_code = code;
	exit_code(code);
	return (code);
}
