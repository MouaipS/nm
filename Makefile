NAME = ft_nm
CC = cc
CFLAGS = -Wall -Wextra -Werror -I include

SRCDIR = src
OBJDIR = objs

SRC = $(SRCDIR)/main.c

OBJ = $(SRCS:.c=.o)
SRCS = $(addprefix $(SRC_DIR), $(SRC))
OBJS = $(addprefix $(OBJ_DIR), $(OBJ))


all:$(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR):
	mkdir -p obj

obj/%.o : src/%.c $(HEADER) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -c -o $@

fclean: clean
	rm -f $(NAME)	

clean:
	rm -f $(OBJS) 

re: fclean all

.PHONY: all fclean clean re