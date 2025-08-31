/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:30 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/31 11:03:07 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	exec_external(t_shell_data *sh, t_cmd *c)
{
	char	*path;
	char	**envp;

	(void)sh;
	if (!c->argv || !c->argv[0])
		return (0);
	if (ft_strchr(c->argv[0], '/'))
		path = ft_strdup(c->argv[0]);
	else
		path = find_in_path(sh, c->argv[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(c->argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	envp = env_list_to_array(sh->env);
	execve(path, c->argv, NULL);
	perror("execve");
	free(path);
	return (126);
}

/* ---------- redirs combo (≤4 var, ≤25 lines) ---------- */

static int	apply_all_redirs(t_cmd *c, int *fdin, int *fdout)
{
	if (apply_redirs_files(c, fdin, fdout))
		return (1);
	if (apply_redirs_heredoc(c, fdin))
		return (1);
	return (0);
}

/* ---------- tiny fd helpers (short, ≤25 lines) ---------- */

static void	fdpack_init(t_fdpack *p)
{
	p->in = -1;
	p->out = -1;
	p->save_in = -1;
	p->save_out = -1;
}

static void	fd_restore(t_fdpack *p)
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

/* ---------- builtin on parent (≤4 var, ≤25 lines) ---------- */

static int	run_builtin_parent(t_shell_data *sh, t_cmd *c)
{
	t_fdpack	p;
	int			status;

	fdpack_init(&p);
	if (apply_all_redirs(c, &p.in, &p.out))
		return (1);
	if (p.in != -1)
	{
		p.save_in = dup(STDIN_FILENO);
		dup2(p.in, STDIN_FILENO);
		close(p.in);
	}
	if (p.out != -1)
	{
		p.save_out = dup(STDOUT_FILENO);
		dup2(p.out, STDOUT_FILENO);
		close(p.out);
	}
	status = exec_builtin(sh, c->argv);
	fd_restore(&p);
	return (status);
}

/* ---------- child path (≤4 var, ≤25 lines) ---------- */

static void	child_exec(t_shell_data *sh, t_node *n, int fds[2])
{
	t_fdpack	p;
	char		**expv;

	fdpack_init(&p);
	if (apply_all_redirs(n->cmd, &p.in, &p.out))
		_exit(1);
	if (p.in != -1 && dup2(p.in, STDIN_FILENO) < 0)
		_exit(1);
	if (p.out != -1 && dup2(p.out, STDOUT_FILENO) < 0)
		_exit(1);
	if (fds[0] != -1 && fds[0] != STDIN_FILENO)
		if (dup2(fds[0], STDIN_FILENO) < 0)
			_exit(1);
	if (fds[1] != -1 && fds[1] != STDOUT_FILENO)
		if (dup2(fds[1], STDOUT_FILENO) < 0)
			_exit(1);
	expv = expand_argv_if_needed(n->cmd->argv);
	if (expv)
		n->cmd->argv = expv;
	if (n->cmd->argv && n->cmd->argv[0] && is_builtin(n->cmd->argv[0]))
		_exit(exec_builtin(sh, n->cmd->argv));
	_exit(exec_external(sh, n->cmd));
}

/* ---------- wait helper (≤4 var, ≤25 lines) ---------- */

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

/* ---------- public entry (≤4 args, ≤25 lines) ---------- */

int	run_exec_node(t_shell_data *sh, t_node *n, int fds[2], int is_top)
{
	pid_t		pid;
	const char	*name;

	if (!n->cmd)
		return (1);
	name = NULL;
	if (n->cmd->argv)
		name = n->cmd->argv[0];
	if (is_top && name && is_builtin(name))
		return (run_builtin_parent(sh, n->cmd));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		child_exec(sh, n, fds);
	if (fds[0] != -1 && fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds[1] != -1 && fds[1] != STDOUT_FILENO)
		close(fds[1]);
	return (wait_and_status(pid));
}
