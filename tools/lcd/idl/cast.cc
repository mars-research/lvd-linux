#include "cast.h"

class CCSTFile::write()
{
}

void CCSTFuncDef::write()
{
}

void CCSTDeclaration::write()
{
}

void CCSTStoClassSpecifier::write()
{
}

void CCSTSimpleTypeSpecifier::write()
{
}

void CCSTStructUnionSpecifier::write()
{
}

void CCSTStructDeclaration::write()
{
}

void CCSTStructDecList::write()
{
}

void CCSTStructDeclarator::write()
{
}

void CCSTDeclarator::write()
{
}

void CCSTPointer::write()
{
}

// probably does not need to be a void.
void CCSTTypeQualifier : CCSTSpecifierQual 
{
  /*
    <type-qualifier> ::= const
                   | volatile
   */
  // TODO this is silly
};

void CCSTDirectDec
{
  /*
    <direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )
   */
  // TODO, should these be split up

};

void CCSTConstExpr::write()
{
}

void CCSTLogicalOrExpr::write()
{
}

void CCSTLogicalAndExpr::write()
{
}

void CCSTInclusiveOrExpr::write()
{
}

void CCSTXorExpr::write()
{
}

void CCSTAndExpr::write()
{
}

void CCSTEqExpr::write()
{
}

void CCSTRelationalExpr::write()
{
}

void CCSTShiftExpr::write()
{
}

void CCSTAdditiveExpr::write()
{ 
}

void CCSTMultExpr::write()
{
}

void CCSTCastExpr::write()
{
}

void CCSTUnaryExpr : public CCSTCastExpr
{
  /*
    <unary-expression> ::= <postfix-expression>
                     | ++ <unary-expression>
                     | -- <unary-expression>
                     | <unary-operator> <cast-expression>
                     | sizeof <unary-expression>
                     | sizeof <type-name>
   */
  // TODO
  //break this up into smaller
};

void CCSTPostFixExpr : public CCSTUnaryExpr
{
  /*
    <postfix-expression> ::= <primary-expression>
                       | <postfix-expression> [ <expression> ]
                       | <postfix-expression> ( {<assignment-expression>}* )
                       | <postfix-expression> . <identifier>
                       | <postfix-expression> -> <identifier>
                       | <postfix-expression> ++
                       | <postfix-expression> --
   */
  // TODO
};

void CCSTPrimaryExpr : public CCSTPostFixExpr
{
  /*
    <primary-expression> ::= <identifier>
                       | <constant>
                       | <string>
                       | ( <expression> )
   */
  // TODO
};

void CCSTInteger : public CCSTConst
{
  // TODO
};

void CCSTChar : public CCSTConst
{
  // TODO
};

void CCSTFloat : public CCSTConst
{
  // TODO
};

void CCSTEnumConst : public CCSTConts
{
  //TODO
};

void CCSTExpression::write()
{
}

void CCSTAssignExpr
{
  /*
    <assignment-expression> ::= <conditional-expression>
                          | <unary-expression> <assignment-operator> <assignment-expression>
   */
  // TODO
};

void CCSTAssignOp::write()
{
}

void CCSTUnaryOp::write()
{
}

void CCSTTypeName::write()
{
}

void CCSTParamTypeList::write()
{
}

void CCSTParamList::write()
{
}

void CCSTParamDeclaration : public CCSTParamList
{
  /*
    <parameter-declaration> ::= {<declaration-specifier>}+ <declarator>
                          | {<declaration-specifier>}+ <abstract-declarator>
                          | {<declaration-specifier>}+
   */
  // TODO
  
};

void CCSTAbstDeclarator : public CCSTBase
{
  /*
    <abstract-declarator> ::= <pointer>
                        | <pointer> <direct-abstract-declarator>
                        | <direct-abstract-declarator>
   */
  // TODO
};

void CCSTDirectAbstDeclarator : public CCSTBase
{
  /*
    <direct-abstract-declarator> ::=  ( <abstract-declarator> )
                               | {<direct-abstract-declarator>}? [ {<constant-expression>}? ]
                               | {<direct-abstract-declarator>}? ( {<parameter-type-list>|? )
   */
  // TODO
};

void CCSTEnumSpecifier::write()
{
}

void CCSTEnumeratorList::write()
{
}

void CCSTEnumerator::write()
{
}

void CCSTTypedefName::write()
{
}

void CCSTDeclaration::write()
{
}

void CCSTInitDeclarator::write()
{
}

void CCSTInitializer : public CCSTInitializerList
{
  /*
    
<initializer> ::= <assignment-expression>
                | { <initializer-list> }
                | { <initializer-list> , }
   */

  // TODO
};

void CCSTInitializerList
{
  // TODO
};

void CCSTCompoundStatement::write()
{
}

void CCSTPlainLabelStatement::write()
{
}

void CCSTCaseStatement::write()
{
}

void CCSTExprStatement::write()
{
}

void CCSTSelectionStatement::write()
{
}

void CCSTIfStatement::write()
{
}

void CCSTIfElseStatement::write()
{
}

void CCSTSwitchStatement::write()
{
  
}

void CCSTWhileLoop::write()
{
  // write while
  // write (
  this->cond_->write();
  // write )
  // should write { ?
  this->body_->write();
  // write }
}

void CCSTDoLoop::write()
{
  // write do
  // should write { here?
  // write {
  this->body_->write();
  // write }
  // write while
  // write ( 
  this->cond_->write();
  // write ) 
  // write ;
}

void CCSTForLoop::write()
{
  // write for (
  if(this->init_ == null)
    this->init_->write();
  // write ;
  if(this->cond_ == null)
    this->cond_->write();
  // write ;
  if(this->up_ == null)
    this->up_->write();
  // write )
  // should i write { here?
  // write {
  this->body_->write();
  // write }
}

void CCSTGoto::write()
{
  // write goto
  // write this->identifier_
  // write ;
}

void CCSTContinue::write()
{
  // write continue ;
}

void CCSTBreak::write()
{
  // write break;
}

void CCSTReturn::write()
{
  if(this->expr_ == null)
    {
      // write return;
    }
  else
    {
      // write return
      this->expr_->write();
      // write semicolon
    }
}
