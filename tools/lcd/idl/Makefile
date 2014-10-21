
lcd_compiler: lcd_ast.o lcd_compiler.o lcd_idl.o
		g++ lcd_idl.o lcd_ast.o lcd_compiler.o -o compiler

lcd_compiler.o: lcd_compiler.cpp include/lcd_ast.h lcd_idl.h
		g++ -c -g lcd_compiler.cpp

lcd_idl.o: parser/lcd_idl.h include/lcd_ast.h

lcd_ast.o: include/lcd_ast.h

lcd_idl.h: 
	parser/vembyr-1.1/peg.py --h parser/lcd_idl.peg > parser/lcd_idl.h

lcd_idl.cpp:
	parser/vembyr-1.1/peg.py --cpp parser/lcd_idl.peg > parser/lcd_idl.cpp

clean:
		rm lcd_idl.cpp lcd_idl.h lcd_ast.h.gch compiler *.o

test:	lcd_compiler
	./test/test.py
