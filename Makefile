MAKEFLAGS  += -s
NAME        = minishell

SRC_DIR     = srcs
INC_DIR     = includes
LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

PARSING_SRCS = parsing/check_parenthesis.c parsing/check_pipe.c parsing/check_quote.c parsing/check_quotes.c \
				parsing/check_redirect.c parsing/handling.c parsing/parenthesis_handling.c parsing/redirect.c \
				parsing/tokenizer.c parsing/utils.c parsing/validate_input.c parsing/wildcard.c
UTILS_SRCS = utils/checker.c
CLEANUP_SRCS = cleanup/error_exit.c cleanup/error_free.c cleanup/error_print.c cleanup/free_binary_tree.c
LOGIC_TREE_SRCS = logic_tree/get_node.c logic_tree/logic_tree_parenthesis.c logic_tree/logic_tree.c \
				logic_tree/logic_tree_utils.c
BINARY_TREE_SRCS = binary_tree/binary_tree.c binary_tree/binary_tree_utils.c binary_tree/binary_tree_getter.c
LOOP_SRCS = loop/minishell_loop.c loop/loop_utils.c
SIGNAL_SRCS = signal/signal.c signal/signal1.c

SRCS = $(addprefix $(SRC_DIR)/, $(PARSING_SRCS) $(UTILS_SRCS) $(CLEANUP_SRCS) $(LOGIC_TREE_SRCS) $(BINARY_TREE_SRCS) $(SIGNAL_SRCS) $(LOOP_SRCS) main.c)

OBJ_DIR = obj
OBJ = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(INC_DIR) -I $(LIBFT_DIR)/includes

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -lncurses -o $(NAME)
	@echo "\033[1;33m"
	@echo "                      A long time ago in a galaxy far, far away..." ; sleep 0.6
	@echo "" ; sleep 0.3
	@echo "                                   EPISODE III" ; sleep 0.4
	@echo "                         THE PREQUEL: REVENGE OF THE BUILD" ; sleep 0.5
	@echo "" ; sleep 0.4
	@echo "              A disturbance ripples through the make system." ; sleep 0.6
	@echo "              Ancient rules and flags awaken as compilation begins." ; sleep 0.6
	@echo "              The developers prepare the artifacts for deployment..." ; sleep 0.8
	@echo "" ; sleep 0.4
	@echo "              In the shadows of stale branches and forgotten commits," ; sleep 0.6
	@echo "              a new pipeline stirs. Compiler warnings gather like storm clouds," ; sleep 0.6
	@echo "              and linkers whisper of missing symbols across the void." ; sleep 0.6
	@echo "              Brave maintainers, armed with patches and coffee, converge" ; sleep 0.6
	@echo "              to tame the wild dependencies and restore order to headers." ; sleep 0.7
	@echo "" ; sleep 0.4
	@echo "              Tests tremble as the CI awakens. Memory leaks flee before" ; sleep 0.6
	@echo "              the ordered onslaught of valgrind and careful review." ; sleep 0.6
	@echo "              The makefile, once humble, now becomes a beacon —" ; sleep 0.6
	@echo "              summoning object files, archives, and the final executable." ; sleep 0.7
	@echo "" ; sleep 0.4
	@echo "              Yet ancient bugs linger in the log; only through teamwork" ; sleep 0.6
	@echo "              and many small commits will the build find true balance." ; sleep 0.6
	@echo "\033[0m" ; sleep 0.4
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⢿⡤⠤⠬⣉⣉⣿⣿⣆⣀⣠⣤⡀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣶⣦⡄⢠⡄⠀⠀⠀⠀⠀⣤⢶⡿⣾⣿⣿⡿⡷⢦⣾⠿⠿⠛⠻⣿⡿⠆⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣎⡒⠁⢠⢊⠕⠛⠭⣑⠶⠶⣭⣠⠍⣓⠉⠀⠀⣩⣴⠟⠁⠀⠀⠀⠀⠈⠃⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⢲⣒⣄⡀⠀⠀⠀⠀⠀⠀⠀⢇⣇⣀⡏⠜⠂⠤⡰⠁⣰⡿⣿⣿⣽⡔⠁⣠⡾⠋⠁⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡞⡰⠁⡠⢒⡭⢕⡢⢄⣠⣤⠒⣭⣵⣞⢶⣯⣝⣒⣤⡥⢤⣿⣿⣿⣿⡿⣿⣿⢿⡻⣽⢿⣿⣽⢲⡦⣤⣀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢯⣇⢰⢡⠧⡀⣠⠞⣩⣭⣶⣭⡒⢭⡿⣿⣞⠛⣯⣷⣾⣿⣿⣿⣟⣿⣿⣽⡞⣿⡱⣏⣻⣭⣷⡻⠾⣿⣿⣷⣄⣴⣤⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢉⠟⢼⡀⢠⠃⣼⣿⣿⣿⣿⣷⠀⠳⡻⡟⣾⣮⡻⢿⣯⣿⣿⣿⠳⣿⣿⣿⣯⠟⠛⠉⠉⠀⠀⠀⠈⠉⠁⠉⢿⡿⠂"; sleep 0.01
	@echo "⠀⠀⡀⠀⠀⠀⠀⢀⣀⠤⡔⡲⠽⢦⣀⠏⠺⣄⣿⣷⣾⣷⣷⣿⣧⣀⠙⠤⣟⣛⣛⣶⣙⣛⡭⠕⠊⠋⢻⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⢰⣽⠿⣷⡴⣶⣭⣷⣾⣭⡿⠈⢦⣀⣷⣄⢠⣾⣿⣿⣿⣿⣿⣿⣷⣽⣯⣶⣦⣄⡀⠀⠉⠹⢦⣴⣠⡀⡀⡅⠹⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠘⠷⡀⢈⣣⡻⣜⣫⡁⠀⠀⢀⣀⣩⣭⣼⣾⡿⣟⢿⡯⢿⣳⣟⣾⡽⣏⢿⡼⣝⣳⢖⡤⣀⠀⠈⠑⠧⣁⢀⠂⠙⠱⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠹⣳⣿⣿⣮⣽⠶⠾⠛⠛⣩⣟⣟⡳⣭⢛⡿⢾⣹⢟⣽⣯⣿⣽⢾⠧⢟⣭⢯⢟⡾⣽⣫⢶⢤⣄⡀⠙⢣⡦⠖⢋⡥⢭⡓⢄⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠉⠉⣮⣒⡶⡄⢀⣴⡺⣭⠷⣎⢷⣱⠯⠚⣁⣼⡾⣿⣿⣷⣿⣿⠀⠀⠀⠉⠙⠚⠵⢏⣯⡷⡾⣽⠛⠉⠑⠢⣌⠛⢏⠈⠳⠙⢆⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⢿⠟⢯⣵⣿⣳⣽⢲⡏⠟⠚⣁⣴⠾⠛⠛⠿⣿⣷⣿⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠛⢿⣶⣤⣀⠀⠑⢄⠃⠀⠀⡠⠃⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⢠⡞⠉⢷⣯⠞⡺⠕⣀⣴⠾⠋⠁⠀⠀⠀⠀⠈⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠷⠦⠵⠖⠋⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠈⠳⡀⠈⢯⣩⣴⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠙⣖⣭⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠻⢿⡿⡇⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣜⣽⣾⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠋⠙⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo ""; sleep 0.4
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣤⡶⠤⢤⠤⢤⣄⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠠⠔⠚⠋⢩⡇⡇⠘⠈⠀⠐⢼⠀⢠⣯⡉⡽⡤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⣴⣾⡅⢀⣄⠀⠀⠀⡘⣰⠁⢀⠀⡇⠀⠘⠀⠀⠃⠀⠀⢃⠀⠑⠢⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠸⢿⡷⢹⠀⠀⠔⡆⠀⡆⠁⡌⢸⠈⠀⢻⠃⠀⡄⠃⢰⠄⣆⣾⢠⠀⠀⠀⠉⠂⢄⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⢀⢔⠌⠉⠉⣉⠤⠰⣙⡻⡟⢿⡄⠀⠀⠃⠤⠤⠀⢠⡟⠠⠤⠈⠀⣀⣇⣀⡈⣇⣸⠉⡌⢸⠓⠆⠙⠀⠃⡆⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⢰⢒⣅⣀⣥⣭⡉⡁⡷⢼⣷⣷⣨⢌⡇⠀⠀⠀⠀⠀⠀⣈⣡⣶⠀⠀⠀⠀⠀⠀⠀⢸⡮⣸⠇⠈⠐⠒⢠⠤⢤⢜⡀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠓⣿⠛⠻⠛⣛⡭⢣⡸⢎⡙⡟⢁⣀⣷⣸⣆⣀⣰⠄⠀⡴⠘⣿⠁⠀⠀⠀⠀⠀⠀⠘⡯⡟⣠⡀⠀⡐⠀⡄⠈⠚⠃⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⢠⣼⣿⢿⣿⡿⣿⣿⣿⠋⢻⢟⣯⢽⣱⣶⣶⣶⣾⣾⣯⣭⣤⣇⠉⠯⢭⠭⠭⠛⠓⢒⣿⣃⡾⠥⠄⣛⣃⢿⣿⣿⠸⡀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠈⠚⠻⠅⠬⠟⠻⠿⠋⠙⠛⢛⣿⣿⣿⢿⡿⠿⣛⠻⡻⣿⣿⣯⡽⠦⢸⢸⢸⣖⣒⡴⠿⠿⠿⣿⣿⣿⣷⢶⠒⠃⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⣿⣿⣿⣱⡯⠚⠻⢮⢳⠇⠁⣟⢿⡦⣥⣧⣯⢭⣯⡟⡉⣷⢙⣟⠶⠶⠛⣧⢸⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢂⡽⣻⡷⡎⢟⢶⣏⣒⣻⡶⢀⡬⣹⣿⣿⣿⣿⣷⣷⣁⠀⣾⣾⣿⢋⣷⡤⢾⠎⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡸⢿⡝⣅⡶⢧⠒⡺⠳⠧⠇⠀⠀⠀⠉⠙⢿⣿⡿⠿⢿⢷⣡⢿⠁⣶⣎⡸⠀⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢣⣷⣱⣾⢠⡳⡹⠁⠀⠀⠀⠀⠀⠀⠀⠀⢰⢸⠀⠑⠈⡏⠉⠟⡏⣿⢉⡅⡇⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⡻⠯⣻⣵⣱⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⣄⠀⠀⢐⡇⠀⠸⣹⡽⣾⣽⡇⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⢧⡇⢀⡓⠎⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⢠⡀⠀⠀⢑⣾⡄⢏⢣⠀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢴⢞⡿⠗⠾⡜⣠⠤⠼⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣠⠦⡆⠀⠀⠀⠸⣧⣧⣘⡾⡀⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣻⣋⣐⣤⢸⣻⢹⠠⢳⠁⠀⠀⠀⠀⠀⠀⠀⠀⢰⢹⡿⠄⡇⡇⠀⠀⠀⣻⣟⣨⠙⠁⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⠃⠹⡖⢪⠃⢳⡯⡆⠰⣻⠀⠀⠀⠀⠀⠀⠀⠀⠀⡟⠟⠰⢿⠁⠀⠀⠀⢱⠷⢭⡝⢱⠀⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⡏⠠⡰⠀⡆⠀⠈⣷⢱⠄⠁⢆⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢸⠀⠀⠀⠀⠈⣷⠀⢰⠘⡄⠀⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠀⠀⣮⣿⡚⡌⠇⡔⡄⠀⠀⡹⣮⠒⣬⣩⢢⠀⠀⠀⠀⠀⠀⡜⡷⡇⣀⢺⡆⠀⠀⠀⠀⢿⡤⠶⣾⢛⡄⠀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⠠⢮⣻⣷⣹⡰⢷⣮⠇⠀⠀⢧⣇⣿⣓⣟⣿⡧⠀⠀⠀⠀⢠⢿⣷⡻⠺⣼⣇⠀⠀⠀⠈⣽⣗⠺⢺⣿⣗⡀⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⡿⣲⡟⠚⠚⠛⢻⡃⡆⠀⠀⣾⣿⡟⠒⠻⠛⡿⡇⠀⠀⠀⣾⣗⠓⠒⡚⢺⣽⡀⠀⠀⢸⣗⣽⢛⠓⠚⣿⣡⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⠀⢉⠏⠀⠀⠀⠀⠸⣉⠁⠀⠀⡉⠇⠤⠥⣀⡇⠯⡁⠀⠀⠀⡩⠇⠀⠀⢇⣸⡯⡀⠀⠀⢈⡽⠐⠁⠀⠀⠸⢅⠀⠀"; sleep 0.01
	@echo "					⠀⠀⠀⠀⠀⢸⠁⢀⣀⡠⠴⢆⠤⢄⡇⠀⢸⠍⠩⠍⠤⠐⢧⠂⢽⠀⠀⢸⣄⠀⠀⠄⢂⣷⣳⢥⠀⠀⡿⠿⡹⠥⠤⠐⡟⠣⠇⠀"; sleep 0.01
	@echo "					⠀⠀⠀⢠⣔⣊⡢⠤⠤⠤⠴⣮⣐⣠⠼⠻⡒⠚⠒⠒⠒⠚⠐⠁⠘⠒⠻⠄⠘⠒⠒⠒⠚⠳⠬⠗⠺⠬⠤⠑⠒⠒⠒⠚⠷⠧⠾⠕"; sleep 0.01
	@echo ""; sleep 0.4
	@echo "\033[1;31m"
	@echo " ▄█     █▄     ▄████████  ▄█        ▄████████  ▄██████▄     ▄▄▄▄███▄▄▄▄      ▄████████            "; sleep 0.1
	@echo "███     ███   ███    ███ ███       ███    ███ ███    ███  ▄██▀▀▀███▀▀▀██▄   ███    ███            "; sleep 0.1
	@echo "███     ███   ███    █▀  ███       ███    █▀  ███    ███  ███   ███   ███   ███    █▀             "; sleep 0.1
	@echo "███     ███  ▄███▄▄▄     ███       ███        ███    ███  ███   ███   ███  ▄███▄▄▄                "; sleep 0.1
	@echo "███     ███ ▀▀███▀▀▀     ███       ███        ███    ███  ███   ███   ███ ▀▀███▀▀▀                "; sleep 0.1
	@echo "███     ███   ███    █▄  ███       ███    █▄  ███    ███  ███   ███   ███   ███    █▄             "; sleep 0.1
	@echo "███ ▄█▄ ███   ███    ███ ███▌    ▄ ███    ███ ███    ███  ███   ███   ███   ███    ███            "; sleep 0.1
	@echo " ▀███▀███▀    ██████████ █████▄▄██ ████████▀   ▀██████▀    ▀█   ███   █▀    ██████████            "; sleep 0.1
	@echo ""; sleep 0.4
	@echo "                        ███      ▄██████▄                                                         "; sleep 0.1
	@echo "                    ▀█████████▄ ███    ███                                                        "; sleep 0.1
	@echo "                       ▀███▀▀██ ███    ███                                                        "; sleep 0.1
	@echo "                        ███   ▀ ███    ███                                                        "; sleep 0.1
	@echo "                        ███     ███    ███                                                        "; sleep 0.1
	@echo "                        ███     ███    ███                                                        "; sleep 0.1
	@echo "                        ███     ███    ███                                                        "; sleep 0.1
	@echo "                       ▄████▀    ▀██████▀                                                         "; sleep 0.1
	@echo ""; sleep 0.4
	@echo "   ▄▄▄▄███▄▄▄▄    ▄█  ███▄▄▄▄    ▄█     ▄████████    ▄█    █▄       ▄████████  ▄█        ▄█       "; sleep 0.1
	@echo " ▄██▀▀▀███▀▀▀██▄ ███  ███▀▀▀██▄ ███    ███    ███   ███    ███     ███    ███ ███       ███       "; sleep 0.1
	@echo " ███   ███   ███ ███▌ ███   ███ ███▌   ███    █▀    ███    ███     ███    █▀  ███       ███       "; sleep 0.1
	@echo " ███   ███   ███ ███▌ ███   ███ ███▌   ███         ▄███▄▄▄▄███▄▄  ▄███▄▄▄     ███       ███       "; sleep 0.1
	@echo " ███   ███   ███ ███▌ ███   ███ ███▌ ▀███████████ ▀▀███▀▀▀▀███▀  ▀▀███▀▀▀     ███       ███       "; sleep 0.1
	@echo " ███   ███   ███ ███  ███   ███ ███           ███   ███    ███     ███    █▄  ███       ███       "; sleep 0.1
	@echo " ███   ███   ███ ███  ███   ███ ███     ▄█    ███   ███    ███     ███    ███ ███▌    ▄ ███▌    ▄ "; sleep 0.1
	@echo "  ▀█   ███   █▀  █▀    ▀█   █▀  █▀    ▄████████▀    ███    █▀      ██████████ █████▄▄██ █████▄▄██ "; sleep 0.1
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