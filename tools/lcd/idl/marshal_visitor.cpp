#include "marshal_visitor.h"

MarshalVisitor::MarshalVisitor()
{
}

const char* ret_name_gen(char *name, int num)
{
  std::ostringstream os ;
  
  //throw the value into the string stream
  char* ret_name= (char*) malloc(sizeof(char)*(strlen(name)+16+5));
  sprintf(ret_name, "ret_%s%d", name, num);
  printf("ret name %s\n", ret_name);
  return ret_name;
}

Marshal_type* MarshalVisitor::visit(File *file, Registers *data)
{
  // call accept for each rpc
  // ++it vs it++
  printf("here in marshal visitor file\n");
  std::vector<Rpc*> rpc_definitions = file->rpc_defs();
  for(std::vector<Rpc*>::iterator it = rpc_definitions.begin(); it != rpc_definitions.end(); ++it)
    {
      Rpc * temp = *it;
      temp->accept(this, data);
    }
  return 0x0;
}
Marshal_type* MarshalVisitor::visit(ProjectionField *proj_field, Registers *data)
{
  Marshal_type *mt = proj_field->type()->accept(this, data);
  mt->set_name(proj_field->field_name());
  return mt;
}

Marshal_type* MarshalVisitor::visit(Rpc *rpc, Registers *data)
{
  printf("here in marshal visitor rpc\n");
  std::vector<Parameter*> parameters = rpc->parameters();
  Registers *reg = new Registers();
  for(std::vector<Parameter*>::iterator it = parameters.begin(); it != parameters.end(); ++it)
    {
      Parameter * temp = *it;
      temp->accept(this, reg);
    }
  
  // handle return;
  Type* explicit_return_type = rpc->explicit_return_type();
  Registers *ret_regs = new Registers();
  Marshal_type *explicit_marshal_info = explicit_return_type->accept(this, ret_regs);
  explicit_marshal_info->set_name(ret_name_gen(rpc->name(), 0));
  std::vector<Type*> implicit_return_types = rpc->implicit_return_types();
  std::vector<Marshal_type*> implicit_marshal_info;

  int i = 1;
  for(std::vector<Type*>::iterator it2 = implicit_return_types.begin(); it2 != implicit_return_types.end(); ++it2)
    {
      Type* tmp = *it2;
      Marshal_type *mt = tmp->accept(this, data);
      mt->set_name(ret_name_gen(rpc->name(), i));
      implicit_marshal_info.push_back(mt);
      i ++;
    }
  
  rpc->set_implicit_ret_marshal_info(implicit_marshal_info);
  rpc->set_explicit_ret_marshal_info(explicit_marshal_info);
  return 0x0;
}
Marshal_type* MarshalVisitor::visit(Parameter *parameter, Registers *data)
{
  printf("here in marshal visitor param\n");
  Marshal_type *mt = parameter->type()->accept(this, data);
  mt->set_name(parameter->name());
  parameter->set_marshal_info(mt);
  printf("return from param\n");
  return 0x0;
}

Marshal_type* MarshalVisitor::visit(ProjectionType *proj_type, Registers *data)
{
  printf("here proj\n");
  std::vector<Marshal_type*> marshal_fields;
  std::vector<ProjectionField*> fields = proj_type->fields();
  for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); ++it)
    {
      ProjectionField *pf = *it;
      Marshal_type *mt = pf->accept(this, data);
      mt->set_name(pf->field_name());
      marshal_fields.push_back(mt);
    }
  return new Marshal_projection(proj_type, marshal_fields);
}

Marshal_type* MarshalVisitor::visit(IntegerType* int_type, Registers *data)
{
  printf("here int\n");
  int reg = data->allocate_next_free_register(); // check if available
  return new Marshal_integer(int_type, reg);
}

Marshal_type* MarshalVisitor::visit(PointerType* pt, Registers *data)
{
  printf("here pointer\n");
  Marshal_type *mt = pt->type()->accept(this, data);
  return new Marshal_pointer(pt, mt);
}

Marshal_type* MarshalVisitor::visit(Typedef* td, Registers *data)
{
  printf("here typedef\n");
  return new Marshal_typedef(td, td->type()->accept(this, data) );
}

Marshal_type* MarshalVisitor::visit(VoidType *vt, Registers *data)
{
  printf("here void\n");
  return new Marshal_void(vt);
}

Marshal_type* MarshalVisitor::visit(FileScope *fs, Registers *data)
{
  Assert(1 != 0, "Error: FileScope does not support this\n");
}

Marshal_type* MarshalVisitor::visit(RootScope *rs, Registers *data)
{
  Assert(1 != 0, "Error: Rootscope does not support this\n");
}
