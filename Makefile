CLANG = clang++
CFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL -DGL_SILENCE_DEPRECATION -w
INCLUDE = -I./include -I/opt/homebrew/include
VPATH = src
RM = rm -rf

GENERAL =

NAME = a
SRCS = $(addsuffix .cpp, $(GENERAL)) src/glad.c main.cpp

OBJ_DIR = obj
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
#OBJS := $(OBJS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CLANG) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp Makefile
	$(CLANG) $(CFLAGS) -c $< -o $@ $(INCLUDE)

$(OBJ_DIR)/%.o: %.c Makefile
	$(CLANG) $(CFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

run: all clean
	./$(NAME)

.PHONY: all clean fclean re run
