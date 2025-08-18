NAME        = minishell
SRC         =
OBJ_DIR     = obj
OBJ         = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

.SILENT:

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\033[1;31m"
	@echo ".------..------..------..------..------..------..------..------."
	@echo "|M.--. ||I.--. ||N.--. ||I.--. ||H.--. ||E.--. ||L.--. ||L.--. |"
	@echo "| (\/) || (\/) || :(): || (\/) || :/\: || (\/) || :/\: || :/\: |"
	@echo "| :\/: || :\/: || ()() || :\/: || (__) || :\/: || (__) || (__) |"
	@echo "| '--'M|| '--'I|| '--'N|| '--'I|| '--'H|| '--'E|| '--'L|| '--'L|"
	@echo "'------''------''------''------''------''------''------''------'"
	@echo "                        MINIHELL"
	@echo "\033[0m"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re