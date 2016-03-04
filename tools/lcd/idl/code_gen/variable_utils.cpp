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
