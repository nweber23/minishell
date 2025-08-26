// A stronger validation tester for minishell parsing
// - Exercises many tricky edge cases
// - Verifies that validate_input agrees with the individual checks
// - Captures and classifies error messages from validate_input

#include "minishell.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>

// ------------------------ small helpers ------------------------

static bool is_only_space(const char *s) {
    if (!s) return true;
    while (*s) {
        if (!isspace((unsigned char)*s)) return false;
        s++;
    }
    return true;
}

static const char *b2s(bool b) { return b ? "true" : "false"; }

static void str_trim_trailing(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[n-1] = '\0';
        n--;
    }
}

static void print_header(void) {
    printf("MINISHELL VALIDATION TESTER (extended)\n");
    printf("======================================\n\n");
    printf("Note: overall_ok = !validate_input(shell) (your API returns false on success, true on error)\n");
    printf("This tester also:\n");
    printf("- Predicts overall_ok from individual checks and flags aggregation mismatches\n");
    printf("- Captures stderr from validate_input and classifies the error type\n\n");
}

// naive classifier for the captured error message
typedef enum e_err_kind {
    ERR_NONE,
    ERR_PIPE,
    ERR_QUOTE,
    ERR_REDIR_NEWLINE,
    ERR_REDIR_TOKEN,
    ERR_PAREN_OPEN,
    ERR_PAREN_CLOSE,
    ERR_SYNTAX_GENERIC
} t_err_kind;

static t_err_kind classify_error_msg(const char *msg) {
    if (!msg || !*msg) return ERR_NONE;

    // Quote-related
    if (strstr(msg, "unexpected EOF while looking for matching"))
        return ERR_QUOTE;

    // Parentheses
    if (strstr(msg, "expected `)'")) // OPEN_MSG
        return ERR_PAREN_OPEN;
    if (strstr(msg, "near unexpected token `)'")) // CLOSE_MSG
        return ERR_PAREN_CLOSE;

    // Redirections
    if (strstr(msg, "near unexpected token `newline'"))
        return ERR_REDIR_NEWLINE;

    // Pipe or generic token error; check for backticked token presence
    if (strstr(msg, "near unexpected token")) {
        if (strstr(msg, "`|'")) return ERR_PIPE;
        return ERR_REDIR_TOKEN; // Often redir errors fall here too
    }

    // Fallback
    if (strstr(msg, "syntax error"))
        return ERR_SYNTAX_GENERIC;

    return ERR_NONE;
}

// capture stderr around validate_input call
static size_t capture_vi_stderr(t_shell_data *sh, char *buf, size_t cap, bool *overall_ok) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    fflush(NULL);

    int olderr = dup(STDERR_FILENO);
    if (olderr == -1) {
        perror("dup");
        exit(1);
    }
    if (dup2(pipefd[1], STDERR_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
    close(pipefd[1]);

    // call validate_input
    bool vi_ret = validate_input(sh);
    if (overall_ok) *overall_ok = !vi_ret;

    fflush(NULL);
    // restore stderr
    if (dup2(olderr, STDERR_FILENO) == -1) {
        perror("dup2 restore");
        exit(1);
    }
    close(olderr);

    // read the captured message
    ssize_t r = read(pipefd[0], buf, cap ? (ssize_t)(cap - 1) : 0);
    if (r < 0) r = 0;
    if (cap) buf[r] = '\0';
    close(pipefd[0]);
    str_trim_trailing(buf);
    return (size_t)r;
}

// ------------------------ test driver ------------------------

typedef struct s_case {
    const char *name;
    const char *input;
} t_case;

static void run_one(const t_case *tc) {
    t_shell_data sh;
    memset(&sh, 0, sizeof(sh));
    sh.input = ft_strdup(tc->input ? tc->input : "");
    if (!sh.input) {
        fprintf(stderr, "alloc failed\n");
        exit(1);
    }

    const char *s = sh.input;
    bool call_funcs = (s && s[0] != '\0');

    // individual checks
    bool pipe_ok = true, quotes_ok = true, quote_pos_ok = true, redirect_ok = true, paren_ok = true;
    if (call_funcs) {
        pipe_ok = pipe_check((char *)s);
        quotes_ok = quote_check((char *)s);
        quote_pos_ok = quote_position_check((char *)s);
        redirect_ok = redirect_check((char *)s);
        paren_ok = parenthesis_check((char *)s);
    }

    // predict overall_ok from individual checks, but allow empty/whitespace as ok
    bool predicted_ok = is_only_space(s) ? true
                         : (pipe_ok && quotes_ok && quote_pos_ok && redirect_ok && paren_ok);

    // capture stderr from validate_input
    char errbuf[512];
    bool overall_ok = true;
    size_t nread = capture_vi_stderr(&sh, errbuf, sizeof(errbuf), &overall_ok);
    t_err_kind kind = classify_error_msg(nread ? errbuf : "");

    // print
    printf("--------------------------------------------------------------------------------\n");
    printf("TEST: %s\n", tc->name);
    printf("INPUT: %s\n", tc->input);
    printf("pipe=%s quotes=%s quote_pos=%s redirect=%s paren=%s | predicted_overall=%s | actual_overall=%s\n",
           b2s(pipe_ok), b2s(quotes_ok), b2s(quote_pos_ok), b2s(redirect_ok), b2s(paren_ok),
           b2s(predicted_ok), b2s(overall_ok));
    if (!overall_ok || nread) {
        const char *k =
            kind == ERR_PIPE ? "PIPE" :
            kind == ERR_QUOTE ? "QUOTE" :
            kind == ERR_REDIR_NEWLINE ? "REDIR(newline)" :
            kind == ERR_REDIR_TOKEN ? "REDIR(token)" :
            kind == ERR_PAREN_OPEN ? "PAREN(open)" :
            kind == ERR_PAREN_CLOSE ? "PAREN(close)" :
            kind == ERR_SYNTAX_GENERIC ? "SYNTAX" : "NONE";
        printf("stderr: [%s] %s\n", k, errbuf[0] ? errbuf : "(no message)");
    }

    // verdicts
    bool agg_match = (predicted_ok == overall_ok);
    printf("AGGREGATION: %s (validate_input should %s when any sub-check fails)\n",
           agg_match ? "OK" : "MISMATCH",
           "reject");

    if (overall_ok && nread) {
        printf("WARN: validate_input returned OK but still printed an error message\n");
    } else if (!overall_ok && !nread) {
        printf("WARN: validate_input returned error but printed no message\n");
    }

    // cleanup
    free(sh.trimmed);
    free(sh.input);
}

int main(void) {
    print_header();

    // New, extended edge cases (beyond your current suite)
    const t_case cases[] = {
        // Empty / whitespace / line breaks
        {"Empty", ""},
        {"Spaces only", "     "},
        {"Tabs only", "\t\t\t"},
        {"Trailing newline", "echo hi\n"},
        {"Newline only", "\n"},

        // Pipes and logical ops at edges
        {"Single pipe only", "|"},
        {"Double pipe only", "||"},
        {"Double amp only", "&&"},
        {"Pipe then cmd", "| ls"},
        {"Cmd then pipe", "ls |"},
        {"Triple pipe spaced", "a |  |  | b"},
        {"Pipe with quoted literal pipe", "echo '|' | cat"},
        {"Pipe adjacent no spaces", "a|b"},
        {"AND ok", "true && echo ok"},
        {"OR ok", "false || echo ok"},
        {"Leading AND", "&& echo ok"},
        {"Trailing AND", "echo ok &&"},
        {"Mix AND/OR malformed", "echo a &&& echo b"},
        {"Pipe before paren", "| (echo x)"},
        {"Pipe after paren", "(echo x) |"},

        // Quotes mixing and mismatches
        {"Unclosed single", "echo 'abc"},
        {"Unclosed double", "echo \"abc"},
        {"Crossed quotes", "echo \"a'b\"c\""},
        {"Balanced with metas inside", "echo \"(a|b) > x\""},
        {"Quote around pipe token", "\"|\""},
        {"Quote around paren tokens", "'( )'"},
        {"Empty quotes then pipe", "'' | echo x"},
        {"Only empty quotes", "''"},

        // Redirections: missing targets, bad tokens, and valid quoted targets
        {"Output missing target", "echo >"},
        {"Input missing target", "cat < "},
        {"Append missing target", "echo >>   "},
        {"Heredoc missing limiter", "cat <<"},
        {"Redir followed by pipe", "echo > | file"},
        {"Redir followed by >", "echo > > out"},
        {"Redir followed by <", "echo > < out"},
        {"Redir followed by )", "echo > )"},
        {"Redir followed by (", "echo > ("},
        {"Valid quoted target (single)", "echo > 'my file'"},
        {"Valid quoted target (double)", "echo > \"my file\""},
        {"Redir to empty quoted name", "echo > ''"},
        {"Heredoc with quoted limiter", "cat << 'EOF'"},
        {"Heredoc limiter is pipe", "cat << |"},
        {"Chain redirs ok", "cat < in > out"},
        {"Weird flip ><", ">< file"},
        {"Triple >", "echo >>> file"},

        // Parentheses: positions and nesting
        {"Only opening parens", "(((("},
        {"Only closing parens", "))))"},
        {"Wrong order", ")echo("},
        {"Balanced simple", "(echo hi)"},
        {"Balanced nested", "((echo hi))"},
        {"Paren with pipe inside", "(a|b)"},
        {"Paren with trailing pipe", "(a | )"},
        {"Paren groups with AND/OR", "(a) && (b || c)"},
        {"Paren inside single quotes only", "echo '('"},
        {"Paren inside double quotes only", "echo \")\""},
        {"Empty parens", "()"},
        {"Parens around empty quotes", "('')"},
        {"Parens around redir missing target", "(echo > )"},

        // Mixed complex
        {"Pipe and unclosed quote", "echo \"hi |"},
        {"Paren and unclosed quote", "(echo 'hi)"},
        {"Redir and unclosed quote", "echo > \"file"},
        {"AND then bad redir", "true && echo > |"},
        {"OR then bad paren", "false || )"},
        {"Nested bad inside quotes should be ignored", "echo \"( ( > | )\""},
        {"Multiple meta collisions", "echo >>> | << < > ) ("},

        // Misc tokens many shells accept but your validators may treat as normal
        {"Semicolons sequence", "echo a;; echo b"},
        {"Ampersand mid", "echo a & echo b"},
        {"Pipe with tabs", "\techo\t|\tcat\t"},
    };

    const size_t N = sizeof(cases)/sizeof(cases[0]);
    for (size_t i = 0; i < N; ++i) {
        run_one(&cases[i]);
    }

    printf("================================================================================\n");
    printf("Legend:\n");
    printf("- predicted_overall comes from: (pipe && quotes && quote_pos && redirect && paren), with empty/whitespace allowed\n");
    printf("- actual_overall is derived from validate_input (negated return)\n");
    printf("- AGGREGATION MISMATCH suggests validate_input disagrees with its sub-checks or empty/whitespace handling\n");
    printf("- stderr classification uses includes/error.h strings (PIPE/QUOTE/REDIR/PAREN)\n");
    return 0;
}