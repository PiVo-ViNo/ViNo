SRCS = test_TokenScanner.cpp ../src/TokenScanner.cpp
OBJS = test_TokenScanner.o TokenScanner.o
INCLUDE = -I../include -I../src
CXX = g++
CXXFLAGS = -g -std=c++20

tokenscanner: ${OBJS}
	${CXX} -o $@ $^ ${CXXFLAGS}

%.o: %.cpp
	${CXX} $< -c ${INCLUDE} ${CXXFLAGS}

.PHONY: clean
clean:
	rm -rf tokenscanner.* ${OBJS}