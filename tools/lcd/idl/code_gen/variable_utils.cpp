#include "ccst.h"
#include "code_gen.h"

/* 
 * CAST code to declare a static variable
 */
CCSTDeclaration* declare_static_variable(Variable *gv)
{
  
  std::vector<CCSTDecSpecifier*> specifier;
  specifier.push_back(new CCSTStoClassSpecifier(static_t));

  std::vector<CCSTSpecifierQual*> type_for_global = type(gv->type());
  specifier.insert(specifier.end(), type_for_global.begin(), type_for_global.end());

  std::vector<CCSTInitDeclarator*> declarators;
  declarators.push_back(new CCSTDeclarator( pointer(gv->pointer_count()),
					    new CCSTDirectDecId(gv->identifier())));
  
  return new CCSTDeclaration(specifier, declarators);
}

CCSTDeclaration* declare_variable(Variable *v)
{
  std::vector<CCSTInitDeclarator*> decs;
  decs.push_back(new CCSTDeclarator(pointer(v->pointer_count())
				    , new CCSTDirectDecId(v->identifier())));
  
  return new CCSTDeclaration(type2(v->type()), decs);
}


// recursively declare variables for callee glue code.
// declare container if the var has a container.
// otherwise declare the variable and recurse.
std::vector<CCSTDeclaration*> declare_variables_callee(Variable *v)
{
  std::vector<CCSTDeclaration*> declarations;

  if(v->container() != 0x0) {
    printf("declaring container %s\n", v->container()->identifier());
    declarations.push_back(declare_variable(v->container()));
  } else {
    printf("declaring variable %s\n", v->identifier());
    declarations.push_back(declare_variable(v));
  }

  if(v->type()->num() == 4 || v->type()->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");

    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      std::vector<CCSTDeclaration*> tmp_declarations = declare_containers_only_callee(pf);
      declarations.insert(declarations.end(), tmp_declarations.begin(), tmp_declarations.end());
    }
  }
  return declarations;
}

std::vector<CCSTDeclaration*> declare_containers_only_callee(Variable *v)
{
  std::vector<CCSTDeclaration*> declarations;
  
  if(v->container() != 0x0) {
    declarations.push_back(declare_variable(v->container()));
  }
  
  if(v->type()->num() == 4 || v->type()->num() == 9) {
    ProjectionType *pt = dynamic_cast<ProjectionType*>(v->type());
    Assert(pt != 0x0, "Error: dynamic cast to projection type failed\n");
    
    std::vector<ProjectionField*> fields = pt->fields();
    for(std::vector<ProjectionField*>::iterator it = fields.begin(); it != fields.end(); it ++) {
      ProjectionField *pf = *it;
      std::vector<CCSTDeclaration*> tmp_declarations = declare_containers_only_callee(pf);
      declarations.insert(declarations.end(), tmp_declarations.begin(), tmp_declarations.end());
    }
    
  }
  return declarations;
}
