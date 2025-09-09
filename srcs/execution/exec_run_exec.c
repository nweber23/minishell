/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:30 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/09 12:59:34 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	print_cmd_not_found(const char *name)
{
	ft_putstr_fd("minishell: ", 2);
	if (name != NULL)
		ft_putstr_fd((char *)name, 2);
	ft_putendl_fd(": command not found", 2);
}

static char	*resolve_program_path(t_shell_data *sh, const char *name)
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

static void	apply_dup_and_close(int from_fd, int to_fd)
{
	if (from_fd != -1)
	{
		dup2(from_fd, to_fd);
		close(from_fd);
	}
}

static void	close_pair_if_set(int fds[2])
{
	if (fds == NULL)
		return ;
	if (fds[0] != -1)
		close(fds[0]);
	if (fds[1] != -1)
		close(fds[1]);
}

static char	**maybe_expand_argv(char **argv)
{
	char	**expanded;

	expanded = expand_argv_if_needed(argv);
	if (expanded != NULL)
		return (expanded);
	return (argv);
}

static int	exec_external(t_shell_data *sh, t_cmd *cmd)
{
	char	*program_path;
	char	**env_array;

	if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL)
		return (0);
	program_path = resolve_program_path(sh, cmd->argv[0]);
	if (program_path == NULL)
		return (print_cmd_not_found(cmd->argv[0]), 127);
	env_array = env_list_to_array(sh->env);
	execve(program_path, cmd->argv, env_array);
	perror("execve");
	ft_array_free(env_array);
	free(program_path);
	return (126);
}

static int	exec_builtin_in_parent(t_shell_data *sh, t_cmd *cmd)
{
	t_fdpack	fd_pack;
	int			exit_status;

	fdpack_init(&fd_pack);
	if (apply_all_redirs(cmd, &fd_pack.in, &fd_pack.out) != 0)
	{
		fd_restore(&fd_pack);
		return (1);
	}
	fd_apply_inout(&fd_pack);
	cmd->argv = maybe_expand_argv(cmd->argv);
	exit_status = exec_builtin(sh, cmd->argv);
	fd_restore(&fd_pack);
	return (exit_status);
}

static void	child_exec(t_shell_data *sh, t_node *node, int in_fd, int out_fd)
{
	t_fdpack	fd_pack;
	int			exit_status;

	reset_child_signals();
	fdpack_init(&fd_pack);
	fd_pack.in = in_fd;
	fd_pack.out = out_fd;
	if (apply_all_redirs(node->cmd, &fd_pack.in, &fd_pack.out) != 0)
		exit(1);
	apply_dup_and_close(fd_pack.in, STDIN_FILENO);
	apply_dup_and_close(fd_pack.out, STDOUT_FILENO);
	node->cmd->argv = maybe_expand_argv(node->cmd->argv);
	if (node->cmd->argv != NULL && is_builtin(node->cmd->argv[0]) != 0)
	{
		exit_status = exec_builtin(sh, node->cmd->argv);
		exit(exit_status);
	}
	exit_status = exec_external(sh, node->cmd);
	exit(exit_status);
}

static int	wait_and_status(pid_t child_pid)
{
	int	status;

	if (waitpid(child_pid, &status, 0) == -1)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static void	select_inout_from_pair(int pair[2], int *in_fd, int *out_fd)
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

int	run_exec_node(t_shell_data *sh, t_node *node, int pipe_fds[2], int is_top)
{
	pid_t		child_pid;
	const char	*cmd_name;
	int			in_fd;
	int			out_fd;

	if (node == NULL || node->cmd == NULL)
		return (1);
	cmd_name = NULL;
	if (node->cmd->argv != NULL)
		cmd_name = node->cmd->argv[0];
	if (is_top != 0 && cmd_name != NULL && is_builtin(cmd_name) != 0)
		return (exec_builtin_in_parent(sh, node->cmd));
	select_inout_from_pair(pipe_fds, &in_fd, &out_fd);
	child_pid = fork();
	if (child_pid == -1)
		return (perror("fork"), 1);
	if (child_pid == 0)
		child_exec(sh, node, in_fd, out_fd);
	close_pair_if_set(pipe_fds);
	return (wait_and_status(child_pid));
}