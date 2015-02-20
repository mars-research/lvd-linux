class CCSTBase
{
};

class CCSTFile : public CCSTBase
{
  // Other stuff like File
  vector<CCSTExDeclaration> defs_;
 public:
  CCSTFile(vector<CCSTExDeclaration> defs){this->defs_ = defs;}
  virtual void write();
};

class CCSTExDeclaration : public CCSTBase
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

class CCSTDecSpecifier : public CCSTBase
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

class CCSTStructDeclarator : public CCSTStructDecList
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

class CCSTDeclarator : public CCSTStructDeclarator
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

class CCSTPointer : public CCSTAbstDeclarator
{
  /*
    <pointer> ::= * {<type-qualifier>}* {<pointer>}?
   */
  vector<type_qualifier> type_q_;
  CCSTPointer *p_;
 public:
  CCSTPointer(vector<type_qualifier> type_q, CCSTPointer *p){this->type_q_ = type_q; this->p_ = p;}
  CCSTPointer(){this->type_q_ = null; this->p_ = null;}
  CCSTPointer(vector<CCSTTypeQualifier> type_q){this->type_q_ = type_q; this->p_ = null;}
  CCSTPointer(CCSTPointer *p){this->type_q_ = null; this->p_ = p;}
  virtual void write();
};

// probably does not need to be a class.
enum type_qualifer(none_t, const_t, volatile_t);

class CCSTDirectDec : public CCSTBase
{
  /*
    <direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )
   */
 public:
  virtual void write() = 0;
  
};

class CCSTDirectDecId
{
  /*
    <direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )
   */
  const char* id_;
 public:
  CCSTDirectDecId(const char* id){this->id_ = id;}
  virtual void write();

};

class CCSTDirectDecDec : public CCSTDirectDec
{
  /*
    <direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )
   */
  CCSTDeclarator *dec_;
 public:
  CCSTDirectDecDec(CCSTDeclarator *dec){this->dec_ = dec;}
  virtual void write();
  
};
class CCSTDirectDecConstExpr : public CCSTDirectDec
{
  /*
    <direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )
   */
  CCSTDirectDec *direct_dec_;
  CCSTConstExpr *const_expr_; // if null
 public:
  CCSTDirectDecConstExpr(CCSTDirectDec *direct_dec, CCSTConstExpr *const_expr){this->direct_dec_ = direct_dec; this->const_expr_ = const_expr;}
  CCSTDirectDecConstExpr(CCSTDirectDec *direct_dec){this->direct_dec_ = direct_dec; this->const_expr_ = null;}
  virtual void write();

};
class CCSTDirectDecParamTypeList : public CCSTDirectDeclarator
{
  /*
    <direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )
   */
  CCSTDirectDec * direct_dec_;
  CCSTParamTypeList *p_t_list_;
 public:
  CCSTDirectDecParamTypeList(CCSTDirectDec *direct_dec, CCSTParamTypeList *p_t_list){this->direct_dec_ = direct_dec; this->p_t_list_ = p_t_list;}
  virtual void write();

};
class CCSTDirectDecIdList : public CCSTDirectDeclarator
{
  /*
    <direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )
   */
  CCSTDirectDec *direct_dec_;
  vector<char *> ids_;
 public:
  CCSTDirectDecIdList(CCSTDirectDec *direct_dec, vector<char*> ids){this->direct_dec_ = direct_dec; this->ids_ = ids;}
  virtual void write();
  
};


// is this right?
class CCSTConstExpr : public CCSTBase
{
  /*
    <constant-expression> ::= <conditional-expression>
   */
  CCSTCondExpr *cond_expr_;
 public:
  CCSTConstExpr(CCSTCondExpr *cond_expr){this->cond_expr_ = cond_expr;}
  virtual void write();
};

class CCSTCondExpr : public CCSTAssignExpr
{
  /*
    <conditional-expression> ::= <logical-or-expression>
                           | <logical-or-expression> ? <expression> : <conditional-expression>
   */
  CCSTLogicalOrExpr *log_or_expr_;
  CCSTExpr *expr_;
  CCSTCondExpr *cond_expr_;
 public:
  CCSTCondExpr(CCSTLogicalOrExpr *log_or_expr, CCSTExpr *expr, CCSTCondExpr *cond_expr){this->log_or_expr_ = log_or_expr; this->expr_ = expr; this->cond_expr_ = cond_expr;}
};

class CCSTLogicalOrExpr : public CCSTCondExpr
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
 public:
  virtual void write() = 0;
};

class CCSTUnaryExprCastExpr : public CCSTUnaryExpr
{
  /*
    <unary-expression> ::= <postfix-expression>
                     | ++ <unary-expression>
                     | -- <unary-expression>
                     | <unary-operator> <cast-expression>
                     | sizeof <unary-expression>
                     | sizeof <type-name>
   */
  CCSTUnaryOp *unary_op_;
  CCSTCastExpr *cast_expr_;
 public:
  CCSTUnaryExprCastExpr(CCSTUnaryOp *unary_op, CCSTCastExpr *cast_expr){this->unary_op_ = unary_op; this->cast_expr_ = cast_expr;}
  virtual void write();
};

class CCSTUnaryExprOpOp : public CCSTUnaryExpr
{
  /*
    <unary-expression> ::= <postfix-expression>
                     | ++ <unary-expression>
                     | -- <unary-expression>
                     | <unary-operator> <cast-expression>
                     | sizeof <unary-expression>
                     | sizeof <type-name>
   */
  bool plusplus_; // false is minus minus
  CCSTUnaryExpr *unary_expr_;
 public:
  CCSTUnaryExprOpOp(bool plusplus, CCSTUnaryExpr *unary_expr){this->unary_expr_ = unary_expr; this->plusplus_ = plusplus;}
  virtual void write();
};

class CCSTUnaryExprSizeOf : public CCSTCastExpr
{
  /*
    <unary-expression> ::= <postfix-expression>
                     | ++ <unary-expression>
                     | -- <unary-expression>
                     | <unary-operator> <cast-expression>
                     | sizeof <unary-expression>
                     | sizeof <type-name>
   */
  CCSTUnaryExpr *unary_expr_;
  CCSTTypeName *type_name_;
 public:
  CCSTUnaryExprSizeOf(CCSTUnaryExpr *unary_expr){this->unary_expr_ = unary_expr; this->type_name_ = null;}
  CCSTUnaryExprSizeOf(CCSTTypeName *type_name){this->type_name_  = type_name; this->unary_expr_ = null;}
  virtual void write();
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
 public:
  virtual void write() = 0;
};

class CCSTPostFixExprOpOp : public CCSTPostFixExpr
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
  CCSTPostFixExpr *post_fix_expr_;
  bool plusplus_;
 public:
  CCSTPostFixExprOpOp(CCSTPostFixExpr *post_fix_expr, bool plusplus){this->post_fix_expr_ = post_fix_expr; this->plusplus_ = plusplus;}
  virtual void write();
};

class CCSTPostFixExprAccess : public CCSTPostFixExpr
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
  bool pointer_; // -> true . false
  CCSTPostFixExpr *post_fix_expr_;
 public:
  CCSTPostFixExprAccess(CCSTPostFixExpr *post_fix_expr, bool pointer){this->post_fix_expr_ = post_fix_expr; this->pointer_ = pointer;}
  virtual void write();
};

class CCSTPostFixExprExpr : public CCSTPostFixExpr
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
  CCSTPostFixExpr *post_fix_expr_;
  CCSTExpression *expr_;
 public:
  CCSTPostFixExprExpr(CCSTPostFixExpr *post_fix_expr, CCSTExpression *expr){this->post_fix_expr_ = post_fix_expr; this->expr_ = expr;}
  virtual void write();
};

class CCSTPostFixExprAssnExpr : public CCSTPostFixExpr
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
  CCSTPostFixExpr *post_fix_expr_;
  CCSTAssignExpr *assn_expr_;
 public:
  CCSTPostFixExprAssnExpr(CCSTPostFixExpr *post_fix_expr, CCSTAssignExpr *assn_expr){this->post_fix_expr_ = post_fix_expr; this->assn_expr_ = assn_expr;}
  virtual void write();
};

class CCSTPrimaryExpr : public CCSTPostFixExpr
{
  /*
    <primary-expression> ::= <identifier>
                       | <constant>
                       | <string>
                       | ( <expression> )
   */
  CCSTExpression *expr_;
 public:
  CCSTPrimaryExpr(CCSTExpression *expr){this->expr_ = expr;}
  virtual void write();
};

class CCSTString : public CCSTPrimaryExpr
{
  const char* string_;
 public:
  CCSTString(const char* string){this->string_ = string;}
  virtual void write();
};

class CCSTPrimaryExprId : public CCSTPrimaryExpr
{
  const char* id_;
 public:
  CCSTPrimaryExprId(const char* id){this->id_ = id;}
  virtual void write();
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
  
  char c_;
 public:
  CCSTChar(char c){this->c_ = c;}
  virtual void write();
};

class CCSTFloat : public CCSTConst
{
  
  float f_;
  double d_;
  bool float_;
 public:
  CCSTFloat(float f){this->f_ = f; this->float_ = true;}
  CCSTFloat(double d)this->d_ = d; this->float_ = false;}
};

class CCSTEnumConst : public CCSTConts
{
  
  // values in enum?
  const char* enum_val_;
 public:
  CCSTEnumConst(const char* enum_val){this->enum_val_ = enum_val;}
  virtual void write();
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
  CCSTUnaryExpr *unary_expr_;
  CCSTAssignOp *assn_op_;
  CCSTAssignExpr *assn_expr_;
 public:
  CCSTAssignExpr(CCSTUnaryExpr *unary_expr, CCSTAssignOp *assn_op, CCSTAssignExpr *assn_expr){this->unary_expr_ = unary_expr; this->assn_op_ = assn_op; this->assn_expr_ = assn_expr; }
  virtual void write();
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
  vector<CCSTDecSpecifier> dec_specs_;
  CCSTDeclarator *dec_;
  CCSTAbstDeclarator *abs_dec_;
 public:
  CCSTParamDeclaration(vector<CCSTDecSpecifier> dec_specs){this->dec_specs_ = dec_specs;}
  CCSTParamDeclaration(vector<CCSTDecSpecifier> dec_specs, CCSTDeclarator *dec){this->dec_specs_ = dec_specs; this->dec_ = dec; this->abs_dec_ = abs_dec;}
  CCSTParamDeclaration(vector<CCSTDecSpecifier> dec_specs, CCSTAbstDeclarator *abs_dec){this->dec_specs_ = dec_specs; this->abs_dec_ = abs_dec; this->dec_ = dec;}
  virtual void write();
};

class CCSTAbstDeclarator : public CCSTBase
{
  /*
    <abstract-declarator> ::= <pointer>
                        | <pointer> <direct-abstract-declarator>
                        | <direct-abstract-declarator>
   */
  CCSTPointer *p_;
  CCSTDirectAbstDeclarator *d_abs_dec_;
 public:
  CCSTAbstDeclarator(CCSTPointer *p, CCSTDirectAbstDeclarator *d_abs_dec){this->p_ = p; this->d_abs_dec_ = d_abs_dec;}
  virtual void write();
};

class CCSTDirectAbstDeclarator : public CCSTAbstDeclarator
{
  /*
    <direct-abstract-declarator> ::=  ( <abstract-declarator> )
                               | {<direct-abstract-declarator>}? [ {<constant-expression>}? ]
                               | {<direct-abstract-declarator>}? ( {<parameter-type-list>|? )
   */
  
  CCSTAbstDeclarator *abs_dec_;
  CCSTDirectAbstDeclarator *d_abs_dec_;
  CCSTConstExpr *const_expr_;
  CCSTParamTypeList *param_type_list_;
 public:
  CCSTDirectAbstDeclarator(CCSTAbstDeclarator *abs_dec){this->abs_dec_ = abs_dec;}
  CCSTDirectAbstDeclarator(CCSTDirectAbstDeclarator *d_abs_dec, CCSTConstExpr *const_expr){this->d_abs_dec_ = d_abs_dec; this->const_expr_ = const_expr;}
  CCSTDirectAbstDeclarator(CCSTDirectAbstDeclarator *d_abs_dec, CCSTParamTypeList *param_type_list){this->d_abs_dec_ = d_abs_dec; this->param_type_list_ = param_type_list;}
  virtual void write();
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
  CCSTAssignExpr *assn_expr_;
 public:
  CCSTInitializer(CCSTAssignExpr *assn_expr){this->assn_expr_ = assn_expr;}
  virtual void write();
};

class CCSTInitializerList : public CCSTBase
{
  /*
    <initializer-list> ::= <initializer>
                     | <initializer-list> , <initializer>
   */
  vector<CCSTInitializer> init_list_;
 public:
  CCSTInitializerList(vector<CCSTInitializer> init_list){this->init_list_ = init_list;}
  virtual void write();
};

class CCSTCompoundStatement : public CCSTStatement
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

class CCSTStatement : public CCSTBase
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

class CCSTLabeledStatement : public CCSTStatement
{
  /*
    <labeled-statement> ::= <identifier> : <statement>
                      | case <constant-expression> : <statement>
                      | default : <statement>
   */
 public:
  virtual void write() = 0;
};

class CCSTPlainLabelStatement : public CCSTLabeledStatement
{
  const char* id_;
  CCSTStatement *stmnt_;
 public:
  CCSTPlainLabelStatement(const char* id, CCSTStatement *stmnt){this->id_ = id; this->stmnt_ = stmnt;}
  virtual void write();
};

class CCSTCaseStatement : public CCSTLabeledStatement
{
  CCSTConstExpr *case_label_;
  CCSTStatement *body_;
 public:
  CCSTCaseStatement(CCSTConstExpr *c, CCSTStatement *body){this->case_label_ = c; this->body_ = body;}
  virtual void write();
};

class CCSTExprStatement : public CCSTStatement
{
  /*
    <expression-statement> ::= {<expression>}? ;
   */
  CCSTExpression *expr_;
 public:
  CCSTExprStatement(CCSTExpression *expr){this->expr_ = expr;}
};

class CCSTSelectionStatement : public CCSTStatement
{
  /*
    <selection-statement> ::= if ( <expression> ) <statement>
                        | if ( <expression> ) <statement> else <statement>
                        | switch ( <expression> ) <statement>
   */
  virtual void write() = 0;
};

class CCSTIfStatement : public CCSTSelectionStatement
{
  CCSTExpression *cond_;
  CCSTStatement *body_;
 public:
  CCSTIfStatement(CCSTExpression *cond, CCSTStatement *body){this->cond_ = cond; this->body_ = body;}
  virtual void write();
};

class CCSTIfElseStatement : public CCSTSelectionStatement
{
  CCSTExpression *cond_;
  CCSTStatement *if_body_;
  CCSTStatement *else_body_;
 public:
  CCSTIfElseStatement(CCSTExpression *cond, CCSTStatement *if_body, CCSTStatement *else_body){this->cond_ = cond; this->if_body_ = if_body; this->else_body_ = else_body;}
  virtual void write();
};

class CCSTSwitchStatement : public CCSTSelectionStatement
{
  CCSTExpression *expr_;
  CCSTStatement *body_;
 public:
  CCSTSwitchStatement(CCSTExpression *expr, CCSTStatement *body){this->expr_ = expr; this->body_ = body;}
  virtual void write();
};

class CCSTIterationStmnt : public CCSTStatement
{
  /*
    <iteration-statement> ::= while ( <expression> ) <statement>
                        | do <statement> while ( <expression> ) ;
                        | for ( {<expression>}? ; {<expression>}? ; {<expression>}? ) <statement>
   */
 public:
  virtual void write() = 0;
};

class CCSTWhileLoop : public CCSTIterationStmnt
{
  CCSTExpression *cond_;
  CCSTStatement *body_;
 public:
  CCSTWhileLoop(CCSTExpression *cond, CCSTStatement *body){this->cond_ = cond; this->body_ = body;}
  virtual void write();
};

class CCSTDoLoop : public CCSTIterationStmnt
{
  CCSTExpression *cond_;
  CCSTStatement * body_;
 public:
  CCSTDoLoop(CCSTStatement *body, CCSTExpression *cond){this->body_ = body; this->cond_ = cond;}
  virtual void write();
};

class CCSTForLoop : public CCSTIterationStmnt
{ 
  CCSTExpression *init_;
  CCSTExpression *cond_;
  CCSTExpression *up_;
  CCSTStatement *body_;
 public:
  CCSTForLoop(CCSTExpression *init, CCSTExpression *cond, CCSTExpression *up, CCSTStatement *body){ this->init_ = init; this->cond_ = cond; this->up_ = up; this->body_ = body;}
  virtual void write();
};

class CCSTJumpStmnt : public CCSTStatement
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

class CCSTGoto : public CCSTJumpStmnt
{
  const char* identifier_;
 public:
  CCSTGoto(const char* id) {this->identifier_ = id;}
  virtual void write();
  
};

class CCSTContinue : public CCSTJumpStmnt
{
 public:
  CCSTContinue();
  virtual void write();
};

class CCSTBreak : public CCSTJumpStmnt
{
 public:
  CCSTBreak();
  virtual void write();
};

class CCSTReturn : public CCSTJumpStmnt
{
  CCSTExpression *expr_;
 public:
  CCSTReturn(CCSTExpression *expr) {this->expr_ = expr;}
  CCSTReturn() {this->expr_ = null;}
  virtual void write();
};
