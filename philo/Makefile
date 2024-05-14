NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror 
RM = rm -rf
SRCS = main.c utils.c ft_atoi.c ft_itoa.c actions.c
OBJ = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	$(RM) $(SRCS:.c=.o)

fclean: clean
	$(RM) $(NAME)

re: fclean all

git:
	@make fclean
	@git add .
	@git status
	@read -p "Enter commit message: " message; \
	git commit -m "$$message"
	@git push
	@git log

.PHONY = all clean fclean re git