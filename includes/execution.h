/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 12:15:00 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <dirent.h>


typedef struct s_fdpack
{
	int	in;
	int	out;
	int	save_in;
	int	save_out;
}	t_fdpack;


/* Node types for the execution tree */
typedef enum e_ndtype
{
	ND_EXEC,
	ND_PIPE,
	ND_AND,
	ND_OR,
	ND_GROUP
}	t_ndtype;

typedef enum e_rdrtype
{
	R_IN,
	R_OUT_TRUNC,
	R_OUT_APPEND,
	R_HEREDOC
}	t_rdrtype;

typedef struct s_redir
{
	t_rdrtype	type;
	char		*word; /* filename or delimiter */
	struct s_redir *next;
}	t_redir;

typedef struct s_cmd
{
	char	**argv;
	t_redir	*redirs;
}	t_cmd;

typedef struct s_node
{
	t_ndtype		type;
	struct s_node	*left;
	struct s_node	*right;
	t_cmd			*cmd; /* used when type == ND_EXEC */
}	t_node;

/* token stream view */
typedef struct s_tokarr
{
	t_token	**v;
	int		n;
	int		i;
}	t_tokarr;

/* ===== Public build/exec API ===== */
t_node	*build_tree(t_shell_data *sh, t_list *tokens);
void	free_tree(t_node *n);
int		exec_line(t_shell_data *sh, t_node *root);
int		exec_builtin(t_shell_data *sh, char **argv);
int		is_builtin(const char *name);
char	**expand_argv_if_needed(char **argv);

/* ===== Builtins ===== */
int		bi_echo(char **argv);
int		bi_pwd(void);
int		bi_cd(t_shell_data *sh, char **argv);
int		bi_env(t_shell_data *sh);
int		bi_export(t_shell_data *sh, char **argv);
int		bi_unset(t_shell_data *sh, char **argv);
int		bi_exit(t_shell_data *sh, char **argv);

/* ===== Internals shared across units (keep small & explicit) ===== */
/* ast helpers */
t_node	*nd_new(t_ndtype t);
t_redir	*rdr_new(t_rdrtype t, char *word);
int		is_lparen(t_token *t);
int		is_rparen(t_token *t);

/* token stream helpers */
t_token	*peek(t_tokarr *ta);
t_token	*next(t_tokarr *ta);
int		is_cmd_end(t_token *t);

/* argv/redirect assembly */
int		append_word(char ***argv, int *argc, char *w);
int		add_redir(t_redir **lst, t_redir *node);
t_rdrtype	map_rdr(t_token_type tt);

/* parser pieces */
t_node	*parse_group(t_shell_data *sh, t_tokarr *ta);
t_node	*parse_command(t_shell_data *sh, t_tokarr *ta);
t_node	*parse_pipeline(t_shell_data *sh, t_tokarr *ta);
t_node	*parse_and_or(t_shell_data *sh, t_tokarr *ta);

/* redirections + path + exec */
int		apply_redirs_files(t_cmd *c, int *fdin, int *fdout);
int		apply_redirs_heredoc(t_cmd *c, int *fdin);
char	*find_in_path(t_shell_data *sh, const char *cmd);
int		run_exec_node(t_shell_data *sh, t_node *n, int *fds, int is_top);
int		run_pipe(t_shell_data *sh, t_node *n, int is_top);
int		run_node(t_shell_data *sh, t_node *n, int is_top);

// utils env helpers (header)
char	*env_get(t_list *env, const char *key);
int		env_set(t_shell_data *sh, const char *key, const char *value);
char	**env_list_to_array(t_list *env);

//String Util
void	ft_array_free(char **array);
char	*ft_str3var(const char *s1, const char *s2, const char *s3);

#endif
