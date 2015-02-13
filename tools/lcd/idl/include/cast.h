class CCSTBase
{
};

class CCSTFile : CCSTBase
{
};

class CCSTExDeclaration : CCSTBase
{
  /*
  <external-declaration> ::= <function-definition>
                         | <declaration>
  */
};

class CCSTFuncDef : public CCSTExDeclaration
{
  /* <function-definition> ::= 
     {<declaration-specifier>}* <declarator> {<declaration>}* <compound-statement>
  */
};

class CCSTDeclaration : public CCSTExDeclaration
{
  /*
    <declaration> ::=  {<declaration-specifier>}+ {<init-declarator>}*
   */
};

class CCSTDecSpecifier : CCSTBase
{
  /*
    <declaration-specifier> ::= <storage-class-specifier>
                          | <type-specifier>
                          | <type-qualifier>
   */

};

class CCSTStoClassSpecifier : CCSTDecSpecifier
{
  /*
    <storage-class-specifier> ::= auto
                            | register
                            | static
                            | extern
                            | typedef
   */
};

class CCSTTypeSpecifier : CCSTDecSpecifier
{
  /*
    <type-specifier> ::= void
                   | char
                   | short
                   | int
                   | long
                   | float
                   | double
                   | signed
                   | unsigned
                   | <struct-or-union-specifier>
                   | <enum-specifier>
                   | <typedef-name>

   */
};

class CCSTStructUnionSpecifier : CCSTTypeSpecifier
{
  /*
  <struct-or-union-specifier> ::= <struct-or-union> <identifier> { {<struct-declaration>}+ }
                              | <struct-or-union> { {<struct-declaration>}+ }
                              | <struct-or-union> <identifier>
  */
};

class CCSTStructUnion : CCSTTypeSpecifier
{
  /*
    <struct-or-union> ::= struct
                    | union
   */
};

class CCSTStructDec : CCSTBase
{
  /*
    <struct-declaration> ::= {<specifier-qualifier>}* <struct-declarator-list>
   */
};

class CCSTSpecifierQual : CCSTBase
{
  /*
    <specifier-qualifier> ::= <type-specifier>
                        | <type-qualifier>
   */
};

class CCSTStructDecList : CCSTBase
{
  /*
    <struct-declarator-list> ::= <struct-declarator>
                           | <struct-declarator-list> , <struct-declarator>
   */
};

class CCSTStructDec : CCSTStructDecList
{
  /*
  <struct-declarator> ::= <declarator>
                      | <declarator> : <constant-expression>
                      | : <constant-expression>
  */
};

class CCSTDeclarator 
{
  /*
    <declarator> ::= {<pointer>}? <direct-declarator>
   */
};

class CCSTPointer
{
  /*
    <pointer> ::= * {<type-qualifier>}* {<pointer>}?
   */
};

// probably does not need to be a class.
class CCSTTypeQualifier
{
  /*
    <type-qualifier> ::= const
                   | volatile
   */
};

class CCSTDirectDec
{
  /*
    <direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )
   */
};

class CCSTConstExpr
{
  /*
    <constant-expression> ::= <conditional-expression>
   */
};

class CCSTLogicalOrExpr
{
  /*
    <logical-or-expression> ::= <logical-and-expression>
                          | <logical-or-expression || <logical-and-expression>
   */
};

class CCSTLogicalAndExpr
{
  /*
    <logical-and-expression> ::= <inclusive-or-expression>
                           | <logical-and-expression && <inclusive-or-expression>
   */ 
};

class CCSTInclusiveOrExpr 
{
  /*
    <inclusive-or-expression> ::= <exclusive-or-expression>
                            | <inclusive-or-expression> | <exclusive-or-expression>
   */

};

class CCSTXorExpr
{
  /*
    <exclusive-or-expression> ::= <and-expression>
                            | <exclusive-or-expression> ^ <and-expression>
   */
};

class CCSTAndExpr 
{
  /*
    
    <and-expression> ::= <equality-expression>
    | <and-expression> & <equality-expression>
   */
};

class CCSTEqExpr
{
  /*
    <equality-expression> ::= <relational-expression>
                        | <equality-expression> == <relational-expression>
                        | <equality-expression> != <relational-expression>
   */
};

class CCSTRelationalExpr
{
  /*
    <relational-expression> ::= <shift-expression>
                          | <relational-expression> < <shift-expression>
                          | <relational-expression> > <shift-expression>
                          | <relational-expression> <= <shift-expression>
                          | <relational-expression> >= <shift-expression>
   */
};

class CCSTShiftExpr
{
  /*
    <shift-expression> ::= <additive-expression>
                     | <shift-expression> << <additive-expression>
                     | <shift-expression> >> <additive-expression>
   */
};

class CCSTAdditiveExpr
{
  /*
  <additive-expression> ::= <multiplicative-expression>
                        | <additive-expression> + <multiplicative-expression>
                        | <additive-expression> - <multiplicative-expression>
  */
};

class CCSTMultExpr
{
  /*
    <multiplicative-expression> ::= <cast-expression>
                              | <multiplicative-expression> * <cast-expression>
                              | <multiplicative-expression> / <cast-expression>
                              | <multiplicative-expression> % <cast-expression>
   */
};

class CCSTCastExpr
{
  /*
    <cast-expression> ::= <unary-expression>
                    | ( <type-name> ) <cast-expression>
   */
  // todo
};

class CCSTUnaryExpr
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
};

class CCSTPostFixExpr
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

class CCSTPrimaryExpr
{
  /*
    <primary-expression> ::= <identifier>
                       | <constant>
                       | <string>
                       | ( <expression> )
   */
  // TODO
};

class CCSTConst : CCSTPrimaryExpr
{
  /*
    <constant> ::= <integer-constant>
             | <character-constant>
             | <floating-constant>
             | <enumeration-constant>
   */
 public:
  virtual void write() = 0;
};

class CCSTInteger : CCSTConst
{
  // TODO
};

class CCSTChar : CCSTConst
{
  // TODO
};

class CCSTFloat : CCSTConst
{
  // TODO
};

class CCSTEnumConst : CCSTConts
{
  //TODO
};

class CCSTExpression
{
  /*
    <expression> ::= <assignment-expression>
               | <expression> , <assignment-expression>
   */
  vector<CCSTAssignExpr> assn_exprs_;
 public:
  CCSTExpression(vector<CCSTAssignExpr> assn){this->assn_exprs = assn;}
  virtual void write();
};

class CCSTAssignExpr
{
  /*
    <assignment-expression> ::= <conditional-expression>
                          | <unary-expression> <assignment-operator> <assignment-expression>
   */
  // TODO
};

//enum assignment_operator { };

class CCSTAssignOp
{
  /*
    <assignment-operator> ::= =
                        | *=
                        | /=
                        | %=
                        | +=
                        | -=
                        | <<=
                        | >>=
                        | &=
                        | ^=
                        | |=

   */
  // TODO
};

class CCSTUnaryOp
{
  /*
    <unary-operator> ::= &
                   | *
                   | +
                   | -
                   | ~
                   | !
   */
  // TODO
};

class CCSTTypeName
{
  /*
    <type-name> ::= {<specifier-qualifier>}+ {<abstract-declarator>}?
   */
  vector<CCSTSpecifierQual> spec_quals_;
  CCSTAbstDeclarator *abs_dec_;
 public:
  CCSTTypeName(vector<CCSTSpecifierQual spec_quals, CCSTAbstDeclarator *abs_dec){this->spec_quals_ = spec_quals; this->abs_dec_ = abs_dec;}
  virtual void write();
};

class CCSTParamTypeList
{
  /*
    <parameter-type-list> ::= <parameter-list>
                        | <parameter-list> , ...
   */
  CCSTParamList *p_list_;
  bool ellipsis_;
 public:
  CCSTParamTypeList(CCSTParamList *p_list, ellipsis){this->p_list_ = p_list; ellipsis_ = ellipsis;}
  virtual void write();
};

class CCSTParamList
{
  /*
  <parameter-list> ::= <parameter-declaration>
                   | <parameter-list> , <parameter-declaration>

  */
  vector<CCSTParamDeclaration> p_dec_;
 public:
  CCSTParamList(vector<CCSTParamDeclaration> p_dec){this->p_dec_ = p_dec;}
  virtual void write();
};

class CCSTParamDeclaration
{
  /*
    <parameter-declaration> ::= {<declaration-specifier>}+ <declarator>
                          | {<declaration-specifier>}+ <abstract-declarator>
                          | {<declaration-specifier>}+
   */
  // TODO
  
};

class CCSTAbstDeclarator
{
  /*
    <abstract-declarator> ::= <pointer>
                        | <pointer> <direct-abstract-declarator>
                        | <direct-abstract-declarator>
   */
  // TODO
};

class CCSTDirectAbstDeclarator
{
  /*
    <direct-abstract-declarator> ::=  ( <abstract-declarator> )
                               | {<direct-abstract-declarator>}? [ {<constant-expression>}? ]
                               | {<direct-abstract-declarator>}? ( {<parameter-type-list>|? )
   */
  // TODO
};

class CCSTEnumSpecifier
{
  /*
  <enum-specifier> ::= enum <identifier> { <enumerator-list> }
                   | enum { <enumerator-list> }
                   | enum <identifier>
  */
  const char* id_;
  CCSTEnumeratorList *el_;
 public:
  CCSTEnumSpecifier(const char* id, CCSTEnumeratorList *el){this->id_ = id; this->el_ = el;}
  CCSTEnumSpecifier(const char* id){this->id_ = id; this->el_ = null;}
  CCSTEnumSpecifier(CCSTEnumeratorList *el){this->el_ = el; this->id_ = "";}
  virtual void write();
};

class CCSTEnumeratorList
{
  /*
    
<enumerator-list> ::= <enumerator>
                    | <enumerator-list> , <enumerator>
   */
  vector<CCSTEnumerator> list_;
 public:
  CCSTEnumeratorList(vector<CCSTEnumerator> list){this->list_ = list;}
  virtual void write();
};

class CCSTEnumerator
{
  /*
    <enumerator> ::= <identifier>
               | <identifier> = <constant-expression>
   */
  const char* id_;
  CCSTConstExpr *ce_;
 public:
  CCSTEnumerator(const char* id, CCSTConstExpr *ce){this->id_ = id; this->ce_ = ce;}
  CCSTEnumerator(const char* id){this->id_ = id; this->ce_ = null;}
  virtual void write();
};

class CCSTTypedefName
{
  /*
    <typedef-name> ::= <identifier>
   */
  const char* id_;
  CCSTTypedefName(const char* name){this->id_ = name;}
};

class CCSTDeclaration
{
  /*
    <declaration> ::=  {<declaration-specifier>}+ {<init-declarator>}*
   */
  // TODO
};

class CCSTInitDeclarator
{
  /*
    
<init-declarator> ::= <declarator>
                    | <declarator> = <initializer>
   */
  CCSTDeclarator *dec_;
  CCSTInitializer *init_;
 public:
  CCSTInitDeclarator(CCSTDeclarator *dec, CCSTInitializer *init){this->dec_ = dec; this->init_ = init;}
  CCSTInitDeclarator(CCSTDeclarator *dec){this->dec_ = dec; this->init_ = null;}
  virtual void write();
};

class CCSTInitializer
{
  /*
    
<initializer> ::= <assignment-expression>
                | { <initializer-list> }
                | { <initializer-list> , }
   */

  // TODO
};

class CCSTInitializerList
{
  // TODO
};

class CCSTCompoundStatement : CCSTStatement
{
  /*
    <compound-statement> ::= { {<declaration>}* {<statement>}* }
   */
  // is this a body?
  vector<CCSTDeclaration> declarations_;
  vector<CCSTStatement> statements_;
 public:
  CCSTCompoundStatement(vector<CCSTDeclaration> decs, vector<CCSTStatement> s){this->declarations_ = decs; this->statements_ = s;}
  virtual void write();
};

class CCSTStatement
{
  /*
    <statement> ::= <labeled-statement>
              | <expression-statement>
              | <compound-statement>
              | <selection-statement>
              | <iteration-statement>
              | <jump-statement>
   */
 public:
  virtual void write() = 0;
};

class CCSTLabeledStatement : CCSTStatement
{
  /*
    <labeled-statement> ::= <identifier> : <statement>
                      | case <constant-expression> : <statement>
                      | default : <statement>
   */
 public:
  virtual void write() = 0;
};

class CCSTPlainLabelStatement : CCSTLabeledStatement
{
  const char* id_;
  CCSTStatement *stmnt_;
 public:
  CCSTPlainLabelStatement(const char* id, CCSTStatement *stmnt){this->id_ = id; this->stmnt_ = stmnt;}
  virtual void write();
};

class CCSTCaseStatement : CCSTLabeledStatement
{
  CCSTConstExpr *case_label_;
  CCSTStatement *body_;
 public:
  CCSTCaseStatement(CCSTConstExpr *c, CCSTStatement *body){this->case_label_ = c; this->body_ = body;}
  virtual void write();
};

class CCSTExprStatement : CCSTStatement
{
  /*
    <expression-statement> ::= {<expression>}? ;
   */
  CCSTExpression *expr_;
 public:
  CCSTExprStatement(CCSTExpression *expr){this->expr_ = expr;}
};

class CCSTSelectionStatement : CCSTStatement
{
  /*
    <selection-statement> ::= if ( <expression> ) <statement>
                        | if ( <expression> ) <statement> else <statement>
                        | switch ( <expression> ) <statement>
   */
  virtual void write() = 0;
};

class CCSTIfStatement : CCSTSelectionStatement
{
  CCSTExpression *cond_;
  CCSTStatement *body_;
 public:
  CCSTIfStatement(CCSTExpression *cond, CCSTStatement *body){this->cond_ = cond; this->body_ = body;}
  virtual void write();
};

class CCSTIfElseStatement : CCSTSelectionStatement
{
  CCSTExpression *cond_;
  CCSTStatement *if_body_;
  CCSTStatement *else_body_;
 public:
  CCSTIfElseStatement(CCSTExpression *cond, CCSTStatement *if_body, CCSTStatement *else_body){this->cond_ = cond; this->if_body_ = if_body; this->else_body_ = else_body;}
  virtual void write();
};

class CCSTSwitchStatement : CCSTSelectionStatement
{
  CCSTExpression *expr_;
  CCSTStatement *body_;
 public:
  CCSTSwitchStatement(CCSTExpression *expr, CCSTStatement *body){this->expr_ = expr; this->body_ = body;}
  virtual void write();
};

class CCSTIterationStmnt : CCSTStatement
{
  /*
    <iteration-statement> ::= while ( <expression> ) <statement>
                        | do <statement> while ( <expression> ) ;
                        | for ( {<expression>}? ; {<expression>}? ; {<expression>}? ) <statement>
   */
 public:
  virtual void write() = 0;
};

class CCSTWhileLoop : CCSTIterationStmnt
{
  CCSTExpression *cond_;
  CCSTStatement *body_;
 public:
  CCSTWhileLoop(CCSTExpression *cond, CCSTStatement *body){this->cond_ = cond; this->body_ = body;}
  virtual void write();
};

class CCSTDoLoop : CCSTIterationStmnt
{
  CCSTExpression *cond_;
  CCSTStatement * body_;
 public:
  CCSTDoLoop(CCSTStatement *body, CCSTExpression *cond){this->body_ = body; this->cond_ = cond;}
  virtual void write();
};

class CCSTForLoop : CCSTIterationStmnt
{
  CCSTExpression *init_;
  CCSTExpression *cond_;
  CCSTExpression *up_;
  CCSTStatement *body_;
 public:
  CCSTForLoop(CCSTExpression *init, CCSTExpression *cond, CCSTExpression *up, CCSTStatement *body){ this->init_ = init; this->cond_ = cond; this->up_ = up; this->body_ = body;}
  virtual void write();
};

class CCSTJumpStmnt : CCSTStatement
{
  /*
    <jump-statement> ::= goto <identifier> ;
                   | continue ;
                   | break ;
                   | return {<expression>}? ;
   */
  
 public:
  virtual void write() = 0; //?
  
};

class CCSTGoto : CCSTJumpStmnt
{
  const char* identifier_;
 public:
  CCSTGoto(const char* id) {this->identifier_ = id;}
  virtual void write();
  
};

class CCSTContinue : CCSTJumpStmnt
{
 public:
  CCSTContinue();
  virtual void write();
};

class CCSTBreak : CCSTJumpStmnt
{
 public:
  CCSTBreak();
  virtual void write();
};

class CCSTReturn : CCSTJumpStmnt
{
  CCSTExpression *expr_;
 public:
  CCSTReturn(CCSTExpression *expr) {this->expr_ = expr;}
  CCSTReturn() {this->expr_ = null;}
  virtual void write();
};
