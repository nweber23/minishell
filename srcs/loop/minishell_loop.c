/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:48:54 by nweber            #+#    #+#             */
/*   Updated: 2025/09/11 18:12:09 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_interactive(void)
{
    return (isatty(STDIN_FILENO) != 0);
}

// Read a line from stdin without echo/prompt; strip trailing '\n'
static char	*read_line_noninteractive(void)
{
    char	buffer[4096];
    char	*s;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return (NULL);
    s = ft_strdup(buffer);
    if (!s)
        return (NULL);
    if (s[0] != '\0')
    {
        size_t n = ft_strlen(s);
        if (n > 0 && s[n - 1] == '\n')
            s[n - 1] = '\0';
    }
    return (s);
}

static void	setup_readline_tty_once(void)
{
    extern FILE *rl_outstream;
    static int done = 0;
    static FILE *tty = NULL;

    if (done) return;
    done = 1;
    if (is_interactive())
    {
        tty = fopen("/dev/tty", "w");
        if (tty)
            rl_outstream = tty;
        else
            rl_outstream = stderr;
    }
}

int	minishell_loop(t_shell_data *shell, char **envp)
{
    reset_shell(shell);

    if (is_interactive())
        interavtive_signals();
    else
        init_signals();

    input(shell);
    if (is_interactive())
    {
        setup_readline_tty_once();
        shell->input = readline(shell->cwd);
        if (shell->input && shell->input[0] != '\0')
            add_history(shell->input);
    }
    else
        shell->input = read_line_noninteractive();
    if (shell->input && validate_input(shell))
        return (free_shell(shell), minishell_loop(shell, envp));
    if (shell->input == NULL)
    {
        int code = exit_code(-1);
        if (is_interactive())
            exit_msg();
        return (free_shell(shell), code);
    }
    if (!shell->trimmed || shell->trimmed[0] == '\0')
        return (free_shell(shell), minishell_loop(shell, envp));

    if (!ft_strcmp(shell->trimmed, "exit"))
    {
        if (is_interactive())
            exit_msg();
        return (free_shell(shell), exit_code(-1));
    }
    lexer(shell, shell->trimmed);
    shell->env_array = env_list_to_array(shell->env);
    if (shell->root)
    {
        free_tree(shell->root);
        shell->root = NULL;
    }
    shell->root = build_tree(shell, shell->tokens);
    exec_line(shell, shell->root);
    free_shell(shell);
    return (minishell_loop(shell, envp));
}

int	end_process(int value)
{
	if (value == -1)
		return (exit_code(-1));
	return (exit_code(value));
}

void	input(t_shell_data *shell)
{
	char	*cwd;
	char	*prompt1;
	char	*prompt2;

	if (!isatty(STDIN_FILENO))
	{
		shell->cwd = NULL;
		return ;
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		error_malloc("input", shell);
	prompt1 = ft_strjoin("STARSHELL$ ", cwd);
	free(cwd);
	if (!prompt1)
		error_malloc("input", shell);
	prompt2 = ft_strjoin(prompt1, ">");
	free(prompt1);
	if (!prompt2)
		error_malloc("input", shell);
	shell->cwd = prompt2;
}
