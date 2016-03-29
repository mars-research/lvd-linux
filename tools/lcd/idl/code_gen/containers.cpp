#include "ccst.h"
#include "code_gen.h"
#include "lcd_ast.h"

/*
 * code related to container structures.
 */

std::vector<CCSTAssignExpr*> container_of_args(CCSTPostFixExpr *struct_pointer, const char* type_name, const char* field_name)
{
  std::vector<CCSTAssignExpr*> args;
  args.push_back( struct_pointer);
  args.push_back( new CCSTPrimaryExprId(type_name));
  args.push_back( new CCSTPrimaryExprId(field_name));
  return args;
}

std::vector<CCSTStatement*> container_of(Variable *v)
{
  std::vector<CCSTStatement*> statements;
  
  statements.push_back( new CCSTAssignExpr(new CCSTPrimaryExprId(v->container()->identifier())
					   , equals()
					   , function_call("container_of", container_of_args( access(v), struct_name(v->container()->type()->name()), v->type()->name()))));
  
  if (v->type()->num() == 4) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast failed\n");

    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      if( pf->container() != 0x0) {
	std::vector<CCSTStatement*> tmp = container_of(pf);
	statements.insert(statements.end(), tmp.begin(), tmp.end());
      }
    }
  }

  return statements;
}

CCSTCompoundStatement* allocate_and_link_containers(Variable *v)
{
  
}

/* driver function */
CCSTStatement* declare_and_init_variable_callee(Variable *p)
{
  /*
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  
  // TODO: declarations.push_back(); // int "err"
  
  if(p->pointer_count() > 0) {
    return init_variable(p, "callee");
  } else {
    declarations.push_back(declare_variable(p));
    statements.push_back(init_variable(p, "callee"));
  }

  return new CCSTCompoundStatement(declarations, statements);
  */
}

CCSTCompoundStatement* alloc_link_container_callee(Variable *v)
{/*
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  
  /* declare instance of this type with appropriate name 
  
  if(v->container() != 0x0) {
    // allocate container and insert into cspace.
    statements.push_back(alloc_insert_variable_container(v));

    /* if v is a projection loop through fields 
    if(v->type()->num() == 4) {
      ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
      Assert(pt != 0x0, "Error: dynamic cast to projection type failed.\n");
	
      std::vector<ProjectionField*> fields = pt->fields();
      for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
	ProjectionField *pf = *it;

	  // for linking
	// container->real_field.pf_current = ...
	  ProjectionType *tmp = dynamic_cast<ProjectionType*>( real_field(v->container())->type());
	  ProjectionField *tmp_field = find_field(tmp, pf->identifier());

	  if(pf->container() != 0x0) {
	    statements.push_back(alloc_link_container_callee(pf));
	    // link
	    statements.push_back(new CCSTAssignExpr(access(tmp_field)
						    , equals()
						    , access(real_field(pf->container()))));
	  } else if ( pf->pointer_count() > 0 ) { /* need to link pointers 

	    // HEREEEE
	    // 1. declare tmp variable new CCSTPrimaryExprId(append_strings("_"
	    //								   , construct_list_vars(pf)))
	    ProjectionField *tmp_v = new ProjectionField(pf->type()
							 ,append_strings("_"
									 , construct_list_vars(pf))
							 ,pf->pointer_count());
	    declarations.push_back(declare_variable(tmp_v)); // TODO
	    statements.push_back(alloc_variable(tmp_v)); // TODO tmp_v

	    //link
	    statements.push_back(new CCSTAssignExpr(access(tmp_field)
						    , equals()
						    , new CCSTPrimaryExprId(tmp_v->name())));
	  } // construct_list_vars(pf) // append_strings("_", construct_list_vars(pf))
	  
	} // end of loop
	
    }
  } else { /* lookup and init if necessary 
      statements.push_back(lookup_variable_container(v));

      /* if v is a projection loop through fields 
      if(v->type()->num() == 4) {
	ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
	Assert(pt != 0x0, "Error: dynamic cast to projection type failed.\n");
	
	std::vector<ProjectionField*> fields = pt->fields();
	for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
	  ProjectionField *pf = *it;
	  if( alloc_callee(pf, side) || alloc_caller(pf, side)
	      || dealloc_callee(pf, side) || dealloc_caller(pf, side)
	      || in(pf, side) || out(pf, side) ) {
	    /* recurse on field 
	    statements.push_back(init_variable(pf, side)); // is this correct?
	
	  }

	}
	
      } else if ( in(v, side) || out(v, side) ){
	if(v->type()->num() != 5) {
	  statements.push_back( new CCSTAssignExpr( access(real_field(v->container()))
						  , equals()
						  , unmarshal_variable(v)));
	}
      }
    }

    /* set fields/variable 

  } else {
    /* otherwise just set 
    if (v->type()->num() != 4) {
      if(v->pointer_count() > 1) {
	
      }
      
      statements.push_back( new CCSTAssignExpr( access(v)
						, equals()
						, unmarshal_variable(v)));
    } else { // complete I believe 
      /* loop through fields 
      ProjectionType *tmp = dynamic_cast<ProjectionType*>(v->type());
      Assert(tmp != 0x0, "Error: dynamic cast failed.\n");

      std::vector<ProjectionField*> fields = tmp->fields();
      for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
	ProjectionField *pf = *it;
	if (pf->pointer_count() > 0) {
	  statements.push_back(init_variable(pf, side));
	  /* link 
	  statements.push_back(new CCSTAssignExpr(access(pf)
						  , equals()
						  , access(real_field(pf->container()))));

	} else if (in(pf, side) || out(pf, side)) {
	  statements.push_back(declare_init_tmp_variable(pf, side));
	  /* link 
	  statements.push_back(new CCSTAssignExpr(access(pf)
						  , equals()
						  , new CCSTPrimaryExprId(append_strings("_"
											   , construct_list_vars(pf)))));
	}

      }
    } 
  } // end of not pointer

  // end of function
*/
}

/* has already been declared at v->container()->identifier() */
CCSTStatement* lookup_variable_container(Variable *v)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  Assert(v->container() != 0x0, "Error: variable has no container\n");

  std::vector<CCSTAssignExpr*> lookup_args;
  lookup_args.push_back(new CCSTPrimaryExprId("cspace_todo")); // lookup args 1

  ProjectionType *container = dynamic_cast<ProjectionType*>(v->container()->type());
  Assert(container != 0x0, "Error: variables's container does not have type projection\n");
  
  ProjectionField *my_ref_field = container->get_field("my_ref");
  Assert(my_ref_field != 0x0, "Error: could not find my_ref field in projection\n");

  std::vector<CCSTAssignExpr*> __cptr_args;
  __cptr_args.push_back( unmarshal_variable( get_cptr_field( my_ref_field )));

  lookup_args.push_back(function_call("__cptr", __cptr_args)); // ref we get from __cptr(reg_whatever())
  lookup_args.push_back( new CCSTUnaryExprCastExpr( reference()
						    , new CCSTPrimaryExprId(v->container()->identifier() )));

  ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
  Assert(pt != 0x0, "Error: dynamic cast failed.\n");
			
  statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId("err")
					  , equals()
					  , function_call(lookup_name(pt->real_type())
							  , lookup_args)));
  /* do error checking */
  statements.push_back(if_cond_fail(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId("err"))
				    , "lookup"));

}


CCSTStatement* alloc_variable(Variable *v)
{
  AllocateTypeVisitor *worker = new AllocateTypeVisitor();
  return v->type()->accept(worker, v);
}

/* this function allocates the variable's container. 
 * assumes the container has already been declared at v->container()->identifier()
 * inserts container into cspace
 */
CCSTCompoundStatement* alloc_insert_variable_container(Variable *v)
{
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;

  /* Allocate an instance of the container */
  
  /* we only ever allocate containers */
  Assert(v->container() != 0x0, "Error: variable has no container\n");

  /* kzalloc structure */
  std::vector<CCSTAssignExpr*> kzalloc_args;
  kzalloc_args.push_back(new CCSTUnaryExprSizeOf(access(v->container())));
  kzalloc_args.push_back(new CCSTEnumConst("GFP_KERNEL"));

  /* this is ok because container is always a single pointer. */
  statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId(v->container()->identifier())
					  , equals()
					  , function_call("kzalloc", kzalloc_args)));

  /* do error checking */
  statements.push_back(if_cond_fail(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId(v->container()->identifier()))
				    , "kzalloc"));

  /* insert container into cspace */

  /* find my_ref field*/
  ProjectionType *container = dynamic_cast<ProjectionType*>(v->container()->type());
  Assert(container != 0x0, "Error: variables's container does not have type projection\n");
  
  ProjectionField *my_ref_field = container->get_field("my_ref");
  Assert(my_ref_field != 0x0, "Error: could not find my_ref field in projection\n");
  
  std::vector<CCSTAssignExpr*> insert_args;
  insert_args.push_back(new CCSTPrimaryExprId("cspace_todo"));
  insert_args.push_back(new CCSTPrimaryExprId(v->container()->identifier()));
  insert_args.push_back(new CCSTUnaryExprCastExpr(reference()
						  , access(my_ref_field))); // & container->my_ref

  ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
  Assert(pt != 0x0, "Error: dynamic cast to projection type failed.\n");

  statements.push_back(new CCSTAssignExpr(new CCSTPrimaryExprId("err")
					  , equals()
					  , function_call(insert_name(pt->real_type()), insert_args)));

  /* do error checking */
  statements.push_back(if_cond_fail(new CCSTUnaryExprCastExpr(Not(), new CCSTPrimaryExprId("err"))
				    , "lcd insert"));
  
  return new CCSTCompoundStatement(declarations, statements);
}

/* this function's verification isn't very good */
ProjectionField* get_cptr_field(Variable *v)
{
  ProjectionType *cptr_t = dynamic_cast<ProjectionType*>(v->type());
  Assert(cptr_t != 0x0, "Error: dynamic cast to projection type failed.\n");

  ProjectionField* cptr = cptr_t->get_field("cptr");
  Assert(cptr != 0x0, "Error: could not find field cptr.\n");

  return cptr;
}

ProjectionField* real_field(Variable *v, const char* field_name)
{
  ProjectionType *container = dynamic_cast<ProjectionType*>(v->type());
  Assert(container != 0x0, "Error: dynamic cast to projection type failed.\n");

  return find_field(container, field_name);
}

ProjectionField* find_field(ProjectionType *pt, const char* field_name)
{
  ProjectionField *field = pt->get_field(field_name);
  Assert(field != 0x0, "Error: could not find field\n");
  
  return field;
}

/* for declaring tmp variables for projection fields. 
 * to make my life easier
 * only called on things that are not pointers.
 */
CCSTStatement* declare_init_tmp_variable(ProjectionField *pf, const char *side)
{
  /*
  Assert(pf->pointer_count() == 0, "Error: is a pointer\n");

  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  
  /* only called on things that are not pointers 
  std::vector<CCSTInitDeclarator*> decs;
  decs.push_back(new CCSTDeclarator(0x0
				    , new CCSTDirectDecId(append_strings("_"
									 , construct_list_vars(pf)))));
  
  declarations.push_back(new CCSTDeclaration(type2(pf->type()), decs));

  Variable *save_accessor = pf->accessor();
  pf->set_accessor(0x0);
  
  const char* save_name = pf->identifier();
  pf->set_identifier(append_strings("_"
				    , construct_list_vars(pf)));
  
  statements.push_back(init_variable(pf, side));
  
  /* restore 
  pf->set_accessor(save_accessor);
  pf->set_identifier(save_name);
  
  return new CCSTCompoundStatement(declarations, statements);
*/
}

CCSTCompoundStatement* alloc_link_container_caller(Variable *v)
{
  printf("In alloc link container caller. variable is %s\n", v->identifier());
  std::vector<CCSTDeclaration*> declarations;
  std::vector<CCSTStatement*> statements;
  // simple case. v is not a projection
  if(v->type()->num() != 4) {
    if(v->alloc_caller()) {
      return alloc_insert_variable_container(v);
    } else {
      return new CCSTCompoundStatement(declarations, container_of(v));
    }
  }

  if(v->alloc_caller()) {
    // else it is the projection case.
    statements.push_back(alloc_insert_variable_container(v));

    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
    
    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      ProjectionType *tmp = dynamic_cast<ProjectionType*>( real_field(v->container()
								      , v->type()->name())->type());
      ProjectionField *tmp_field = find_field(tmp, pf->identifier());
      if(pf->container() != 0x0) {
	statements.push_back(alloc_link_container_caller(pf));
	// link
	statements.push_back(new CCSTAssignExpr(access(tmp_field)
						, equals()
						, access(real_field(pf->container()
								    , pf->type()->name()))));
      } else if (pf->pointer_count() > 0) {
	// allocate and link
	// declare at a tmp name
	ProjectionField *tmp_v = new ProjectionField(pf->type()
						     ,append_strings("_"
								     , construct_list_vars(pf))
						     ,pf->pointer_count());
	declarations.push_back(declare_variable(tmp_v));
	statements.push_back(alloc_variable(tmp_v));
	statements.push_back(new CCSTAssignExpr(access(tmp_field)
						, equals()
						, new CCSTPrimaryExprId(tmp_v->identifier())));
      }
    }
  } else {
    return new CCSTCompoundStatement(declarations, container_of(v));
  }
  
  return new CCSTCompoundStatement(declarations, statements);
}

 
