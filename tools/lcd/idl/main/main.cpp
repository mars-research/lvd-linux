#include "lcd_ast.h"
#include "lcd_idl.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "ccst.h"
#include "code_gen.h"
//#include "marshal_visitor.h"

int main(int argc, char ** argv)
{
  bool pDebug = false;

  if(argc != 3)
    {
      printf("error in number of args\n");
      exit(0);
    }

  try
    {
      char * file = argv[2];
      // add support for multiple files
      // add option to specify which module to compile,
      // or option for all
      // put each module in a different file
      Project * tree = (Project *) Parser::parse(std::string(file));

      // 
      // todo error reporting stuff
      ErrorReport* er = ErrorReport::instance();
      if(er->errors())
	{
	  printf("There were errors during parsing\n");
	  // cleanup?
	  exit(0);
	}

      if(pDebug) { printf("successfully parsed project.\n"); }
      
      // no longer specify file?
      //  char* out_file = argv[3];
      
      // if(pDebug) { printf("Wring to file: %s.\n", out_file); }

      // FILE* of = fopen(out_file, "w");
      
      if(!strcmp(argv[1],"-serverheader"))
	{
	  printf("todo\n");
	  /*
	  MarshalVisitor* mv = new MarshalVisitor();
	  tree->accept(mv, 0x0);

	  CCSTFile* ccst_tree = generate_server_header(tree);
	  ccst_tree->write(of);
	  */
	  printf("completed header source writing\n");
	}
      else if(!strcmp(argv[1],"-serversource")) // callee
	{
	  tree->resolve_types();
	  tree->create_trampoline_structs();
	  tree->function_pointer_to_rpc();
	  tree->generate_function_tags();
	  // resolve types that weren't resolved during parsing
	  tree->create_container_variables();
	  
	  //	  tree->set_copy_container_accessors(); // TODO

	  tree->copy_types();
	  tree->initialize_types(); // for calling initialize o
	  tree->set_accessors();
	  tree->prepare_marshal();
	  printf("done doing all setup\n");
	  

	  std::vector<Module*> project_modules = tree->modules();
	  for(std::vector<Module*>::iterator it = project_modules.begin(); it != project_modules.end(); it ++) {
	    Module *m = *it;
	    if (m == 0x0) {
	      printf("Module is null\n");
	      exit(0);
	    }

	    char* of_name = (char*) malloc(sizeof(char)*(strlen(m->identifier())+3));
	    std::ostringstream total;
	    total << m->identifier() << ".c";
	    strncpy(of_name, total.str().c_str(), strlen(m->identifier())+3);

	    FILE *of = fopen(of_name, "w");
	    if(!of)
	      {
		printf("Error: unable to open %s for writing\n", m->identifier());
		perror("error");
		// cleanup
		exit(0);
	      }
	    CCSTFile* ccst_tree = generate_server_source(m);
	    ccst_tree->write(of, 0);
	  }

	  printf("Completed Server source writing\n");
	}
      else if(!strcmp(argv[1], "-clientheader"))
	{
	  printf("todo\n");
	  /*
	  CCSTFile* ccst_tree = generate_client_header(tree);
	  ccst_tree->write(of);
	  */
	  printf("completed client header writing\n");
	}
      else if(!strcmp(argv[1], "-clientsource")) // caller
	{
	  tree->create_trampoline_structs();
	  tree->function_pointer_to_rpc();
	  tree->generate_function_tags();
	  tree->resolve_types(); // resolve types that weren't resolved during parsing
	  tree->create_container_variables();
	  
	  tree->set_copy_container_accessors(); // TODO

	  tree->copy_types();
	  tree->initialize_types(); // for calling initialize o
	  tree->set_accessors();
	  tree->prepare_marshal();
	  printf("done doing all setup\n");
	  
	  std::vector<Module*> project_modules = tree->modules();
	  for(std::vector<Module*>::iterator it = project_modules.begin(); it != project_modules.end(); it ++) {
	    Module *m = *it;
	    if (m == 0x0) {
	      printf("Module is null\n");
	      exit(0);
	    }

	    char* of_name = (char*) malloc(sizeof(char)*(strlen(m->identifier())+3));
	    std::ostringstream total;
	    total << m->identifier() << ".c";
	    strncpy(of_name, total.str().c_str(), strlen(m->identifier())+3);

	    FILE *of = fopen(of_name, "w");
	    if(!of)
	      {
		printf("Error: unable to open %s for writing\n", m->identifier());
		perror("error");
		// cleanup
		exit(0);
	      }
	    CCSTFile* ccst_tree = generate_client_source(m);
	    ccst_tree->write(of, 0);
	  }
	  printf("completed client source writing\n");
	  
	}
      else
	{
	  printf("error unrecognized option: %s\n", argv[1]);
	}
      return 0;
    }
  catch (const Parser::ParseException e)
    {
      printf("caught a parser exception\n");
      //  printf("e is: %s\n", e.getReason().c_str());
      
      exit(0);
    }
}
