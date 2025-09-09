/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:30 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/09 11:25:42 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	fdpack_init(t_fdpack *p)
{
	p->in = -1;
	p->out = -1;
	p->save_in = -1;
	p->save_out = -1;
}

void	fd_apply_inout(t_fdpack *p)
{
	if (p->in != -1)
	{
		p->save_in = dup(STDIN_FILENO);
		dup2(p->in, STDIN_FILENO);
		close(p->in);
	}
	if (p->out != -1)
	{
		p->save_out = dup(STDOUT_FILENO);
		dup2(p->out, STDOUT_FILENO);
		close(p->out);
	}
}

void	fd_restore(t_fdpack *p)
{
	if (p->save_in != -1)
	{
		dup2(p->save_in, STDIN_FILENO);
		close(p->save_in);
	}
	if (p->save_out != -1)
	{
		dup2(p->save_out, STDOUT_FILENO);
		close(p->save_out);
	}
}

static int	exec_external(t_shell_data *sh, t_cmd *cmd)
{
	char	*program_path;
	char	**env_array;

	program_path = NULL;
	env_array = NULL;
	if (!cmd->argv || !cmd->argv[0])
		return (0);
	if (ft_strchr(cmd->argv[0], '/'))
		program_path = ft_strdup(cmd->argv[0]);
	else
		program_path = find_in_path(sh, cmd->argv[0]);
	if (!program_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	env_array = env_list_to_array(sh->env);
	execve(program_path, cmd->argv, env_array);
	perror("execve");
	ft_array_free(env_array);
	free(program_path);
	return (126);
}


static int run_builtin_parent(t_shell_data *sh, t_cmd *cmd)
{
    t_fdpack p;
    int status;
	char	**expanded;

    fdpack_init(&p);
    if (apply_all_redirs(cmd, &p.in, &p.out) != 0)
    {
        fd_restore(&p);
        return (1);
    }
    fd_apply_inout(&p);
	expanded = expand_argv_if_needed(cmd->argv);
	if (expanded != NULL)
		cmd->argv = expanded;
    status = exec_builtin(sh, cmd->argv);
    fd_restore(&p);
    return (status);
}

static void	child_exec(t_shell_data *sh, t_node *node, int in_fd, int out_fd)
{
	t_fdpack	p;
	char		**expanded;
	int			status;

	reset_child_signals();
	fdpack_init(&p);
	p.in = in_fd;
	p.out = out_fd;
	if (apply_all_redirs(node->cmd, &p.in, &p.out) != 0)
		exit(1);
	if (p.in != -1)
	{
		dup2(p.in, STDIN_FILENO);
		close(p.in);
	}
	if (p.out != -1)
	{
		dup2(p.out, STDOUT_FILENO);
		close(p.out);
	}
	expanded = expand_argv_if_needed(node->cmd->argv);
	if (expanded != NULL)
		node->cmd->argv = expanded;
	if (node->cmd->argv && is_builtin(node->cmd->argv[0]) != 0)
	{
		status = exec_builtin(sh, node->cmd->argv);
		exit(status);
	}
	status = exec_external(sh, node->cmd);
	exit(status);
}




static int	wait_and_status(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	run_exec_node(t_shell_data *sh, t_node *node, int fds[2], int is_top)
{
	pid_t		pid;
	const char	*name;
	int			in_fd;
	int			out_fd;

	if (!node || !node->cmd)
		return (1);
	name = NULL;
	if (node->cmd->argv)
		name = node->cmd->argv[0];
	if (is_top && name && is_builtin(name) != 0)
		return (run_builtin_parent(sh, node->cmd));

	in_fd = -1;
	out_fd = -1;
	if (fds != NULL)
	{
		if (fds[0] != -1)
			in_fd = fds[0];
		if (fds[1] != -1)
			out_fd = fds[1];
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		child_exec(sh, node, in_fd, out_fd);

	/* parent: TUTUP kedua ujung pipe kalau ada */
	if (fds != NULL)
	{
		if (fds[0] != -1)
			close(fds[0]);
		if (fds[1] != -1)
			close(fds[1]);
	}
	return (wait_and_status(pid));
}
