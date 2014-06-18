
all: compiler

compiler: flounder_parser.o idl_compiler.o ast.o visitor.o
	g++ flounder_parser.o visitor.o idl_compiler.o ast.o -o compiler

idl_compiler.o: idl_compiler.cpp flounder_parser.h
		g++ -c -g idl_compiler.cpp flounder_parser.h

ast.o: ast.cpp ast.h
	g++ -c -g ast.cpp ast.h

visitor.o: visitor.cpp visitor.h
	g++ -c -g visitor.cpp visitor.h

flounder_parser.o: flounder_parser.cpp flounder_parser.h
		g++ -c -g flounder_parser.cpp flounder_parser.h

flounder_parser.cpp:
		vembyr-1.1/peg.py --cpp flounder > flounder_parser.cpp

flounder_parser.h:
		vembyr-1.1/peg.py --h flounder > flounder_parser.h

clean:
		rm flounder_parser.* *.o compiler peg_peg.py
		vembyr-1.1/peg_peg.pyc