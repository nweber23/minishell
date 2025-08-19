NAME        = minishell

SRC_DIR     = srcs
INC_DIR     = includes
LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

SRCS =

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
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re libft