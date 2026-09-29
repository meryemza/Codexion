NAME = codexion
SRC = coders/coders_monitor.c \
	coders/heap.c coders/helper_init.c coders/init.c coders/main.c \
	coders/parsing.c coders/routine.c coders/clean.c coders/util.c \
	coders/util2.c

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread 

RM = rm -f

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME): $(OBJ) coders/codexion.h
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY : all clean fclean remak
