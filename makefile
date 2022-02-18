CC=g++
CCFLAGS= -Wall -Werror -std=c++11 -g
LIBFLAGS= Mesh.hh Triangle.hh Point.hh
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
TST= $(wildcard uniTest/*.cpp)
OBJ_TEST= $(filter-out main.o, $(OBJ)) $(TST:.cpp=.o)
EXEC= logic

all: $(EXEC)

testcase : $(OBJ_TEST)
	g++ -std=c++11 -Wall -o $@ $^

$(EXEC): $(OBJ)
	$(CC) $(LIBFLAGS) $^ -o $@

%.o: %.cc
	$(CC) $(CCFLAGS) $@ -c $<

.depend:
	g++ -MM $(SRC) > .depends
-include .depends

clean:
	rm -f $(OBJ) $(EXEC)
cleantest:
	rm -f $OBJ_TEST) testcase
distclean : clean cleantest
