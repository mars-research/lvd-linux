
lcd_compiler: lcd_ast.o lcd_compiler.o lcd_idl.o scope.o
		g++ lcd_idl.o lcd_ast.o scope.o lcd_compiler.o -o compiler

lcd_compiler.o: lcd_compiler.cpp include/lcd_ast.h lcd_idl.h
		g++ -c -g lcd_compiler.cpp

lcd_idl.o: lcd_idl.h include/lcd_ast.h lcd_idl.cpp
		g++ -c parser/lcd_idl.cpp

scope.o: include/lcd_ast.h ast/scope.cpp
	g++ -c ast/scope.cpp include/lcd_ast.h

lcd_ast.o: include/lcd_ast.h ast/lcd_ast.cpp
	g++ -c ast/lcd_ast.cpp include/lcd_ast.h

lcd_idl.h: 
	parser/vembyr-1.1/peg.py --h parser/lcd_idl.peg > parser/lcd_idl.h

lcd_idl.cpp:
	parser/vembyr-1.1/peg.py --cpp parser/lcd_idl.peg > parser/lcd_idl.cpp

clean:
		rm parser/lcd_idl.cpp parser/lcd_idl.h lcd_ast.h.gch compiler *.o

test:	lcd_compiler
	./test/test.py
