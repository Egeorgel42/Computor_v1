CC = c++
SRC = help.cpp \
main.cpp \
run.cpp \
Term.cpp \
solveFirstDegree.cpp \
solveSecondDegree.cpp

OBJ = ${SRC:.cpp=.o}
CFLAGS = -Wall -Werror -Wextra -g #-fsanitize=address
LFLAGS = -lgmpxx -lgmp
NAME = computor

all: $(NAME)

$(NAME): $(OBJ) Makefile
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(LFLAGS)

.PHONY: all clean fclean re