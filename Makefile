NAME=vanillavm
CXXFLAGS=-std=c++11 -Wall -Werror -g -O0 -Wstrict-aliasing=2
OBJ_FILES=main.o
CXX=g++

INCLUDES=-I./include

all: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $<

clean:
	rm *.o
	rm $(NAME)

%.o: ./%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<

