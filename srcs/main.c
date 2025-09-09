#include "minishell.h"

static void create_env_list(t_shell_data *sh, char **envp)
{
    int i = 0;

    sh->env = NULL;
    while (envp && envp[i])
    {
        char *eq = ft_strchr(envp[i], '=');
        if (eq)
        {
            size_t klen = (size_t)(ezq - envp[i]);
            char  *key  = ft_substr(envp[i], 0, klen);
            char  *val  = ft_strdup(eq + 1);
            if (!key || !val || env_set(sh, key, val) != 0)
                error_malloc("create_env_list", sh);
            free(key);
            free(val);
        }
        i++;
    }
}

static void bump_shlvl(t_shell_data *sh)
{
    const char *cur;
    int val;
    char *s;

    cur = env_get(sh->env, "SHLVL");
    if (cur == NULL)
        val = 0;
    else
        val = ft_atoi(cur);
    s = ft_itoa(val + 1);
    if (!s)
        error_malloc("bump_shlvl", sh);
    if (env_set(sh, "SHLVL", s) != 0)
        error_malloc("bump_shlvl:env_set", sh);
    free(s);
}

int main(int ac, char **av, char **envp)
{
    t_shell_data sh;

    (void)av;
    ft_memset(&sh, 0, sizeof(sh));
    if (ac > 1)
    {
        ft_putendl_fd("i dont want arguments", 2);
        return (EXIT_FAILURE);
    }
    create_env_list(&sh, envp);
    bump_shlvl(&sh);
    minishell_loop(&sh, envp);
    free_env(sh.env);
    return (0);
}
