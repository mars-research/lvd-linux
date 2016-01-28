.PHONY: clean test

bin = compiler
idl = parser/lcd_idl.cpp include/lcd_idl.h

CXXFLAGS = -g -fsanitize=address  -fno-omit-frame-pointer -Iinclude/ -lasan
CXX = g++
LDFLAGS = -fsanitize=address

objects = lcd_ast.o main.o lcd_idl.o scope.o symbol_table.o variable.o types.o type_name_visitor.o error.o marshal_op.o ccst.o server.o client.o helper.o unmarshal.o marshal.o

$(bin): $(objects) 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

main.o: main/main.cpp include/lcd_ast.h include/lcd_idl.h include/error.h include/ccst.h include/code_gen.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

error.o: error/error.cpp include/error.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

marshal.o: code_gen/marshal.cpp include/ccst.h include/code_gen.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

unmarshal.o: code_gen/unmarshal.cpp include/ccst.h include/code_gen.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

helper.o: code_gen/helper.cpp include/ccst.h include/code_gen.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

client.o: code_gen/client.cpp include/ccst.h include/code_gen.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

server.o: code_gen/server.cpp include/ccst.h include/code_gen.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

ccst.o: ccst.cpp include/ccst.h include/marshal_op.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

type_name_visitor.o: type_name_visitor.cpp include/lcd_ast.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

types.o: ast/types.cpp include/lcd_ast.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

variable.o: ast/variable.cpp include/lcd_ast.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

lcd_idl.o: $(idl) include/lcd_ast.h 
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

scope.o: ast/scope.cpp include/lcd_ast.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

symbol_table.o: symbol_table.cpp include/symbol_table.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

marshal_op.o: marshal_op.cpp include/marshal_op.h include/lcd_ast.h include/assert.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

lcd_ast.o: ast/lcd_ast.cpp include/marshal_op.h include/lcd_ast.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter-out %.h,$^)

include/lcd_idl.h: parser/lcd_idl.peg 
	parser/vembyr-1.1/peg.py --h $^ > $@ 

parser/lcd_idl.cpp: parser/lcd_idl.peg
	parser/vembyr-1.1/peg.py --cpp $^ > $@ 

clean:
	-rm -f $(objects) $(bin) $(idl) inclue/lcd_ast.h.gch

test: $(bin)	
	./test/test.py
