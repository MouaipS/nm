NAME	= ft_nm

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -I include

SRCDIR	= src
OBJDIR	= obj

SRCS	= $(SRCDIR)/main.c \
		  $(SRCDIR)/parse.c \
		  $(SRCDIR)/symbols.c \
		  $(SRCDIR)/sort.c \
		  $(SRCDIR)/print.c \
		  $(SRCDIR)/utils.c

OBJS	= $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
