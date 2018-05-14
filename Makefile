NAME=vanillavm
SRC_DIR := ./src
OBJ_DIR := ./out
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
CXXFLAGS=-std=c++11 -Wall -Werror -g -O0 -Wextra -Wstrict-aliasing=2 \
		 -Wformat=2 -Wno-format-nonliteral -Wshadow -Wpointer-arith -Wcast-qual \
		 -Wmissing-prototypes -Wno-missing-braces -pedantic-errors -pthread
CXX=g++

OUT_DIR = ./out

MKDIR_P = mkdir -p

.PHONY: clean test directories

INCLUDES=-I./include

all: directories compile

compile: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $^

clean:
	$(RM) ./out/*.o
	$(RM) $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

test: all
	./vanillavm test/test.am test/test.am

directories: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

