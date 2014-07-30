
lcd_compiler: lcd_ast.o lcd_compiler.o lcd_idl.o
		g++ lcd_idl.o lcd_ast.o lcd_compiler.o -o compiler

lcd_compiler.o: lcd_compiler.cpp lcd_ast.h lcd_idl.h
		g++ -c -g lcd_compiler.cpp

lcd_idl.o: lcd_idl.h lcd_ast.h

lcd_ast.o: lcd_ast.h

lcd_idl.h: 
	vembyr-1.1/peg.py --h lcd_idl.peg > lcd_idl.h

lcd_idl.cpp:
	vembyr-1.1/peg.py --cpp lcd_idl.peg > lcd_idl.cpp

clean:
		rm lcd_idl.cpp lcd_idl.h lcd_ast.h.gch compiler *.o