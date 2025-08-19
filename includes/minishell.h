/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:00:25 by nweber            #+#    #+#             */
/*   Updated: 2025/08/19 10:46:29 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>

//EXECUITON

typedef enum e_rtype
{
	R_IN,
	R_OUT,
	R_APP,
	R_HDOC
}	t_rtype;

typedef struct s_redir
{
	t_rtype	type;
	char	*arg;
	int		hdoc_fd;
}	t_redir;

typedef struct s_cmd
{
	char	**argv;
	t_redir	*redirs;
	int		redir_count;
	int		is_builtin;
}	t_cmd;

typedef struct s_pipeline
{
	t_cmd	*cmds;
	int		count;
}	t_pipeline;

int		exec_pipeline(t_pipeline *pl);

int		is_builtin_name(const char *s);
int		run_builtin_parent(t_cmd *c);
int		run_builtin_child(t_cmd *c);

//Signal
void	set_child_signals_default(void);
void	set_parent_signals_interactive(void);
pid_t	spawn_child(int i, int n, int prev[2], int next[2], t_cmd *c);

//Environment
char	**ms_envp(void);
int		ms_collect_all_heredoc(t_pipeline *pl);
int		ms_apply_redirs(const t_cmd *c);
char	*ms_resolve_path(const char *cmd, char **envp);

//Utils
void	close_pair(int p[2]);
int		save_stdio(int fd[2]);
int		restore_stdio(int fd[2]);
char	**split_colon(const char *s);
void	free_split(char **v);
char	*join_path(const char *dir, const char *leaf);
#endif
