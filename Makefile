CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = pipex

SRC = main.c \
	pipex_execution.c \
	utils1.c \
	utils2.c \
	utils3.c \
	init_pipex.c \
	pipex_cleanup.c \
	err_utils.c \
	io_and_proc.c \
	pipex_command_parser.c

OBJ = $(SRC:.c=.o)

INCLUDES = pipex.h

GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

all: $(NAME)

%.o: %.c $(INCLUDES)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW)Compiling:$(RESET) $<"

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)Build complete:$(RESET) $(NAME)"

clean:
	@rm -f $(OBJ)
	@echo "$(YELLOW)Cleaned object files$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)Cleaned executable$(RESET)"

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJ)
