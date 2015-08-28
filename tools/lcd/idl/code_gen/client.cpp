#include "ccst.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

CCSTFile* generate_client_header(File* f)
{
  
}

CCSTFile* generate_client_source(File* f)
{
  
  std::vector<CCSTExDeclaration*> definitions;
  
  
  CCSTFile *c_file = new CCSTFile(definitions);
  return c_file;
}

CCSTCompoundStatement* create_caller_body(Rpc *r)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  std::vector<Parameter*> params = r->parameters();
  for(std::vector<Parameter*>::iterator it = params.begin(); it != params.end(); it ++) {
    Parameter *p = (Parameter*) *it;
    statements.push_back(marshal_parameter(p));
  }

  // implicit returns
  std::vector<Marshal_type*> implicit_ret_info = r->implicit_ret_marshal_info();
  for(std::vector<Marshal_type*>::iterator it = implicit_ret_info.begin(); it != implicit_ret_info.end(); it ++)
    {
      Marshal_type *mt = *it;
      UnmarshalTypeVisitor *visitor = new UnmarshalTypeVisitor();
      statements.push_back(mt->accept(visitor));
      
      
    }
  
  if(r->explicit_return_type()->num() != 5) { // not void
    Marshal_type *ret_info = r->explicit_ret_marshal_info();
    
    CCSTPointer *p = 0x0;
    if(r->explicit_return_type()->num() == 3) {
      p = new CCSTPointer();
    }
    std::vector<CCSTDecSpecifier*> ret_type = type(r->explicit_return_type());
    

    UnmarshalTypeVisitor *visitor = new UnmarshalTypeVisitor();
    ret_info->accept(visitor);
    
    statements.push_back(new CCSTReturn(adfads));
  }
  else {
    statements.push_back(new CCSTReturn());
  }

  return new CCSTCompoundstatement(declarations, statements);
  
}
