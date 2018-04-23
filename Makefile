NAME=vanillavm
CXXFLAGS=-std=c++11 -Wall -Werror -g -O0 -Wextra -Wstrict-aliasing=2 \
		 -Wformat=2 -Wno-format-nonliteral -Wshadow -Wpointer-arith -Wcast-qual \
		 -Wmissing-prototypes -Wno-missing-braces -pedantic-errors -pthread
OBJ_FILES=main.o
CXX=g++

.PHONY: clean test

INCLUDES=-I./include

all: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $<

clean:
	$(RM) *.o
	$(RM) $(NAME)

%.o: ./%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<

test: all
	./vanillavm test/test.am test/test.am
