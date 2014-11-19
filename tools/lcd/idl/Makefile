.PHONY: clean test

bin = compiler
idl = parser/lcd_idl.cpp include/lcd_idl.h

CXXFLAGS = -g -Iinclude/
CXX = g++

objects = lcd_ast.o main.o lcd_idl.o scope.o header_gen.o error.o

$(bin): $(objects) 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

main.o: main/main.cpp include/lcd_ast.h include/lcd_idl.h include/gen_visitor.h include/error.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

error.o: error/error.cpp include/error.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

header_gen.o: gen/header_gen.cpp include/gen_visitor.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

lcd_idl.o: $(idl) include/lcd_ast.h 
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

scope.o: ast/scope.cpp include/lcd_ast.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

lcd_ast.o: ast/lcd_ast.cpp include/lcd_ast.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

include/lcd_idl.h: parser/lcd_idl.peg 
	parser/vembyr-1.1/peg.py --h $^ > $@ 

parser/lcd_idl.cpp: parser/lcd_idl.peg
	parser/vembyr-1.1/peg.py --cpp $^ > $@ 

clean:
	-rm -f $(objects) $(bin) $(idl) lcd_ast.h.gch

test: $(bin)	
	./test/test.py
