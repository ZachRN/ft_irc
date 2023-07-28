NAME = ircserv
BUILD_DIR = build
VPATH := $(subst $(" "),:,$(shell find srcs -type d))
SRCS += $(wildcard srcs/*.cpp srcs/*/*.cpp) 
OBJ_FILES = $(addprefix $(BUILD_DIR)/, $(notdir $(patsubst %.cpp, %.o, $(SRCS))))
HEADER_FILES := $(addprefix -I,$(shell find includes -type d -print))
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address #NOTE C98 NOT ENFORCED ANYMORE
#-g -fsanitize=adress 
CC = c++

all: build_dir $(NAME)

$(NAME): $(OBJ_FILES)
	@echo Assembling $(NAME)
	@$(CC) $(HEADER_FILES) -o $@ $^ $(CFLAGS)
	@echo $(NAME) has been made!

$(BUILD_DIR)/%.o: %.cpp
	@$(CC) $(HEADER_FILES) -c $(CFLAGS) -o $@ $<

build_dir:
	clear
	@if [ -d "./$(BUILD_DIR)" ]; then \
	echo "Build Directory Already Exists"; \
	else \
	mkdir -p $(BUILD_DIR); \
	fi

clean:
	clear
	@echo Cleaning all object files 
	@ rm -f $(BUILD_DIR)/*.o
	@echo Removing Build Directory
	@if [ -d "./$(BUILD_DIR)" ]; then \
	rmdir $(BUILD_DIR); fi

fclean: clean
	@echo Removing $(NAME)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re build_dir bonus