NAME =				ft_tar

SRC_FILES =			main.c
OBJ_FILES =			$(patsubst %.c,%.o,$(SRC_FILES))

SRC_DIR =			src/
OBJ_DIR =			obj/
INCLUDE_DIRS =		include/ libft/includes

SRC =				$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ =				$(addprefix $(OBJ_DIR), $(OBJ_FILES))

CC =				gcc
C_FLAGS =			-Wall -Wextra -Werror -g
INCLUDE_FLAGS =		$(addprefix -I , $(INCLUDE_DIRS))

RED =				\033[31m
GREEN =				\033[32m
BLUE =				\033[34m
YELLOW =			\033[33m
MAGENTA =			\033[35m
GREY =				\033[37m
GREEN_LIGHT =		\033[92m
YELLOW_LIGHT =		\033[93m
YELLOW_BOLD =		\033[1;33m
YELLOW_LIGHT_BOLD =	\033[1;93m
MAGENTA_LIGHT =		\033[95m
BLINK =				\033[5m
GREEN_LIGHT_BLINK =	\033[5;92m
END_COLOUR =		\033[0m

# .SILENT:
all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(YELLOW_LIGHT)$(NAME): $(YELLOW)Compiling $(NAME)...$(END_COLOUR)"
	$(CC) $(C_FLAGS) $^ -o $@
	@echo "$(YELLOW_LIGHT)$(NAME): $(GREEN)Successfully compiled $(NAME)!$(END_COLOUR)"

obj/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW_LIGHT)$(NAME): $(YELLOW)Cleaning objects...$(END_COLOUR)"
	@/bin/rm -rf $(OBJ_DIR)
	@echo "$(YELLOW_LIGHT)$(NAME): $(GREEN)Successfully cleaned all objects!$(END_COLOUR)"

fclean: clean
	@echo "$(YELLOW_LIGHT)$(NAME): $(YELLOW)Cleaning executable...$(END_COLOUR)"
	@/bin/rm -f $(NAME) debug
	@echo "$(YELLOW_LIGHT)$(NAME): $(GREEN)Successfully cleaned everything!$(END_COLOUR)"

re: fclean all

.PHONY: all force clean fclean re
