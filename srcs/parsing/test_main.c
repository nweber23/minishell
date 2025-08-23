#include "minishell.h"
#include <stdio.h>
#include <string.h>

// Test helper functions
void print_token(t_token *token)
{
    const char *type_names[] = {
        "WORD", "PIPE", "HERE_DOC", "APPEND", "INFILE", "OUTFILE", "AND", "OR", "PARENTHESIS"
    };
    const char *state_names[] = {
        "SINGLE_Q", "DOUBLE_Q", "GENERAL", "EXPAND"
    };

    printf("Token[%d]: value='%s', type=%s, state=%s\n",
           token->position, token->value, type_names[token->type], state_names[token->state]);
}

void print_tokens(t_shell_data *shell)
{
    t_list *current = shell->tokens;
    printf("=== TOKENS ===\n");
    while (current)
    {
        print_token((t_token *)current->content);
        current = current->next;
    }
    printf("==============\n\n");
}

void free_tokens(t_shell_data *shell)
{
    t_list *current = shell->tokens;
    t_list *next;

    while (current)
    {
        next = current->next;
        if (current->content)
        {
            t_token *token = (t_token *)current->content;
            if (token->value)
                free(token->value);
            free(token);
        }
        free(current);
        current = next;
    }
    shell->tokens = NULL;
}

void test_case(const char *test_name, const char *input, t_shell_data *shell)
{
    printf("========================================\n");
    printf("TEST: %s\n", test_name);
    printf("INPUT: \"%s\"\n", input);
    printf("========================================\n");

    // Reset tokens
    free_tokens(shell);

    // Create tokens
    create_token(shell, (char *)input);

    // Print results
    print_tokens(shell);
}

int main(void)
{
    t_shell_data shell;
    shell.tokens = NULL;

    printf("MINISHELL PARSER TEST SUITE\n");
    printf("===========================\n\n");

    // Basic parenthesis tests
    test_case("Simple parentheses", "(echo hello)", &shell);
    test_case("Nested parentheses", "((echo hello))", &shell);
    test_case("Multiple parentheses", "(echo hello) && (echo world)", &shell);

    // Parentheses with operators
    test_case("Parentheses with pipe", "(echo hello | grep h)", &shell);
    test_case("Parentheses with AND", "(echo hello) && echo world", &shell);
    test_case("Parentheses with OR", "(echo hello) || echo world", &shell);
    test_case("Complex parentheses", "(echo hello | grep h) && (echo world)", &shell);

    // Unbalanced parentheses edge cases
    test_case("Opening parenthesis only", "(echo hello", &shell);
    test_case("Closing parenthesis only", "echo hello)", &shell);
    test_case("Multiple unbalanced", "((echo hello)", &shell);
    test_case("Wrong order", ")echo hello(", &shell);

    // Whitespace handling
    test_case("Spaces around parentheses", " ( echo hello ) ", &shell);
    test_case("No spaces", "(echo hello)", &shell);
    test_case("Mixed spaces", "( echo hello) &&(echo world )", &shell);
    test_case("Tabs and spaces", "\t(\techo\thello\t)\t", &shell);

    // Empty parentheses
    test_case("Empty parentheses", "()", &shell);
    test_case("Empty with spaces", "( )", &shell);
    test_case("Empty nested", "(())", &shell);

    // Quotes inside parentheses
    test_case("Single quotes in parentheses", "(echo 'hello world')", &shell);
    test_case("Double quotes in parentheses", "(echo \"hello world\")", &shell);
    test_case("Mixed quotes", "(echo 'hello' \"world\")", &shell);
    test_case("Parentheses in quotes", "echo '(hello)' \"(world)\"", &shell);

    // Redirection with parentheses
    test_case("Redirect before parentheses", "< input (cat)", &shell);
    test_case("Redirect after parentheses", "(cat) > output", &shell);
    test_case("Redirect inside parentheses", "(cat < input > output)", &shell);
    test_case("Here doc with parentheses", "(cat << EOF)", &shell);
    test_case("Append with parentheses", "(echo hello >> file)", &shell);

    // Complex combinations
    test_case("All operators", "(echo hello | grep h) && (echo world || echo fail) >> output", &shell);
    test_case("Deeply nested", "(((echo hello | grep h) && echo world) || echo fail)", &shell);
    test_case("Multiple commands", "(echo a; echo b) && (echo c | echo d)", &shell);

    // Edge cases with metacharacters
    test_case("Pipe before parentheses", "echo hello | (grep h)", &shell);
    test_case("AND after parentheses", "(echo hello) && echo world", &shell);
    test_case("OR between parentheses", "(echo hello) || (echo world)", &shell);

    // Special characters
    test_case("Dollar sign", "(echo $HOME)", &shell);
    test_case("Question mark", "(echo $?)", &shell);
    test_case("Wildcards", "(echo *)", &shell);
    test_case("Backslash", "(echo \\hello)", &shell);

    // Long commands
    test_case("Very long command", "(echo this is a very long command with many words and arguments)", &shell);

    // Multiple parentheses groups
    test_case("Sequential parentheses", "(echo hello) (echo world)", &shell);
    test_case("Parentheses with different operators", "(echo a) && (echo b) || (echo c) | (echo d)", &shell);

    // Error cases that should be handled gracefully
    test_case("Only opening parentheses", "((((", &shell);
    test_case("Only closing parentheses", "))))", &shell);
    test_case("Alternating", "()()()())", &shell);
    test_case("Malformed nesting", "(()()", &shell);

    // Real-world like examples
    test_case("Conditional execution", "(test -f file) && (cat file) || (echo 'file not found')", &shell);
    test_case("Background process simulation", "(sleep 5) && echo done", &shell);
    test_case("Directory operations", "(cd /tmp) && (ls -la) && (cd -)", &shell);

    // Stress tests
    test_case("Many nested levels", "((((((echo hello))))))", &shell);
    test_case("Many sequential", "(a)(b)(c)(d)(e)(f)(g)", &shell);

    // Clean up
    free_tokens(&shell);

    printf("========================================\n");
    printf("TEST SUITE COMPLETED\n");
    printf("========================================\n");

    return 0;
}