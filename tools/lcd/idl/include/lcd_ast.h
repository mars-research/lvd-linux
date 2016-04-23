#ifndef LCD_AST_H
#define LCD_AST_H

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <stdio.h>
#include "marshal_op.h"
#include "symbol_table.h"
#include "ccst.h"

class MarshalPrepareVisitor;
class CCSTStatement;
class CCSTTypeName;
class Marshal_type;
class TypeVisitor;
class MarshalVisitor;
class TypeNameVisitor;
class AllocateTypeVisitor;
class Variable;
class Type;
class Parameter;
class Rpc;
class ProjectionType;
class Project;

enum PrimType {pt_char_t, pt_short_t, pt_int_t, pt_long_t, pt_longlong_t, pt_capability_t};
enum type_k {};

template<class T, class T2>
class ASTVisitor;

class Base
{			  
 public:
};

class LexicalScope : public Base
{
 public:
  LexicalScope *outer_scope_;
  std::map<std::string, Type*> type_definitions_;
  std::map<std::string, Variable*> variables_;
  std::map<std::pair<std::string, std::vector<Parameter*> >, Rpc*> rpc_definitions_; // rpc or function pointer. why do we keep this? 

  std::vector<std::string> identifiers_; // new

  std::vector<LexicalScope*> inner_scopes_;
  LexicalScope();
  LexicalScope(LexicalScope *outer_scope);
  std::vector<Rpc*> rpc_in_scope();
  bool insert(Rpc *r);
  bool insert(Variable *v);

  bool insert_identifier(const char* id);
  bool contains_identifier(const char* id);

  Variable* lookup_variable(const char *sym, int* err);
  Type* lookup(const char *sym, int* err);
  bool insert(const char *sym, Type* type);
  bool contains(const char *symbol);
  virtual void set_outer_scope(LexicalScope *ls);
  void add_inner_scope(LexicalScope *ls);
  void add_inner_scopes(std::vector<LexicalScope*> scopes);
  std::map<std::string, Type*> type_definitions();
  std::vector<LexicalScope*> inner_scopes();
  LexicalScope* outer_scope();
  void resolve_types();
  void create_trampoline_structs();
  std::vector<Rpc*> function_pointer_to_rpc();
  std::map<std::string, Type*> all_type_definitions();
  std::map<std::string, Type*> all_types_outer();
  std::map<std::string, Type*> all_types_inner();
};

class GlobalScope : public LexicalScope
{
  static GlobalScope *instance_;
 public:
  GlobalScope();
  virtual void set_outer_scope(LexicalScope *ls); // x
  static GlobalScope* instance(); // x
};

class Type : public Base
{
 public:
  virtual Type* clone() const = 0;
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker) = 0;
  virtual CCSTTypeName* accept(TypeNameVisitor *worker) = 0;
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v) = 0;
  virtual int num() = 0;
  virtual const char* name() = 0;
  virtual void resolve_types(LexicalScope *ls) = 0;
  virtual void create_trampoline_structs(LexicalScope *ls) = 0;
};

class InitializeType : public Type
{
 public:
  Type* type_; // this is the type that WILL be initialized.  
  std::vector<Variable*> values_; // this is what will initialize the type
  InitializeType(Type *type, std::vector<Variable*> init_values);
  InitializeType(const InitializeType& other);
  virtual Type* clone() const { return new InitializeType(*this); }
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual int num();
  virtual const char* name();
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_trampoline_structs(LexicalScope *ls);

  void initialize();
};

class UnresolvedType : public Type
{
 public:
  const char *type_name_;
  UnresolvedType(const char* type_name);
  UnresolvedType(const UnresolvedType& other);
  virtual Type* clone() const { return new UnresolvedType(*this); }
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker); // need to add unresolved type to these visitors.
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual int num();
  virtual const char* name();
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class Variable : public Base
{
 public:
  virtual Variable* clone() const = 0;
  virtual Type* type() = 0;
  virtual const char* identifier() = 0;
  virtual void set_identifier(const char* id) = 0;
  virtual void set_accessor(Variable *v) = 0;
  virtual Variable* accessor() = 0;
  virtual void set_marshal_info(Marshal_type *mt) = 0;
  virtual Marshal_type* marshal_info() = 0;
  virtual int pointer_count() = 0;
  virtual void set_pointer_count(int pcount) = 0;
  virtual void prepare_marshal(MarshalPrepareVisitor *worker) = 0;
  virtual void resolve_types(LexicalScope *ls) = 0;
  virtual void create_container_variable(LexicalScope *ls) = 0;
  virtual void initialize_type() = 0;

  virtual void set_in(bool b) = 0;
  virtual void set_out(bool b) = 0;
  virtual void set_alloc_caller(bool b) = 0;
  virtual void set_alloc_callee(bool b) = 0;
  virtual void set_dealloc_caller(bool b) = 0;
  virtual void set_dealloc_callee(bool b) = 0;
  virtual void set_bind_caller(bool b) = 0;
  virtual void set_bind_callee(bool b) = 0;

  virtual bool in() = 0;
  virtual bool out() = 0;
  virtual bool alloc_caller() = 0;
  virtual bool alloc_callee() = 0;
  virtual bool dealloc_caller() = 0;
  virtual bool dealloc_callee() = 0;
  virtual bool bind_caller() = 0;
  virtual bool bind_callee() = 0;

  virtual Variable* container() = 0;
};

class GlobalVariable : public Variable
{
 public:
  Type *type_;
  const char *id_;
  int pointer_count_;
  Marshal_type *marshal_info_;
  Variable *container_;
  Variable *accessor_;
  GlobalVariable(Type *type, const char *id, int pointer_count);
  GlobalVariable(const GlobalVariable& other);
  virtual Variable* clone() const { return new GlobalVariable(*this); }
  virtual Variable* container();
  virtual void prepare_marshal(MarshalPrepareVisitor *worker);
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_container_variable(LexicalScope *ls);
  virtual Type* type();
  virtual const char* identifier();
  virtual void set_identifier(const char* id);
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual void set_marshal_info(Marshal_type *mt);
  virtual Marshal_type* marshal_info();
  virtual int pointer_count();
  virtual void set_pointer_count(int pcount);
  virtual void initialize_type();

  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_dealloc_caller(bool b);
  virtual void set_dealloc_callee(bool b);
  virtual void set_bind_caller(bool b);
  virtual void set_bind_callee(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool dealloc_caller();
  virtual bool dealloc_callee();
  virtual bool bind_caller();
  virtual bool bind_callee();
};

class Parameter : public Variable
{
 public:
  bool in_;
  bool out_;
  bool alloc_callee_;
  bool alloc_caller_;
  bool dealloc_callee_;
  bool dealloc_caller_;
  bool bind_caller_; 
  bool bind_callee_;
  
  Type* type_;
  const char* name_;
  Marshal_type *marshal_info_;
  Variable *accessor_;
  int pointer_count_;
  Variable *container_;
  Parameter();
  Parameter(Type* type, const char* name, int pointer_count);
  Parameter(const Parameter& other);
  virtual Variable* clone() const { return new Parameter(*this); }
  virtual Variable* container();
  virtual void prepare_marshal(MarshalPrepareVisitor *worker);
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_container_variable(LexicalScope *ls);
  virtual Type* type();
  virtual void set_marshal_info(Marshal_type* mt);
  virtual Marshal_type* marshal_info(); 
  virtual const char* identifier();
  virtual void set_identifier(const char* id);
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual int pointer_count();
  virtual void set_pointer_count(int pcount);
  virtual void initialize_type();
  
  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_dealloc_caller(bool b);
  virtual void set_dealloc_callee(bool b);  
  virtual void set_bind_caller(bool b);
  virtual void set_bind_callee(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool dealloc_caller();
  virtual bool dealloc_callee();
  virtual bool bind_caller();
  virtual bool bind_callee();
};


// a parameter without a name
// add to variables.cpp
class FPParameter : public Parameter
{
 public:
  Type *type_;
  int pointer_count_;
  Marshal_type *marshal_info_;
  Variable *container_;
  FPParameter(Type *type, int pointer_count);
  FPParameter(const FPParameter& other);
  virtual Variable* clone() const { return new FPParameter(*this); }
  virtual Variable* container();
  virtual Type* type();
  virtual const char* identifier();
  virtual void set_identifier(const char* id);
  virtual int pointer_count();
  virtual void set_pointer_count(int pcount);
  virtual void set_marshal_info(Marshal_type *mt);
  virtual Marshal_type* marshal_info();
  virtual void prepare_marshal(MarshalPrepareVisitor *worker);
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_container_variable(LexicalScope *ls);
  virtual void initialize_type();

  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_dealloc_caller(bool b);
  virtual void set_dealloc_callee(bool b);
  virtual void set_bind_caller(bool b);
  virtual void set_bind_callee(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool dealloc_caller();
  virtual bool dealloc_callee();
  virtual bool bind_caller();
  virtual bool bind_callee();
};

class ReturnVariable : public Variable
{
 public:
  const char* name_; // to be decided by a name space or something
  Type* type_;
  Marshal_type *marshal_info_;
  Variable* accessor_;
  int pointer_count_;
  Variable *container_;
  ReturnVariable();
  ReturnVariable(Type* return_type, int pointer_count, const char* id);
  ReturnVariable(const ReturnVariable& other);
  virtual Variable* clone() const { return new ReturnVariable(*this); }
  virtual Variable *container();
  virtual void set_marshal_info(Marshal_type *mt);
  virtual Marshal_type* marshal_info();
  virtual void prepare_marshal(MarshalPrepareVisitor *worker);
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_container_variable(LexicalScope *ls);
  virtual const char* identifier();
  virtual void set_identifier(const char* id);
  virtual Type* type();
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual int pointer_count();
  virtual void set_pointer_count(int pcount);
  virtual void initialize_type();

  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_dealloc_caller(bool b);
  virtual void set_dealloc_callee(bool b);
  virtual void set_bind_caller(bool b);
  virtual void set_bind_callee(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool dealloc_caller();
  virtual bool dealloc_callee();
  virtual bool bind_caller();
  virtual bool bind_callee();
};

class Function : public Type
{
 public:
  const char *identifier_;
  ReturnVariable *return_var_;
  std::vector<Parameter*> parameters_;
  LexicalScope *current_scope_;
  Function(const char *id, ReturnVariable *return_var, std::vector<Parameter*> parameters, LexicalScope *ls);
  Function(const Function& other);
  virtual Type* clone() const {return new Function(*this); }
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual int num();
  virtual const char* name();
  virtual void resolve_types(LexicalScope *ls);
  Rpc* to_rpc(ProjectionType *pt);
  virtual void create_trampoline_structs(LexicalScope *ls);
};
 
class Typedef : public Type
{
 public:
  Type* type_;
  const char* alias_;
  char* marshal_info_;
  const char* identifier_;
  
  Typedef(const char* id, const char* alias, Type* type);
  Typedef(const Typedef& other);
  virtual Type* clone() const { return new Typedef(*this); }
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual const char* name();
  Type* type();
  const char* alias();
  virtual int num();
  virtual void resolve_types(LexicalScope *ls);
  // virtual void marshal();
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class Channel : public Type 
{
 public:
  Channel();
  Channel(const Channel& other);
  virtual Type* clone() const { return new Channel(*this); }
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual const char* name();
  virtual int num();
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class VoidType : public Type
{
 public:
  VoidType();
  VoidType(const VoidType& other);
  virtual Type* clone() const { return new VoidType(*this); }
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual const char* name();
  virtual int num();
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class IntegerType : public Type
{
 public:
  bool unsigned_;
  PrimType type_;
  int size_;
  IntegerType(PrimType type, bool un, int size);
  IntegerType(const IntegerType& other);
  virtual Type* clone() const { return new IntegerType(*this); }
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual const char* name();
  PrimType int_type();
  bool is_unsigned();
  virtual int num();
  virtual void resolve_types(LexicalScope *ls);
  ~IntegerType(){printf("inttype destructor\n");}
  virtual void create_trampoline_structs(LexicalScope *ls);
};

class ProjectionField : public Variable //?
{
 public:
  bool in_;
  bool out_;
  bool alloc_callee_;
  bool alloc_caller_;
  bool dealloc_callee_;
  bool dealloc_caller_;
  bool bind_caller_;
  bool bind_callee_;

  Type* type_;
  const char* field_name_;
  Variable *accessor_; // 
  int pointer_count_;
  Marshal_type *marshal_info_;
  Variable *container_;
  ProjectionField(Type* field_type, const char* field_name, int pointer_count);
  ~ProjectionField(); 
  ProjectionField(const ProjectionField& other);
  virtual Variable* clone() const { return new ProjectionField(*this); }
  virtual Variable *container();
  virtual Type* type();
  virtual const char* identifier();
  virtual void set_identifier(const char* id);
  virtual void set_accessor(Variable *v);
  virtual Variable* accessor();
  virtual void set_marshal_info(Marshal_type *mt); // add to .cpp file
  virtual Marshal_type* marshal_info(); // make sure all variables have
  virtual void prepare_marshal(MarshalPrepareVisitor *worker);
  virtual void resolve_types(LexicalScope *ls);
  virtual void create_container_variable(LexicalScope *ls);
  virtual int pointer_count();
  virtual void set_pointer_count(int pcount);
  virtual void initialize_type();

  virtual void set_in(bool b);
  virtual void set_out(bool b);
  virtual void set_alloc_caller(bool b);
  virtual void set_alloc_callee(bool b);
  virtual void set_dealloc_caller(bool b);
  virtual void set_dealloc_callee(bool b);
  virtual void set_bind_caller(bool b);
  virtual void set_bind_callee(bool b);

  virtual bool in();
  virtual bool out();
  virtual bool alloc_caller();
  virtual bool alloc_callee();
  virtual bool dealloc_caller();
  virtual bool dealloc_callee();
  virtual bool bind_caller();
  virtual bool bind_callee();

};

class ProjectionType : public Type // complex type
{
 public:
  std::vector<ProjectionField*> channels_;
  const char* id_; 
  const char* real_type_;
  std::vector<ProjectionField*> fields_; 
  ProjectionType();
  ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields, std::vector<ProjectionField*> channels);
  ProjectionType(const char* id, const char* real_type, std::vector<ProjectionField*> fields);
  ProjectionType(const ProjectionType& other);
  virtual Type* clone() const { return new ProjectionType(*this); }
  virtual Marshal_type* accept(MarshalPrepareVisitor *worker);
  virtual CCSTTypeName* accept(TypeNameVisitor *worker);
  virtual CCSTStatement* accept(TypeVisitor *worker, Variable *v);
  virtual const char* name();
  const char* real_type();
  std::vector<ProjectionField*> fields();
  virtual int num();
  virtual void resolve_types(LexicalScope *ls);
  ~ProjectionType(){printf("projection type destructor\n");}
  virtual void create_trampoline_structs(LexicalScope *ls);
  ProjectionField* get_field(const char* field_name);
  void initialize_type();
};

class ProjectionConstructorType : public ProjectionType 
{
 public:
  std::vector<std::pair<Variable*, Variable*> > channel_params_;
  ProjectionConstructorType(const char* id, const char* real_type, std::vector<ProjectionField*> fields, std::vector<ProjectionField*> channel_fields, std::vector<ProjectionField*> channel_params);
  ProjectionConstructorType(const ProjectionConstructorType& other);
  virtual int num();
  virtual Type* clone() const { return new ProjectionConstructorType(*this); }
  virtual void resolve_types(LexicalScope *ls);
  void initialize(std::vector<Variable*> chans);
};

class Rpc : public Base
{
  unsigned int tag_;
  SymbolTable *symbol_table_;
  ReturnVariable *explicit_return_;
  LexicalScope *current_scope_;
  /* -------------- */
  const char* name_;
  std::vector<Parameter* > parameters_;
  
  bool function_pointer_defined_;
  std::vector<Variable*> marshal_projection_parameters(ProjectionType *pt, const char *direction);
  
 public:
  std::vector<Parameter*> hidden_args_;
  Rpc(ReturnVariable *return_var, const char* name, std::vector<Parameter* > parameters, LexicalScope *current_scope);
  void copy_types();
  unsigned int tag();
  void set_tag(unsigned int t);
  void set_function_pointer_defined(bool b);
  void set_hidden_args(std::vector<Parameter*> hidden_args);
  bool function_pointer_defined();
  const char* name();
  const char* enum_name();
  const char* callee_name();
  std::vector<Parameter*> parameters();
  ReturnVariable *return_variable();
  SymbolTable* symbol_table();
  void prepare_marshal();
  void resolve_types();
  void create_trampoline_structs();
  void create_container_variables();
  void set_accessors();
  void initialize_types();
  void set_copy_container_accessors();
  LexicalScope *current_scope();
};

class Module : public Base
{
  // const char *verbatim_;
  const char* module_name_;
  LexicalScope *module_scope_;
  std::vector<GlobalVariable*> channels_;
   // create these from the channels in the constructor.
  std::vector<Rpc*> rpc_definitions_;
 public:
  std::vector<GlobalVariable*> cspaces_;
  GlobalVariable *channel_group;
  Module(const char* id, std::vector<Rpc*> rpc_definitions, std::vector<GlobalVariable*> globals, LexicalScope *ls);
  std::vector<Rpc*> rpc_definitions();  
  std::vector<GlobalVariable*> channels();
  LexicalScope *module_scope();
  void prepare_marshal();
  void resolve_types();
  void function_pointer_to_rpc();
  void create_trampoline_structs();
  void generate_function_tags(Project *p);
  void create_container_variables();
  void copy_types();
  void set_accessors();
  void initialize_types();
  void set_copy_container_accessors();
  const char* identifier();
};

class Include : public Base
{
  bool relative_; // true if "" false for <>
  const char *path_;
 public:
  Include(bool relative, const char* path);
};

class Project : public Base
{
  LexicalScope *project_scope_;
  std::vector<Module*> project_modules_;
  std::vector<Include*> project_includes_;
  unsigned int last_tag_;
  
 public:
  Project(LexicalScope *scope, std::vector<Module*> modules, std::vector<Include*> includes);
  void prepare_marshal();
  void resolve_types();
  void function_pointer_to_rpc();
  void create_trampoline_structs();
  void generate_function_tags();
  void create_container_variables();
  void copy_types();
  void set_accessors();
  void initialize_types();
  void set_copy_container_accessors();
  std::vector<Module*> modules();
  unsigned int get_next_tag();
};

class TypeNameVisitor // generates CCSTTypeName for each type.
{
 public:
  CCSTTypeName* visit(UnresolvedType *ut);
  CCSTTypeName* visit(Typedef *td);
  CCSTTypeName* visit(VoidType *vt);
  CCSTTypeName* visit(IntegerType *it);
  CCSTTypeName* visit(ProjectionType *pt);
  CCSTTypeName* visit(Function *fp);
  CCSTTypeName* visit(Channel *c);
  CCSTTypeName* visit(ProjectionConstructorType *pct);
  CCSTTypeName* visit(InitializeType *it);
};

class TypeVisitor
{
 public:
  virtual CCSTStatement* visit(UnresolvedType *ut, Variable *v) = 0;
  virtual CCSTStatement* visit(Function *fp, Variable *v) = 0;
  virtual CCSTStatement* visit(Typedef *td, Variable *v) = 0;
  virtual CCSTStatement* visit(VoidType *vt, Variable *v) = 0;
  virtual CCSTStatement* visit(IntegerType *it, Variable *v) = 0;
  virtual CCSTStatement* visit(ProjectionType *pt, Variable *v) = 0;
  virtual CCSTStatement* visit(Channel *c, Variable *v) = 0;
  virtual CCSTStatement* visit(ProjectionConstructorType *pct, Variable *v) = 0;
  virtual CCSTStatement* visit(InitializeType *it, Variable *v) = 0;
};

class AllocateTypeVisitor : public TypeVisitor    
{
 public:
  AllocateTypeVisitor();
  virtual CCSTStatement* visit(UnresolvedType *ut, Variable *v);
  virtual CCSTStatement* visit(Function *fp, Variable *v);
  virtual CCSTStatement* visit(Typedef *td, Variable *v);
  virtual CCSTStatement* visit(VoidType *vt, Variable *v);
  virtual CCSTStatement* visit(IntegerType *it, Variable *v);
  virtual CCSTStatement* visit(ProjectionType *pt, Variable *v);
  virtual CCSTStatement* visit(Channel *c, Variable *v);
  virtual CCSTStatement* visit(ProjectionConstructorType *pct, Variable *v);
  virtual CCSTStatement* visit(InitializeType *it, Variable *v);
};

#endif
