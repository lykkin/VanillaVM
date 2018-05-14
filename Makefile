NAME=vanillavm
SRC_DIR := ./src
OBJ_DIR := ./out
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
CXX=g++

OUT_DIR = ./out

MKDIR_P = mkdir -p

.PHONY: clean test directories

INCLUDES_DIR = ./include

CXXFLAGS=-std=c++11 -Wall -Werror -g -O0 -Wextra -Wstrict-aliasing=2 \
		 -Wformat=2 -Wno-format-nonliteral -Wshadow -Wpointer-arith -Wcast-qual \
		 -Wmissing-prototypes -Wno-missing-braces -pedantic-errors -pthread -I$(INCLUDES_DIR)

all: directories compile

compile: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(NAME) $^

clean:
	$(RM) ./out/*.o
	$(RM) $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(INCLUDES_DIR)/%.h)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

test: all
	./vanillavm test/test.am test/test.am

directories: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

