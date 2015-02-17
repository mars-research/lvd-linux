class CCSTBase
{
};

class CCSTFile : CCSTBase
{
  // Other stuff like File
  vector<CCSTExDeclaration> defs_;
 public:
  CCSTFile(vector<CCSTExDeclaration> defs){this->defs_ = defs;}
  virtual void write();
};

class CCSTExDeclaration : CCSTBase
{
  /*
  <external-declaration> ::= <function-definition>
                         | <declaration>
  */
 public:
  virtual void write() = 0;
};

class CCSTFuncDef : public CCSTExDeclaration
{
  /* <function-definition> ::= 
     {<declaration-specifier>}* <declarator> {<declaration>}* <compound-statement>
  */
  vector<CCSTDecSpecifier> specifiers_;
  CCSTDeclarator *ret_;
  vector<CCSTDeclaration> decs_;
  CCSTCompoundStatement *body_;
 public:
  CCSTFuncDef(vector<CCSTDecSpecifier> specifiers, CCSTDeclarator *ret, vector<CCSTDeclaration> decs_, CCSTCompoundStatement *body){this->specifiers_ = specifiers; this->ret_ = ret; this->decs_ = decs; this->body_ = body;}
  virtual void write();
};

class CCSTDeclaration : public CCSTExDeclaration
{
  /*
    <declaration> ::=  {<declaration-specifier>}+ {<init-declarator>}*
   */
  vector<CCSTDecSpecifier> specifier_;
  vector<CCSTInitDeclarator> decs_;
 public:
  CCSTDeclaration(vector<CCSTDecSpecifier> specifier, vector<CCSTInitDeclarator> decs){this->specifier_ = specifier; this->decs_ = decs;}
  virtual void write();
};

class CCSTDecSpecifier : CCSTBase
{
  /*
    <declaration-specifier> ::= <storage-class-specifier>
                          | <type-specifier>
                          | <type-qualifier>
   */
 public:
  virtual void write() = 0;

};

enum sto_class_t {auto_t, register_t, static_t, extern_t, typedef_t};

class CCSTStoClassSpecifier : CCSTDecSpecifier
{
  // is this even encessary?
  /*
    <storage-class-specifier> ::= auto
                            | register
                            | static
                            | extern
                            | typedef
   */
  sto_class_t val_;
 public:
  CCSTStoClassSpecifier(sto_class_t val){this->val_ = val;}
  virtual void write();
};

enum type_spec_t {void_t, char_t, short_t, int_t, long_t, float_t, double_t,
		  signed_t, unsigned_t, other_t};

class CCSTTypeSpecifier : CCSTSpecifierQual // slightly different from c_bnf
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
 public:
  virtual void write() = 0;
};

class CCSTSimpleTypeSpecifier : CCSTTypeSpecifier
{
  type_spec_t type_;
 public:
  CCSTSimpleTypeSpecifier(type_spec_t type){this->type_ = type;}
  virtual void write();
};

enum struct_union_t {struct_t, union_t}; // probably unecessary

class CCSTStructUnionSpecifier : CCSTTypeSpecifier
{
  /*
  <struct-or-union-specifier> ::= <struct-or-union> <identifier> { {<struct-declaration>}+ }
                              | <struct-or-union> { {<struct-declaration>}+ }
                              | <struct-or-union> <identifier>
  */
  struct_union_t s_or_u_;
  const char* id_;
  vector<CCSTStructDeclaration> struct_dec_;
 public:
  CCSTStructUnionSpecifier(struct_union_t s_or_u, const char* id){this->s_or_u_ = s_or_u; this->id_ = id; this->struct_dec_ = null;}
  CCSTStructUnionSpecifier(struct_union_t s_or_u, const char* id, vector<CCSTStructDeclaration> struct_dec){this->s_or_u_ = s_or_u; this->id_ = id; this->struct_dec_ = struct_dec;}
  CCSTStructUnionSpecifier(struct_union_t s_or_u, vector<CCSTStructDeclaration> struct_dec){this->s_or_u_ = s_or_u; this->id_ = ""; this->struct_dec_ = struct_dec;}
  virtual void write();
};

class CCSTStructDeclaration : CCSTBase
{
  /*
    <struct-declaration> ::= {<specifier-qualifier>}* <struct-declarator-list>
   */
  vector<CCSTSpecifierQual> spec_qual_;
  CCSTStructDecList *dec_list_;
 public:
  CCSTSTructDeclaration(vector<CCSTSpecifierQual spec_qual, CCSTStructDecList *dec_list){this->spec_qual_ = spec_qual; this->dec_list_ = dec_list;}
  virtual void write();
};

class CCSTSpecifierQual : CCSTDecSpecifier
{
  /*
    <specifier-qualifier> ::= <type-specifier>
                        | <type-qualifier>
   */
 public:
  virtual void write() = 0;
};


class CCSTStructDecList : CCSTBase
{
  /*
    <struct-declarator-list> ::= <struct-declarator>
                           | <struct-declarator-list> , <struct-declarator>
   */
  vector<CCSTStructDeclarator> struct_decs_;
 public:
  CCSTStructDecList(vector<CCSTStructDeclarator> struct_decs){this->struct_decs_ = struct_decs;}
  virtual void write();
};

class CCSTStructDeclarator : CCSTBase
{
  /*
  <struct-declarator> ::= <declarator>
                      | <declarator> : <constant-expression>
                      | : <constant-expression>
  */
  CCSTDeclarator *dec_;
  CCSTConstExpr *expr_;
 public:
  CCSTStructDeclarator(CCSTDeclarator *dec){this->dec_ = dec; this->expr_ = null;}
  CCSTStructDeclarator(CCSTDeclarator *dec, CCSTConstExpr *expr){this->dec_ = dec; this->expr_ = expr;}
  CCSTStructDeclarator(CCSTConstExpr *expr){this->dec_ = null; this->expr_ = expr;}
  virtual void write();
};

class CCSTDeclarator : CCSTBase
{
  /*
    <declarator> ::= {<pointer>}? <direct-declarator>
   */
  CCSTPointer *pointer_;
  CCSTDirectDec *d_dec_;
 public:
  CCSTDeclarator(CCSTPointer *pointer, CCSTDirectDec *d_dec){this->pointer_ = pointer; this->d_dec_ = d_dec;}
  virtual void write();
};

class CCSTPointer : CCSTBase
{
  /*
    <pointer> ::= * {<type-qualifier>}* {<pointer>}?
   */
  vector<CCSTTypeQualifier> type_q_;
  CCSTPointer *p_;
 public:
  CCSTPointer(vector<CCSTTypeQualifier> type_q, CCSTPointer *p){this->type_q_ = type_q; this->p_ = p;}
  CCSTPointer(){this->type_q_ = null; this->p_ = null;}
  CCSTPointer(vector<CCSTTypeQualifier> type_q){this->type_q_ = type_q; this->p_ = null;}
  CCSTPointer(CCSTPointer *p){this->type_q_ = null; this->p_ = p;}
  virtual void write();
};

// probably does not need to be a class.
class CCSTTypeQualifier : CCSTSpecifierQual 
{
  /*
    <type-qualifier> ::= const
                   | volatile
   */
  // TODO this is silly
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
  // TODO, should these be split up

};

// is this right?
class CCSTConstExpr : public CCSTBase
{
  /*
    <constant-expression> ::= <conditional-expression>
   */
 public:
  virtual void write() = 0;
};

class CCSTLogicalOrExpr
{
  /*
    <logical-or-expression> ::= <logical-and-expression>
                          | <logical-or-expression || <logical-and-expression>
   */
  CCSTLogicalAndExpr *and_;
  CCSTLogicalOrExpr *or_;
 public:
  CCSTLogicalOrExpr(CCSTLogicalOrExpr *or, CCSTLogicalAndExpr *and){this->and_ = and; this->or_ = or;}
  virtual void write();
};

class CCSTLogicalAndExpr : public CCSTLogicalOrExpr
{
  /*
    <logical-and-expression> ::= <inclusive-or-expression>
                           | <logical-and-expression && <inclusive-or-expression>
   */
  CCSTLogicalAndExpr *and_;
  CCSTInclusiveOrExpr *or_;
 public:
  CCSTLogicalAndExpr(CCSTLogicalAndExpr *and, CCSTInclusiveOrExpr *or){this->and_ = and; this->or_ = or;}
  virtual void write();
};

class CCSTInclusiveOrExpr : public CCSTLogicalAndExpr
{
  /*
    <inclusive-or-expression> ::= <exclusive-or-expression>
                            | <inclusive-or-expression> | <exclusive-or-expression>
   */
  CCSTInclusiveOrExpr *in_or_;
  CCSTXorExpr *xor_;
 public:
  CCSTInclusiveOrExpr(CCSTInclusiveOrExpr *in_or, CCSTXorExpr *xor){this->in_or_ = in_or; this->xor_ = xor;}
  virtual void write();

};

class CCSTXorExpr : public CCSTInclusiveOrExpr
{
  /*
    <exclusive-or-expression> ::= <and-expression>
                            | <exclusive-or-expression> ^ <and-expression>
   */
  CCSTXorExpr *xor_;
  CCSTAndExpr *and_;
 public:
  CCSTXorExpr(CCSTXorExpr *xor, CCSTAndExpr *and){this->xor_ = xor; this->and_ = and;}
  virtual void write();
};

class CCSTAndExpr : public CCSTXorExpr
{
  /*
    
    <and-expression> ::= <equality-expression>
    | <and-expression> & <equality-expression>
   */
  CCSTAndExpr *and_;
  CCSTEqExpr *eq_;
 public:
  CCSTAndExpr(CCSTAndExpr *and, CCSTEqExpr *eq){this->and_ = and; this->eq_ = eq;}
  virtual void write();
};

class CCSTEqExpr 
{
  /*
    <equality-expression> ::= <relational-expression>
                        | <equality-expression> == <relational-expression>
                        | <equality-expression> != <relational-expression>
   */
  bool equal_;
  CCSTEqExpr *eq_expr_;
  CCSTRelationalExpr *r_expr_;
 public:
  CCSTEqExpr(bool equal, CCSTEqExpr *eq_expr, CCSTRelationalExpr *r_expr){this->equal_ = equal; this->eq_expr_ = eq_expr; this->r_expr_ = r_expr;}
  virtual void write();
  
};

enum relational_op {lessthan_t, greaterthan_t, lessthaneq_t, greaterthaneq_t};
class CCSTRelationalExpr : public CCSTeqExpr
{
  /*
    <relational-expression> ::= <shift-expression>
                          | <relational-expression> < <shift-expression>
                          | <relational-expression> > <shift-expression>
                          | <relational-expression> <= <shift-expression>
                          | <relational-expression> >= <shift-expression>
   */
  relational_op op_;
  CCSTRelationalExpr *r_expr_;
  CCSTShiftExpr *s_expr_;
 public:
  CCSTRelationalExpr(relational_op op, CCSTRelationalExpr *r_expr, CCSTShiftExp *s_expr){this->op_ = op; this->r_expr_ = r_expr; this->s_expr_ = s_expr;}
  virtual void write();
  
};

enum shift_op {leftshift_t, rightshift_t};

class CCSTShiftExpr : public CCSTRelationalExpr
{
  /*
    <shift-expression> ::= <additive-expression>
                     | <shift-expression> << <additive-expression>
                     | <shift-expression> >> <additive-expression>
   */
  shift_op shift_;
  CCSTShiftExpr *s_expr_;
  CCSTAdditiveExpr *a_expr_;
 public:
  CCSTShiftExpr(shift_op shift, CCSTShiftExpr *s_expr, CCSTAdditiveExpr *a_expr){this->shift_ = shift; this->s_expr_ = s_expr; this->a_expr_ = a_expr;}
  virtual void write();
};

enum additive_op { plus_t, minus_t};
class CCSTAdditiveExpr : public CCSTShiftExpr
{ 
  /*
  <additive-expression> ::= <multiplicative-expression>
                        | <additive-expression> + <multiplicative-expression>
                        | <additive-expression> - <multiplicative-expression>
  */
  additive_op op_;
  CCSTAdditiveExpr *a_expr_;
  CCSTMultExpr *m_expr_;
 public:
  CCSTAdditiveExpr(additive_op op, CCSTAdditiveExpr *a_expr, CCSTMultExpr *m_expr){this->op_ = op; this->a_expr_ = a_expr; this->m_expr_ = m_expr;}
  virtual void write();
};

enum mult_op {multiply_t, divide_t, mod_t};

class CCSTMultExpr : public CCSTAdditiveExpr
{
  /*
    <multiplicative-expression> ::= <cast-expression>
                              | <multiplicative-expression> * <cast-expression>
                              | <multiplicative-expression> / <cast-expression>
                              | <multiplicative-expression> % <cast-expression>
   */
  mult_op op_;
  CCSTMultExpr *m_expr_;
  CCSTCastExpr *c_expr_;
 public:
  CCSTMultExpr(mult_op op, CCSTMultExpr *m_expr, CCSTCastExpr *c_expr){this->op_ = op; this->m_expr_ = m_expr; this->c_expr_ = c_expr;}
  virtual void write();
};

class CCSTCastExpr : public CCSTMultExpr
{
  /*
    <cast-expression> ::= <unary-expression>
                    | ( <type-name> ) <cast-expression>
   */
  CCSTTypeName *cast_type_;
  CCSTCastExpr *cast_expr_;
 public:
  CCSTCastExpr(CCSTTypeName *cast_type, CCSTCastExpr *cast_expr){this->cast_type_ = cast_type; this->cast_expr_ = cast_expr;}
  virtual void write();
};

class CCSTUnaryExpr : public CCSTCastExpr
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

class CCSTPostFixExpr : public CCSTUnaryExpr
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

class CCSTPrimaryExpr : public CCSTPostFixExpr
{
  /*
    <primary-expression> ::= <identifier>
                       | <constant>
                       | <string>
                       | ( <expression> )
   */
  // TODO
};

class CCSTConst : public CCSTPrimaryExpr
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

class CCSTInteger : public CCSTConst
{
  // TODO
};

class CCSTChar : public CCSTConst
{
  // TODO
};

class CCSTFloat : public CCSTConst
{
  // TODO
};

class CCSTEnumConst : public CCSTConts
{
  //TODO
};

class CCSTExpression : public CCSTBase
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

class CCSTAssignExpr :: public CCSTExpression
{
  /*
    <assignment-expression> ::= <conditional-expression>
                          | <unary-expression> <assignment-operator> <assignment-expression>
   */
  // TODO
};

enum assign_op {equal_t, mult_eq_t, div_eq_t, mod_eq_t, plus_eq_t, minus_eq_t, lshift_eq_t,
		rshift_eq_t, and_eq_t, xor_eq_t, or_eq_t};

class CCSTAssignOp  : public CCSTBase
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
  assign_op op_;
 public:
  CCSTAssignOp(assign_op op){this->op_ = op;}
  virtual void write();
};

enum unary_op {bit_and_t, mult_t, plus_t, minus_t, tilde_t, bang_t};

class CCSTUnaryOp : public CCSTBase
{
  // probably overkill
  /*
    <unary-operator> ::= &
                   | *
                   | +
                   | -
                   | ~
                   | !
   */
  unary_op op_;
 public:
  CCSTUnaryOP(unary_op op){this->op_ = op;}
  virtual void write();
};

class CCSTTypeName : public CCSTBase
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

class CCSTParamTypeList : public CCSTBase
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

class CCSTParamList : public CCSTParamTypeList
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

class CCSTParamDeclaration : public CCSTParamList
{
  /*
    <parameter-declaration> ::= {<declaration-specifier>}+ <declarator>
                          | {<declaration-specifier>}+ <abstract-declarator>
                          | {<declaration-specifier>}+
   */
  // TODO
  
};

class CCSTAbstDeclarator : public CCSTBase
{
  /*
    <abstract-declarator> ::= <pointer>
                        | <pointer> <direct-abstract-declarator>
                        | <direct-abstract-declarator>
   */
  // TODO
};

class CCSTDirectAbstDeclarator : public CCSTBase
{
  /*
    <direct-abstract-declarator> ::=  ( <abstract-declarator> )
                               | {<direct-abstract-declarator>}? [ {<constant-expression>}? ]
                               | {<direct-abstract-declarator>}? ( {<parameter-type-list>|? )
   */
  // TODO
};

class CCSTEnumSpecifier : public CCSTTypeSpecifier
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

class CCSTEnumeratorList : public CCSTBase
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

class CCSTEnumerator : public CCSTEnumeratorList
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

class CCSTTypedefName : public CCSTTypeSpecifier
{
  /*
    <typedef-name> ::= <identifier>
   */
  const char* id_;
  CCSTTypedefName(const char* name){this->id_ = name;}
};

class CCSTDeclaration : public CCSTExDeclaration
{
  /*
    <declaration> ::=  {<declaration-specifier>}+ {<init-declarator>}*
   */
  // TODO
  vector<CCSTDecSpecifier> dec_spec_;
  CCSTInitDeclarator *init_dec_;
 public:
  CCSTDeclaration(vector<CCSTDecSpecifier> dec_spec, CCSTInitDeclarator *init_dec){this->dec_spec_ = dec_spec; this->init_dec_ = init_dec;}
  virtual void write();
};

class CCSTInitDeclarator : public CCSTBase
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

class CCSTInitializer : public CCSTInitializerList
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
