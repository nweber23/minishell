MAKEFLAGS  += -s
NAME        = minishell

SRC_DIR     = srcs
INC_DIR     = includes
LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

PARSING_SRCS = parsing/check_parenthesis.c parsing/check_pipe.c parsing/check_quote.c parsing/check_quotes.c \
				parsing/check_redirect.c parsing/handling.c parsing/parenthesis_handling.c parsing/redirect.c \
				parsing/tokenizer.c parsing/utils.c parsing/validate_input.c parsing/wildcard.c parsing/token_state.c
UTILS_SRCS = utils/checker.c utils/utils_env.c utils/utils_string.c
CLEANUP_SRCS = cleanup/error_exit.c cleanup/error_free.c cleanup/error_print.c cleanup/free_binary_tree.c cleanup/free_shell.c
LOGIC_TREE_SRCS = logic_tree/get_node.c logic_tree/logic_tree_parenthesis.c logic_tree/logic_tree.c \
				logic_tree/logic_tree_utils.c
BINARY_TREE_SRCS = binary_tree/binary_tree.c binary_tree/binary_tree_utils.c binary_tree/binary_tree_getter.c \
				binary_tree/get_infile.c binary_tree/get_outfile.c
LOOP_SRCS = loop/minishell_loop.c loop/loop_utils.c
SIGNAL_SRCS = signal/signal.c signal/signal1.c
BUILTIN_SRCS = builtins/bi_cd.c builtins/bi_echo.c builtins/bi_env.c builtins/bi_exit.c builtins/bi_export.c \
				builtins/bi_pwd.c builtins/bi_unset.c
EXECUTION_SRCS = execution/exec_ast_nodes.c  execution/exec_tokstream.c execution/exec_builtin_dispatch.c \
				execution/exec_parse_group.c execution/exec_parse_pipeline.c execution/exec_parse_andor.c \
				execution/exec_parse_command.c execution/exec_argv_redir.c execution/exec_redirs.c\
				execution/exec_heredoc.c execution/exec_path.c execution/exec_expand.c execution/exec_build_tree.c\
				execution/exec_run_exec.c execution/exec_run_pipe.c execution/exec_run_node.c execution/exec_expand_line.c

SRCS = $(addprefix $(SRC_DIR)/, $(PARSING_SRCS) $(UTILS_SRCS) $(CLEANUP_SRCS) $(LOGIC_TREE_SRCS) $(BINARY_TREE_SRCS) $(SIGNAL_SRCS) $(LOOP_SRCS) $(BUILTIN_SRCS) $(EXECUTION_SRCS) main.c)

OBJ_DIR = obj
OBJ = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(INC_DIR) -I $(LIBFT_DIR)/includes

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -lncurses -o $(NAME)
	

$(LIBFT):
	@$(MAKE) -s --no-print-directory -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -s --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -s --no-print-directory -C $(LIBFT_DIR) fclean

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re libft