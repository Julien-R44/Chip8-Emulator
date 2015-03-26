NAME	= chip8
DEBUG	= yes
ifeq ($(DEBUG),no)
	CFLAGS=-Wall -Wextra -Werror
else
	CFLAGS= -g
endif
LDFLAGS	= -I./inc/
LFLAGS	= -lSDL
SRC		= \
			main.c
OBJ		= $(SRC:.c=.o)
SRCDIR	= ./src/
OBJDIR	= ./obj/
INCDIR	= ./include/
SRCS	= $(addprefix $(SRCDIR), $(SRC))
OBJS	= $(addprefix $(OBJDIR), $(OBJ))
INCS	= $(addprefix $(INCDIR), $(INC))

.SILENT:

all: $(NAME)

$(NAME): $(OBJS) $(INCS)
	gcc $(CFLAGS) -o $@ $^ $(LFLAGS)
	echo "\\033[1;32mSuccess.\\033[0;39m"

$(OBJS): $(SRCS)
ifeq ($(DEBUG),yes)
	echo "\\033[1;31mDEBUG COMPILATION.. (no flags except -g)\\033[0;39m"
else
	echo "\\033[1;31mCompilation with -Wall -Wextra -Werror...\\033[0;39m"
endif
	echo "\\033[1;34mGenerating objects... Please wait.\\033[0;39m"
	gcc $(CFLAGS) -c $(SRCS) $(LDFLAGS)
	mkdir -p $(OBJDIR)
	mv $(OBJ) $(OBJDIR)

.PHONY: clean fclean re

clean:
	echo "\\033[1;34mDeleting objects...\\033[0;39m"
	rm -f $(OBJS)

fclean: clean
	echo "\\033[1;34mDeleting $(NAME)\\033[0;39m"
	rm -f $(NAME)

re: fclean all
