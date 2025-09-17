/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/17 17:04:05 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <dirent.h>

# ifdef __APPLE__
#  define FD_PREFIX "/dev/fd/"
# else
#  define FD_PREFIX "/proc/self/fd/"
# endif

typedef struct s_fdpack
{
	int				in;
	int				out;
	int				save_in;
	int				save_out;
}					t_fdpack;

typedef enum e_rdrtype
{
	R_IN,
	R_OUT_TRUNC,
	R_OUT_APPEND,
	R_HEREDOC
}					t_rdrtype;

typedef struct s_redir
{
	t_rdrtype		type;
	char			*word;
	int				quoted_delim;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
}					t_cmd;

typedef struct s_node
{
	t_node_type		type;
	struct s_node	*left;
	struct s_node	*right;
	t_cmd			*cmd;
}					t_node;

typedef struct s_tokarr
{
	t_token			**v;
	int				n;
	int				i;
}					t_tokarr;

/* ===== Public build/exec API ===== */
t_node				*build_tree(t_shell_data *shell, t_list *tokens);
int					exec_line(t_shell_data *sh, t_node *root);
int					exec_builtin(t_shell_data *sh, char **argv);
int					is_builtin(const char *name);
char				**expand_argv_if_needed(char **argv);

/* ===== Builtins ===== */
int					bi_echo(char **argv);
int					bi_pwd(void);
int					bi_cd(t_shell_data *sh, char **argv);
int					bi_env(t_shell_data *sh);
int					bi_export(t_shell_data *sh, char **argv);
int					bi_unset(t_shell_data *sh, char **argv);
int					bi_exit(t_shell_data *sh, char **argv);

/* ===== Internals shared across units (keep small & explicit) ===== */
/* ast helpers */
t_node				*nd_new(t_node_type t);
t_redir				*rdr_new(t_rdrtype t, char *word);
int					is_lparen(t_token *t);
int					is_rparen(t_token *t);

/* token stream helpers */
t_token				*peek(t_tokarr *ta);
t_token				*next(t_tokarr *ta);
int					is_cmd_end(t_token *t);
int					heredoc_to_fd(t_redir *r);
char				*expand_line_env(t_shell_data *sh, char *line);

/* argv/redirect assembly */

int					append_word(char ***argv, int *argc, char *w);
int					add_redir(t_redir **lst, t_redir *node);
t_rdrtype			map_rdr(t_token_type tt);
int					is_redir_tok(t_token *token);
int					argv_len(char **vector);
void				mask_quoted_stars(char *s);
char				*expand_token_value(t_shell_data *sh, t_token *tok);
int					append_word_simple(char ***argv, char *word_copy);
t_node				*parse_factor(t_shell_data *sh, t_tokarr *ta);

/* parser pieces */
t_node				*parse_group(t_shell_data *sh, t_tokarr *ta);
t_node				*parse_command(t_shell_data *sh, t_tokarr *ta);
t_node				*parse_pipeline(t_shell_data *sh, t_tokarr *ta);
t_node				*parse_and_or(t_shell_data *sh, t_tokarr *ta);
t_node				*pipeline_syntax_err(void);

/* redirections + path + exec */

int					apply_all_redirs(t_cmd *cmd, int *fdin, int *fdout);
char				*find_in_path(t_shell_data *sh, const char *cmd);
int					run_exec_node(t_shell_data *sh, t_node *n, int fds[2],
						int is_top);
int					run_pipe(t_shell_data *sh, t_node *n, int is_top);
int					run_node(t_shell_data *sh, t_node *n, int is_top);
int					wait_status(pid_t pid);
int					starts_command(t_token *t);
t_node				*pipeline_syntax_eof(void);
t_node				*pipeline_syntax_err(void);
int					right_is_redir_only(t_node *right);
pid_t				fork_left(t_shell_data *sh, t_node *node, int pipe_fds[2]);
pid_t				fork_right(t_shell_data *sh, t_node *node, int pipe_fds[2]);

/*Export*/
int					export_add_key(t_shell_data *sh, const char *key);
int					export_remove_key(t_shell_data *sh, const char *key);
int					export_contains_key(t_list *lst, const char *key);
int					print_export_list(t_shell_data *sh);

/*Execution*/
void				print_cmd_not_found(const char *name);
char				*resolve_program_path(t_shell_data *sh, const char *name);
void				apply_dup_and_close(int from_fd, int to_fd);
void				close_pair_if_set(int fds[2]);
int					exec_external(t_shell_data *sh, t_cmd *cmd);
void				free_argv_vec(char **argv);
void				restore_masked_stars(char *s);
void				child_exec(t_shell_data *sh, t_node *node, int in_fd,
						int out_fd);
void				expand_argv_inplace(t_cmd *cmd);
int					exec_builtin_in_parent(t_shell_data *sh, t_cmd *cmd);
int					prepare_heredocs_tree(t_node *n, t_shell_data *sh);

// utils env helpers (header)
char				*env_get(t_list *env, const char *key);
int					env_set(t_shell_data *sh, const char *key,
						const char *value);
char				**env_list_to_array(t_list *env);

// String Util
void				ft_array_free(char **array);
char				*ft_str3var(const char *s1, const char *s2, const char *s3);

/*Build Tree Utils*/
void				free_string_array(char **argv);
void				free_command_node(t_cmd *cmd);
void				free_subtrees(t_node *node);
void				free_tree(t_node *node);

/*Expand Utils*/
int					has_star(const char *pattern);
int					glob_match(const char *pattern, const char *name);
int					should_skip_hidden(const char *pattern, const char *name);
char				*exp_join_n(char *acc, const char *s, size_t n);
char				*exp_join(char *acc, const char *s);
size_t				exp_scan_name(const char *line, size_t pos);
char				*exp_status_str(t_shell_data *sh);
char				*exp_env_value_dup(t_shell_data *sh, const char *line,
						size_t *i);
int					build_match_list(const char *pattern, char ***out_vec,
						int *out_cnt);

/*Utils File Directory*/
void				fdpack_init(t_fdpack *p);
void				fd_apply_inout(t_fdpack *p);
void				fd_restore(t_fdpack *p);
int					open_infile(const char *path);
int					open_out_trunc(const char *path);
int					open_out_append(const char *path);
int					open_heredoc_fd(t_redir *redir);
int					hd_open_pipe(int pfd[2]);
int					hd_loop(t_shell_data *sh, t_redir *rd, int wfd);
int					hd_loop_tty(t_shell_data *sh, t_redir *rd, int wfd);
#endif
