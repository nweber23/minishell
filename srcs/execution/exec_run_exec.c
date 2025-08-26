/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 12:01:30 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:01:31 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "execution.h"

static int	exec_external(t_shell_data *sh, t_cmd *c)
{
	char	*path;

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
	execve(path, c->argv, NULL);
	perror("execve");
	free(path);
	return (126);
}

static int	apply_all_redirs(t_cmd *c, int *fdin, int *fdout)
{
	if (apply_redirs_files(c, fdin, fdout))
		return (1);
	if (apply_redirs_heredoc(c, fdin))
		return (1);
	return (0);
}

int	run_exec_node(t_shell_data *sh, t_node *n, int in_fd, int out_fd, int is_top)
{
	pid_t	pid;
	int		status;
	char	**argv_exp;
	int		fdin;
	int		fdout;

	if (!n->cmd)
		return (1);
	if (n->cmd->argv && n->cmd->argv[0] && is_builtin(n->cmd->argv[0]) && is_top)
	{
		if (apply_all_redirs(n->cmd, &fdin, &fdout))
			return (1);
		if (fdin != -1)
		{
			int tmp = dup(STDIN_FILENO);
			dup2(fdin, STDIN_FILENO);
			close(fdin);
			status = exec_builtin(sh, n->cmd->argv);
			dup2(tmp, STDIN_FILENO);
			close(tmp);
			return (status);
		}
		if (fdout != -1)
		{
			int tmp = dup(STDOUT_FILENO);
			dup2(fdout, STDOUT_FILENO);
			close(fdout);
			status = exec_builtin(sh, n->cmd->argv);
			dup2(tmp, STDOUT_FILENO);
			close(tmp);
			return (status);
		}
		return (exec_builtin(sh, n->cmd->argv));
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		if (apply_all_redirs(n->cmd, &fdin, &fdout))
			_exit(1);
		if (fdin != -1 && dup2(fdin, STDIN_FILENO) < 0)
			_exit(1);
		if (fdout != -1 && dup2(fdout, STDOUT_FILENO) < 0)
			_exit(1);
		if (in_fd != -1 && in_fd != STDIN_FILENO)
		{
			if (dup2(in_fd, STDIN_FILENO) < 0)
				_exit(1);
		}
		if (out_fd != -1 && out_fd != STDOUT_FILENO)
		{
			if (dup2(out_fd, STDOUT_FILENO) < 0)
				_exit(1);
		}
		argv_exp = expand_argv_if_needed(n->cmd->argv);
		if (argv_exp)
			n->cmd->argv = argv_exp;
		_exit(exec_external(sh, n->cmd));
	}
	if (in_fd != -1 && in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != -1 && out_fd != STDOUT_FILENO)
		close(out_fd);
	if (waitpid(pid, &status, 0) == -1)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
