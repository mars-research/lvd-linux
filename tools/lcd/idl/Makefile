.PHONY: clean test

bin = compiler
idl = parser/lcd_idl.cpp include/lcd_idl.h

CXXFLAGS = -g -fsanitize=address  -fno-omit-frame-pointer -Iinclude/ -lasan
CXX = g++
LDFLAGS = -fsanitize=address

objects = lcd_ast.o main.o lcd_idl.o scope.o error.o marshal_op.o ccst.o ast_to_ccst.o marshal_visitor.o

$(bin): $(objects) 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

main.o: main/main.cpp include/lcd_ast.h include/lcd_idl.h include/error.h include/ccst.h include/marshal_visitor.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

error.o: error/error.cpp include/error.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

ast_to_ccst.o: ast_to_ccst.cpp include/ccst.h include/marshal_op.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

ccst.o: ccst.cpp include/ccst.h include/marshal_op.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

lcd_idl.o: $(idl) include/lcd_ast.h 
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

scope.o: ast/scope.cpp include/lcd_ast.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

marshal_visitor.o: marshal_visitor.cpp include/marshal_visitor.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

marshal_op.o: marshal_op.cpp include/marshal_op.h include/lcd_ast.h include/assert.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

lcd_ast.o: ast/lcd_ast.cpp include/lcd_ast.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

include/lcd_idl.h: parser/lcd_idl.peg 
	parser/vembyr-1.1/peg.py --h $^ > $@ 

parser/lcd_idl.cpp: parser/lcd_idl.peg
	parser/vembyr-1.1/peg.py --cpp $^ > $@ 

clean:
	-rm -f $(objects) $(bin) $(idl) inclue/lcd_ast.h.gch

test: $(bin)	
	./test/test.py
