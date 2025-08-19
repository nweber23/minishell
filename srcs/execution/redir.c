#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

static int	redir_in(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror("minishell"), -1);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (perror("minishell"), -1);
	}
	close(fd);
	return (0);
}

static int	redir_out_trunc(const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("minishell"), -1);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (perror("minishell"), -1);
	}
	close(fd);
	return (0);
}

static int	redir_out_append(const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror("minishell"), -1);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (perror("minishell"), -1);
	}
	close(fd);
	return (0);
}

int	ms_apply_redirs(const t_cmd *c)
{
	int	i;

	i = 0;
	while (i < c->redir_count)
	{
		if (c->redirs[i].type == R_IN && redir_in(c->redirs[i].arg) < 0)
			return (-1);
		if (c->redirs[i].type == R_OUT
			&& redir_out_trunc(c->redirs[i].arg) < 0)
			return (-1);
		if (c->redirs[i].type == R_APP
			&& redir_out_append(c->redirs[i].arg) < 0)
			return (-1);
		if (c->redirs[i].type == R_HDOC && c->redirs[i].hdoc_fd >= 0)
		{
			if (dup2(c->redirs[i].hdoc_fd, STDIN_FILENO) < 0)
				return (perror("minishell"), -1);
		}
		i++;
	}
	return (0);
}
