NAME        = minishell

SRC_DIR     = srcs
INC_DIR     = includes
LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

PARSING_SRCS = $(wildcard $(SRC_DIR)/parsing/*.c)
UTILS_SRCS = $(wildcard $(SRC_DIR)/utils/*.c)
CLEANUP_SRCS = $(wildcard $(SRC_DIR)/cleanup/*.c)
BUILTINS_SRCS = $(wildcard $(SRC_DIR)/builtins/*.c)
EXECUTIONS_SRCS = $(wildcard $(SRC_DIR)/execution/*.c)
SRCS = $(PARSING_SRCS) $(UTILS_SRCS) $(CLEANUP_SRCS) $(BUILTINS_SRCS) $(EXECUTIONS_SRCS)

OBJ_DIR = obj
OBJ = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(INC_DIR) -I $(LIBFT_DIR)/includes


.SILENT:

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@echo "\033[1;31m"
	@echo ".------..------..------..------..------..------..------..------."
	@echo "|M.--. ||I.--. ||N.--. ||I.--. ||H.--. ||E.--. ||L.--. ||L.--. |"
	@echo "| (\/) || (\/) || :(): || (\/) || :/\: || (\/) || :/\: || :/\: |"
	@echo "| :\/: || :\/: || ()() || :\/: || (__) || :\/: || (__) || (__) |"
	@echo "| '--'M|| '--'I|| '--'N|| '--'I|| '--'H|| '--'E|| '--'L|| '--'L|"
	@echo "'------''------''------''------''------''------''------''------'"
	@echo "                        MINIHELL"
	@echo "\033[0m"

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
