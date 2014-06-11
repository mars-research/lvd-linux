
all: compiler

compiler: flounder_parser.o idl_compiler.o ast.o
	g++ flounder_parser.o idl_compiler.o ast.o -o compiler

idl_compiler.o: idl_compiler.cpp flounder_parser.h
		g++ -c idl_compiler.cpp flounder_parser.h

ast.o: ast.cpp ast.h
	g++ -c ast.cpp ast.h

flounder_parser.o: flounder_parser.cpp flounder_parser.h
		g++ -c flounder_parser.cpp flounder_parser.h

flounder_parser.cpp:
		vembyr-1.1/peg.py --cpp flounder > flounder_parser.cpp

flounder_parser.h:
		vembyr-1.1/peg.py --h flounder > flounder_parser.h

clean:
		rm flounder_parser.* *.o compiler vembyr-1.1/peg_peg.py
		vembyr-1.1/peg_peg.pyc