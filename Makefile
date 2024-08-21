CC = c++
SRC = help.cpp \
main.cpp \
run.cpp \
Term.cpp \
solveFirstDegree.cpp

OBJ = ${SRC:.cpp=.o}
CFLAGS = -Wall -Werror -Wextra -g #-fsanitize=address
NAME = computor

all: $(NAME)

$(NAME): $(OBJ) Makefile
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: all clean fclean re