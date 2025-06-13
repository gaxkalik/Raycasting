CLANG = clang++ -std=c++17
CFLAGS = -DGL_SILENCE_DEPRECATION -w -MMD -MP -fsanitize=address -g
LDFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL
INCLUDE = -I./include -I/opt/homebrew/include
RM = rm -rf

NAME = rc

CPP_SRCS = $(wildcard src/*.cpp)
C_SRCS = src/glad.c
OTHER_SRCS = main.cpp
SRCS = $(CPP_SRCS) $(C_SRCS) $(OTHER_SRCS)

OBJ_DIR = obj

OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
OBJS := $(OBJS:%.c=$(OBJ_DIR)/%.o)

DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CLANG) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CLANG) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CLANG) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR) $(NAME)

fclean: clean
	$(RM) $(NAME)

# Rebuild everything
re: fclean all

# Include dependencies
-include $(DEPS)

.PHONY: all clean fclean re