.PHONY: default clean test lcd_idl.h lcd_idl.cpp

bin = compiler
idl_cpp = parser/lcd_idl.cpp
idl_h = include/lcd_idl.h
idl = $(idl_cpp) $(idl_h)

CXXFLAGS = -g -fsanitize=address  -fno-omit-frame-pointer -Iinclude/ -lasan
CXXFLAGS += -MMD
CXX = g++
LDFLAGS = -fsanitize=address

CPP_a = $(shell find . -type f -name "*.cpp") $(idl_cpp)
CPP = $(filter-out ./test/%,$(CPP_a))
OBJ = $(patsubst %.cpp,%.o,$(CPP))
DEP = $(patsubst %.cpp,%.d,$(CPP))

default: $(bin)

$(bin): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

main/main.cpp: $(idl_h)

$(idl_h): parser/lcd_idl.peg 
	parser/vembyr-1.1/peg.py --h $^ > $@ 

$(idl_cpp): parser/lcd_idl.peg
	parser/vembyr-1.1/peg.py --cpp $^ > $@ 

clean:
	-rm -f $(OBJ) $(bin) $(idl) include/lcd_ast.h.gch $(DEP)

test: $(bin)	
	./test/test.py

-include $(DEP)
