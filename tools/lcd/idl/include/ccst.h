#ifndef CAST_H
#define CAST_H

#include "lcd_ast.h"
#include "assert.h"

class CCSTFile;
class CCSTAssignExpr;
class CCSTExDeclaration;
class CCSTFuncDef;
class CCSTDeclaration;
class CCSTDecSpecifier;
class CCSTStoClassSpecifier;
class CCSTTypeSpecifier;
class CCSTSimpleTypeSpecifier;
class CCSTStructUnionSpecifier;
class CCSTStructDeclaration;
//class CCSTSpecifierQual;
class CCSTStructDecList;
class CCSTStructDeclarator;
//class CCSTDeclarator;
class CCSTPointer;
class CCSTDirectDeclarator;
class CCSTDirectDecId;
class CCSTDirectDecDec ;
class CCSTDirectDecConstExpr;
class CCSTDirectDecParamTypeList;
class CCSTDirectDecIdList;
class CCSTConstExpr;
class CCSTCondExpr;
class CCSTLogicalOrExpr;

class CCSTLogicalAndExpr;
class CCSTInclusiveOrExpr;
class CCSTXorExpr;

class CCSTAndExpr;
class CCSTEqExpr;
class CCSTRelationalExpr;
class CCSTShiftExpr;
class CCSTAdditiveExpr;
class CCSTMultExpr;
class CCSTCastExpr;
class CCSTUnaryExpr;
class CCSTUnaryExprCastExpr;
class CCSTUnaryExprOpOp;
class CCSTUnaryExprSizeOf;
class CCSTPostFixExpr;
class CCSTPostFixExprOpOp;
class CCSTPostFixExprAccess;
class CCSTPostFixExprExpr;
class CCSTPostFixExprAssnExpr;
class CCSTPrimaryExpr;
class CCSTString;
class CCSTPrimaryExprId;
class CCSTConstant;

class CCSTInteger;
class CCSTChar;
class CCSTFloat;
class CCSTEnumConst;
//class CCSTExpression;
//class CCSTAssignExpr;
class CCSTAssignOp;
class CCSTUnaryOp;

class CCSTTypeName;
class CCSTParamTypeList;
class CCSTParamList;
class CCSTParamDeclaration;
//class CCSTAbstDeclarator;
class CCSTDirectAbstDeclarator;
class CCSTEnumSpecifier;
class CCSTEnumeratorList;
class CCSTEnumerator;
class CCSTTypedefName;
class CCSTExprStatement;

class CCSTDeclaration;
class CCSTInitDeclarator;
class CCSTInitializer;
class CCSTInitializerList;
class CCSTCompoundStatement;
class CCSTStatement;
class CCSTLabeledStatement;
class CCSTPlainLabelStatement;
class CCSTCaseStatement;
class CCSTSelectionStatement;
class CCSTIfStatement;
class CCSTIfElseStatement;
class CCSTSwitchStatement;
class CCSTIterationStmnt;

class CCSTWhileLoop;
class CCSTDoLoop;

class CCSTForLoop;
class CCSTJumpStmnt;
class CCSTGoto;
class CCSTContinue;

class CCSTBreak;
class CCSTReturn;

class CCSTBase
{
 public:
  virtual void write(FILE *f) = 0;
};

class CCSTFile : public CCSTBase
{
  std::vector<CCSTExDeclaration*> defs_;
 public:
  CCSTFile(std::vector<CCSTExDeclaration*> defs); //{this->defs_ = defs;}
  virtual void write(FILE *f);
};

class CCSTExDeclaration : public CCSTBase
{
  /*
  <external-declaration> ::= <function-definition>
                         | <declaration>
  */
 public:
  virtual void write(FILE *f) = 0;
};

class CCSTDeclarator;
class CCSTInitDeclarator : public CCSTBase
{
  /*
    
<init-declarator> ::= <declarator>
                    | <declarator> = <initializer>
   */
  CCSTDeclarator *dec_;
  CCSTInitializer *init_;
 public:
  CCSTInitDeclarator() {};
  CCSTInitDeclarator(CCSTDeclarator *dec, CCSTInitializer *init); //{this->dec_ = dec; this->init_ = init;}
  CCSTInitDeclarator(CCSTDeclarator *dec); //{this->dec_ = dec; this->init_ = NULL;}
  virtual void write(FILE *f);
};

class CCSTDeclarator : public CCSTInitDeclarator // this seems incorrect
{
  /*
    <declarator> ::= {<pointer>}? <direct-declarator>
   */
  CCSTPointer *pointer_;
  CCSTDirectDeclarator *d_dec_;
 public:
  CCSTDeclarator(CCSTPointer *pointer, CCSTDirectDeclarator *d_dec); //{this->pointer_ = pointer; this->d_dec_ = d_dec;}
  virtual void write(FILE *f);
};

class CCSTFuncDef : public CCSTExDeclaration
{
  /* <function-definition> ::= 
     {<declaration-specifier>}* <declarator> {<declaration>}* <compound-statement>
  */
  std::vector<CCSTDecSpecifier*> specifiers_;
  CCSTDeclarator *ret_;
  std::vector<CCSTDeclaration*> decs_;
  CCSTCompoundStatement *body_;
 public:
  CCSTFuncDef(std::vector<CCSTDecSpecifier*> specifiers, CCSTDeclarator *ret, std::vector<CCSTDeclaration*> decs, CCSTCompoundStatement *body); //{this->specifiers_ = specifiers; this->ret_ = ret; this->decs_ = decs; this->body_ = body;}
  virtual void write(FILE *f);
};

class CCSTDeclaration : public CCSTExDeclaration
{
  /*
    <declaration> ::=  {<declaration-specifier>}+ {<init-declarator>}*
   */

 public:
  std::vector<CCSTDecSpecifier*>specifier_;
  std::vector<CCSTInitDeclarator*> decs_;
  CCSTDeclaration(std::vector<CCSTDecSpecifier*> specifier, std::vector<CCSTInitDeclarator*> decs); //{this->specifier_ = specifier; this->decs_ = decs;}
  virtual void write(FILE *f);
};

class CCSTDecSpecifier : public CCSTBase
{
  /*
    <declaration-specifier> ::= <storage-class-specifier>
                          | <type-specifier>
                          | <type-qualifier>
   */
 public:
  virtual void write(FILE *f) = 0;

};

enum sto_class_t {auto_t, register_local_t, static_t, extern_t, typedef_t};

class CCSTStoClassSpecifier : public CCSTDecSpecifier
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
  CCSTStoClassSpecifier(sto_class_t val); //{this->val_ = val;}
  virtual void write(FILE *f);
};

class CCSTSpecifierQual : public CCSTDecSpecifier
{
  /*
    <specifier-qualifier> ::= <type-specifier>
                        | <type-qualifier>
   */
 public:
  virtual void write(FILE *f) = 0;
};


enum type_spec_t {void_t, char_t, short_t, int_t, long_t, float_t, double_t,
		  signed_t, unsigned_t, other_t};

class CCSTTypeSpecifier : public CCSTSpecifierQual // slightly different from c_bnf
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
  virtual void write(FILE *f) = 0;
};

class CCSTSimpleTypeSpecifier : public CCSTTypeSpecifier
{
  type_spec_t type_;
 public:
  CCSTSimpleTypeSpecifier(type_spec_t type); //{this->type_ = type;}
  virtual void write(FILE *f);
};

enum struct_union_t {struct_t, union_t}; // probably unecessary

class CCSTStructUnionSpecifier : public CCSTTypeSpecifier
{
  /*
    <struct-or-union-specifier> ::= <struct-or-union> <identifier> { {<struct-declaration>}+ }
                              | <struct-or-union> { {<struct-declaration>}+ }
                              | <struct-or-union> <identifier>
  */
  struct_union_t s_or_u_;
  const char* id_;
  std::vector<CCSTStructDeclaration*> struct_dec_;
 public:
  CCSTStructUnionSpecifier(struct_union_t s_or_u, const char* id); //{this->s_or_u_ = s_or_u; this->id_ = id;}
  CCSTStructUnionSpecifier(struct_union_t s_or_u, const char* id, std::vector<CCSTStructDeclaration*> struct_dec); //{this->s_or_u_ = s_or_u; this->id_ = id; this->struct_dec_ = struct_dec;}
  CCSTStructUnionSpecifier(struct_union_t s_or_u, std::vector<CCSTStructDeclaration*> struct_dec); //{this->s_or_u_ = s_or_u; this->id_ = ""; this->struct_dec_ = struct_dec;}
  virtual void write(FILE *f);
};

class CCSTStructDeclaration : public CCSTBase
{
  /*
    <struct-declaration> ::= {<specifier-qualifier>}* <struct-declarator-list>
   */
  std::vector<CCSTSpecifierQual*> spec_qual_;
  CCSTStructDecList *dec_list_;
 public:
  CCSTStructDeclaration(std::vector<CCSTSpecifierQual*> spec_qual, CCSTStructDecList *dec_list); //{this->spec_qual_ = spec_qual; this->dec_list_ = dec_list;}
  virtual void write(FILE *f);
};

class CCSTStructDecList : public CCSTBase
{
  /*
    <struct-declarator-list> ::= <struct-declarator>
                           | <struct-declarator-list> , <struct-declarator>
   */
  std::vector<CCSTStructDeclarator*> struct_decs_;
 public:
  CCSTStructDecList();
  CCSTStructDecList(std::vector<CCSTStructDeclarator*> struct_decs); //{this->struct_decs_ = struct_decs;}
  virtual void write(FILE *f);
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
  CCSTStructDeclarator();
  CCSTStructDeclarator(CCSTDeclarator *dec); //{this->dec_ = dec; this->expr_ = NULL;}
  CCSTStructDeclarator(CCSTDeclarator *dec, CCSTConstExpr *expr); //{this->dec_ = dec; this->expr_ = expr;}
  CCSTStructDeclarator(CCSTConstExpr *expr); //{this->dec_ = NULL; this->expr_ = expr;}
  virtual void write(FILE *f);
};

// probably does not need to be a class.
enum type_qualifier {none_t, const_t, volatile_t};

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
  CCSTAbstDeclarator();
  CCSTAbstDeclarator(CCSTPointer *p, CCSTDirectAbstDeclarator *d_abs_dec); //{this->p_ = p; this->d_abs_dec_ = d_abs_dec;}
  virtual void write(FILE *f);
};

class CCSTPointer : public CCSTAbstDeclarator
{
  /*
    <pointer> ::= * {<type-qualifier>}* {<pointer>}?
   */
  std::vector<type_qualifier> type_q_;
  CCSTPointer *p_;

 public:
  CCSTPointer(std::vector<type_qualifier> type_q, CCSTPointer *p); //{this->type_q_ = type_q; this->p_ = p;}
  CCSTPointer(); //{this->p_ = NULL;}
  CCSTPointer(std::vector<type_qualifier> type_q); //{this->type_q_ = type_q; this->p_ = NULL;}
  CCSTPointer(CCSTPointer *p); //{this->p_ = p;}
  virtual void write(FILE *f);
};



class CCSTDirectDeclarator : public CCSTBase
{
  /*
    <direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )
   */
 public:
  virtual void write(FILE *f) = 0;
  
};

class CCSTDirectDecId : public CCSTDirectDeclarator
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
  CCSTDirectDecId(const char* id); //{this->id_ = id;}
  virtual void write(FILE *f);

};

class CCSTDirectDecDec : public CCSTDirectDeclarator
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
  CCSTDirectDecDec(CCSTDeclarator *dec); //{this->dec_ = dec;}
  virtual void write(FILE *f);
  
};
class CCSTDirectDecConstExpr : public CCSTDirectDeclarator
{
  /*
    <direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )
   */
  CCSTDirectDeclarator *direct_dec_;
  CCSTConstExpr *const_expr_; // if NULL
 public:
  CCSTDirectDecConstExpr(CCSTDirectDeclarator *direct_dec, CCSTConstExpr *const_expr); //{this->direct_dec_ = direct_dec; this->const_expr_ = const_expr;}
  CCSTDirectDecConstExpr(CCSTDirectDeclarator *direct_dec); //{this->direct_dec_ = direct_dec; this->const_expr_ = NULL;}
  virtual void write(FILE *f);

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
  CCSTDirectDeclarator * direct_dec_;
  CCSTParamTypeList *p_t_list_;
 public:
  CCSTDirectDecParamTypeList(CCSTDirectDeclarator *direct_dec, CCSTParamTypeList *p_t_list); //{this->direct_dec_ = direct_dec; this->p_t_list_ = p_t_list;}
  virtual void write(FILE *f);

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
  CCSTDirectDeclarator *direct_dec_;
  std::vector<char *> ids_;
 public:
  CCSTDirectDecIdList(CCSTDirectDeclarator *direct_dec, std::vector<char*> ids); //{this->direct_dec_ = direct_dec; this->ids_ = ids;}
  virtual void write(FILE *f);
  
};


// is this right?
class CCSTConstExpr : public CCSTBase
{
  /*
    <constant-expression> ::= <conditional-expression>
   */
  CCSTCondExpr *cond_expr_;
 public:
  CCSTConstExpr(CCSTCondExpr *cond_expr); //{this->cond_expr_ = cond_expr;}
  virtual void write(FILE *f);
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
  virtual void write(FILE *f) = 0;
};

class CCSTExpression;

class CCSTExprStatement : public CCSTStatement
{
  /*
    <expression-statement> ::= {<expression>}? ;
   */
  CCSTExpression *expr_;
 public:
  CCSTExprStatement();
  CCSTExprStatement(CCSTExpression *expr); //{this->expr_ = expr;}
  virtual void write(FILE *f);
};

class CCSTExpression : public CCSTExprStatement
{
  /*
    <expression> ::= <assignment-expression>
               | <expression> , <assignment-expression>
   */
  std::vector<CCSTAssignExpr*> assn_exprs_;
 public:
  CCSTExpression();
  CCSTExpression(std::vector<CCSTAssignExpr*> assn); //{this->assn_exprs_ = assn;}
  virtual void write(FILE *f);
};


class CCSTAssignExpr : public CCSTExpression
{
  /*
    <assignment-expression> ::= <conditional-expression>
                          | <unary-expression> <assignment-operator> <assignment-expression>
   */

  CCSTUnaryExpr *unary_expr_;
  CCSTAssignOp *assn_op_;
  CCSTAssignExpr *assn_expr_;
 public:
    CCSTAssignExpr();
  CCSTAssignExpr(CCSTUnaryExpr *unary_expr, CCSTAssignOp *assn_op, CCSTAssignExpr *assn_expr); //{this->unary_expr_ = unary_expr; this->assn_op_ = assn_op; this->assn_expr_ = assn_expr; }
  virtual void write(FILE *f);
};


class CCSTCondExpr : public CCSTAssignExpr
{
  /*
    <conditional-expression> ::= <logical-or-expression>
                           | <logical-or-expression> ? <expression> : <conditional-expression>
   */
  CCSTLogicalOrExpr *log_or_expr_;
  CCSTExpression *expr_;
  CCSTCondExpr *cond_expr_;
 public:
  CCSTCondExpr();
  CCSTCondExpr(CCSTLogicalOrExpr *log_or_expr, CCSTExpression *expr, CCSTCondExpr *cond_expr); //{this->log_or_expr_ = log_or_expr; this->expr_ = expr; this->cond_expr_ = cond_expr;}
  virtual void write(FILE *f);
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
  CCSTLogicalOrExpr();
  CCSTLogicalOrExpr(CCSTLogicalOrExpr *or__, CCSTLogicalAndExpr *and__); //{this->and_ = and__; this->or_ = or__;}
  virtual void write(FILE *f);
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
  CCSTLogicalAndExpr();
  CCSTLogicalAndExpr(CCSTLogicalAndExpr *and__, CCSTInclusiveOrExpr *or__); //{this->and_ = and; this->or_ = or;}
  virtual void write(FILE *f);
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
  CCSTInclusiveOrExpr();
  CCSTInclusiveOrExpr(CCSTInclusiveOrExpr *in_or, CCSTXorExpr *xor__); //{this->in_or_ = in_or; this->xor_ = xor;}
  virtual void write(FILE *f);

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
  CCSTXorExpr();
  CCSTXorExpr(CCSTXorExpr *xor__, CCSTAndExpr *and__); //{this->xor_ = xor; this->and_ = and;}
  virtual void write(FILE *f);
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
  CCSTAndExpr();
  CCSTAndExpr(CCSTAndExpr *and__, CCSTEqExpr *eq); //{this->and_ = and; this->eq_ = eq;}
  virtual void write(FILE *f);
};

class CCSTEqExpr : public CCSTAndExpr
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
  CCSTEqExpr();
  CCSTEqExpr(bool equal, CCSTEqExpr *eq_expr, CCSTRelationalExpr *r_expr); //{this->equal_ = equal; this->eq_expr_ = eq_expr; this->r_expr_ = r_expr;}
  virtual void write(FILE *f);
  
};

class CCSTShiftExpr;

enum relational_op {lessthan_t, greaterthan_t, lessthaneq_t, greaterthaneq_t};
class CCSTRelationalExpr : public CCSTEqExpr
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
  CCSTRelationalExpr();
  CCSTRelationalExpr(relational_op op, CCSTRelationalExpr *r_expr, CCSTShiftExpr *s_expr); //{this->op_ = op; this->r_expr_ = r_expr; this->s_expr_ = s_expr;}
  virtual void write(FILE *f);
  
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
  CCSTShiftExpr();
  CCSTShiftExpr(shift_op shift, CCSTShiftExpr *s_expr, CCSTAdditiveExpr *a_expr); //{this->shift_ = shift; this->s_expr_ = s_expr; this->a_expr_ = a_expr;}
  virtual void write(FILE *f);
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
  CCSTAdditiveExpr();
  CCSTAdditiveExpr(additive_op op, CCSTAdditiveExpr *a_expr, CCSTMultExpr *m_expr); //{this->op_ = op; this->a_expr_ = a_expr; this->m_expr_ = m_expr;}
  virtual void write(FILE *f);
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
  CCSTMultExpr();
  CCSTMultExpr(mult_op op, CCSTMultExpr *m_expr, CCSTCastExpr *c_expr); //{this->op_ = op; this->m_expr_ = m_expr; this->c_expr_ = c_expr;}
  virtual void write(FILE *f);
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
  CCSTCastExpr();
  CCSTCastExpr(CCSTTypeName *cast_type, CCSTCastExpr *cast_expr); //{this->cast_type_ = cast_type; this->cast_expr_ = cast_expr;}
  virtual void write(FILE *f);
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
  virtual void write(FILE *f) = 0;
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
  // *name
  CCSTUnaryOp *unary_op_;
  CCSTCastExpr *cast_expr_;
 public:
  CCSTUnaryExprCastExpr();
  CCSTUnaryExprCastExpr(CCSTUnaryOp *unary_op, CCSTCastExpr *cast_expr); //{this->unary_op_ = unary_op; this->cast_expr_ = cast_expr;}
  virtual void write(FILE *f);
};

enum incr_decr_ops {increment_t, decrement_t};
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
  incr_decr_ops op_;
  CCSTUnaryExpr *unary_expr_;
 public:
  CCSTUnaryExprOpOp();
  CCSTUnaryExprOpOp(incr_decr_ops op, CCSTUnaryExpr *unary_expr); //{this->unary_expr_ = unary_expr; this->op_ = op;}
  virtual void write(FILE *f);
};

class CCSTUnaryExprSizeOf : public CCSTUnaryExpr
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
  CCSTUnaryExprSizeOf();
  CCSTUnaryExprSizeOf(CCSTUnaryExpr *unary_expr); //{this->unary_expr_ = unary_expr; this->type_name_ = NULL;}
  CCSTUnaryExprSizeOf(CCSTTypeName *type_name); //{this->type_name_  = type_name; this->unary_expr_ = NULL;}
  virtual void write(FILE *f);
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
  virtual void write(FILE *f) = 0;
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
 incr_decr_ops op_;
 public:
 CCSTPostFixExprOpOp();
 CCSTPostFixExprOpOp(CCSTPostFixExpr *post_fix_expr, incr_decr_ops op); //{this->post_fix_expr_ = post_fix_expr; this->op_ = op;}
  virtual void write(FILE *f);
};

enum accessor {pointer_access_t, object_access_t};

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
  accessor op_; 
  CCSTPostFixExpr *post_fix_expr_;
  const char * id_;
 public:
  CCSTPostFixExprAccess();
  CCSTPostFixExprAccess(CCSTPostFixExpr *post_fix_expr, accessor op, const char * id); //{this->post_fix_expr_ = post_fix_expr; this->op_ = op;}
  virtual void write(FILE *f);
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
  CCSTPostFixExprExpr();
  CCSTPostFixExprExpr(CCSTPostFixExpr *post_fix_expr, CCSTExpression *expr);
  virtual void write(FILE *f);
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
  std::vector<CCSTAssignExpr*> args_;
 public:
  CCSTPostFixExprAssnExpr();
  CCSTPostFixExprAssnExpr(CCSTPostFixExpr *post_fix_expr, std::vector<CCSTAssignExpr*> args); //{this->post_fix_expr_ = post_fix_expr; this->assn_expr_ = assn_expr;}
  virtual void write(FILE *f);
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
  CCSTPrimaryExpr();
  CCSTPrimaryExpr(CCSTExpression *expr); //{this->expr_ = expr;}
  virtual void write(FILE *f);
};

class CCSTString : public CCSTPrimaryExpr
{
  const char* string_;
 public:
  CCSTString();
  CCSTString(const char* string); //{this->string_ = string;}
  virtual void write(FILE *f);
};

class CCSTPrimaryExprId : public CCSTPrimaryExpr
{
  const char* id_;
 public:
  CCSTPrimaryExprId();
  CCSTPrimaryExprId(const char* id); //{this->id_ = id;}
  virtual void write(FILE *f);
};

class CCSTConstant : public CCSTPrimaryExpr
{
  /*
    <constant> ::= <integer-constant>
             | <character-constant>
             | <floating-constant>
             | <enumeration-constant>
   */
 public:
  virtual void write(FILE *f) = 0;
};

class CCSTInteger : public CCSTConstant
{
  int integer_;
 public:
  CCSTInteger();
  CCSTInteger(int i);
  virtual void write(FILE *f);
};

class CCSTChar : public CCSTConstant
{
  
  char c_;
 public:
  CCSTChar();
  CCSTChar(char c); //{this->c_ = c;}
  virtual void write(FILE *f);
};

class CCSTFloat : public CCSTConstant
{
  
  float f_;
  double d_;
  bool float_;
 public:
  CCSTFloat();
  CCSTFloat(float f); //{this->f_ = f; this->float_ = true;}
  CCSTFloat(double d); //{this->d_ = d; this->float_ = false;}
  virtual void write(FILE *f);
};

class CCSTEnumConst : public CCSTConstant
{
  
  // values in enum?
  const char* enum_val_;
 public:
  CCSTEnumConst();
  CCSTEnumConst(const char* enum_val); //{this->enum_val_ = enum_val;}
  virtual void write(FILE *f);
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
  CCSTAssignOp();
  CCSTAssignOp(assign_op op); //{this->op_ = op;}
  virtual void write(FILE *f);
};

enum unary_op {unary_bit_and_t, unary_mult_t, unary_plus_t, unary_minus_t, unary_tilde_t, unary_bang_t};

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
  CCSTUnaryOp();
  CCSTUnaryOp(unary_op op); //{this->op_ = op;}
  virtual void write(FILE *f);
};

class CCSTTypeName : public CCSTBase
{
  /*
    <type-name> ::= {<specifier-qualifier>}+ {<abstract-declarator>}?
   */
  std::vector<CCSTSpecifierQual*> spec_quals_;
  CCSTAbstDeclarator *abs_dec_;
 public:
  CCSTTypeName();
  CCSTTypeName(std::vector<CCSTSpecifierQual*> spec_quals, CCSTAbstDeclarator *abs_dec); //{this->spec_quals_ = spec_quals; this->abs_dec_ = abs_dec;}
  virtual void write(FILE *f);
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
  CCSTParamTypeList();
  CCSTParamTypeList(CCSTParamList *p_list, bool ellipsis); //{this->p_list_ = p_list; ellipsis_ = ellipsis;}
  virtual void write(FILE *f);
};

class CCSTParamList : public CCSTParamTypeList
{
  /*
  <parameter-list> ::= <parameter-declaration>
                   | <parameter-list> , <parameter-declaration>

  */
  std::vector<CCSTParamDeclaration*> p_dec_;
 public:
  CCSTParamList();
  CCSTParamList(std::vector<CCSTParamDeclaration*> p_dec); //{this->p_dec_ = p_dec;}
  virtual void write(FILE *f);
};

class CCSTParamDeclaration : public CCSTParamList
{
  /*
    <parameter-declaration> ::= {<declaration-specifier>}+ <declarator>
                          | {<declaration-specifier>}+ <abstract-declarator>
                          | {<declaration-specifier>}+
   */
  std::vector<CCSTDecSpecifier*> dec_specs_;
  CCSTDeclarator *dec_;
  CCSTAbstDeclarator *abs_dec_;
 public:
  CCSTParamDeclaration();
  CCSTParamDeclaration(std::vector<CCSTDecSpecifier*> dec_specs); //{this->dec_specs_ = dec_specs;}
  CCSTParamDeclaration(std::vector<CCSTDecSpecifier*> dec_specs, CCSTDeclarator *dec); //{this->dec_specs_ = dec_specs; this->dec_ = dec; this->abs_dec_ = abs_dec;}
  CCSTParamDeclaration(std::vector<CCSTDecSpecifier*> dec_specs, CCSTAbstDeclarator *abs_dec); //{this->dec_specs_ = dec_specs; this->abs_dec_ = abs_dec; this->dec_ = dec;}
  virtual void write(FILE *f);
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
  CCSTDirectAbstDeclarator();
  CCSTDirectAbstDeclarator(CCSTAbstDeclarator *abs_dec); //{this->abs_dec_ = abs_dec;}
  CCSTDirectAbstDeclarator(CCSTDirectAbstDeclarator *d_abs_dec, CCSTConstExpr *const_expr); //{this->d_abs_dec_ = d_abs_dec; this->const_expr_ = const_expr;}
  CCSTDirectAbstDeclarator(CCSTDirectAbstDeclarator *d_abs_dec, CCSTParamTypeList *param_type_list); //{this->d_abs_dec_ = d_abs_dec; this->param_type_list_ = param_type_list;}
  virtual void write(FILE *f);
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
  CCSTEnumSpecifier();
  CCSTEnumSpecifier(const char* id, CCSTEnumeratorList *el); //{this->id_ = id; this->el_ = el;}
  CCSTEnumSpecifier(const char* id); //{this->id_ = id; this->el_ = NULL;}
  CCSTEnumSpecifier(CCSTEnumeratorList *el); //{this->el_ = el; this->id_ = "";}
  virtual void write(FILE *f);
};

class CCSTEnumeratorList : public CCSTBase
{
  /*
    
<enumerator-list> ::= <enumerator>
                    | <enumerator-list> , <enumerator>
   */
  std::vector<CCSTEnumerator*> *list_;
 public:
  CCSTEnumeratorList();
  CCSTEnumeratorList(std::vector<CCSTEnumerator*> *list); //{this->list_ = list;}
  virtual void write(FILE *f);
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
  CCSTEnumerator(const char* id, CCSTConstExpr *ce); //{this->id_ = id; this->ce_ = ce;}
  CCSTEnumerator(const char* id); //{this->id_ = id; this->ce_ = NULL;}
  virtual void write(FILE *f);
};

class CCSTTypedefName : public CCSTTypeSpecifier
{
  /*
    <typedef-name> ::= <identifier>
   */
  const char* id_;
 public:
  CCSTTypedefName(const char* name); //{this->id_ = name;}
  virtual void write(FILE *f);
};



class CCSTInitializerList : public CCSTBase
{
  /*
    <initializer-list> ::= <initializer>
                     | <initializer-list> , <initializer>
   */
  std::vector<CCSTInitializer*> init_list_;
 public:
  CCSTInitializerList();
  CCSTInitializerList(std::vector<CCSTInitializer*> init_list); //{this->init_list_ = init_list;}
  virtual void write(FILE *f);
};

class CCSTInitializer : public CCSTInitializerList
{
  /*
    
<initializer> ::= <assignment-expression>
                | { <initializer-list> }
                | { <initializer-list> , }
   */
  CCSTAssignExpr *assn_expr_;
  CCSTInitializerList *init_list_;
 public:
  CCSTInitializer(CCSTAssignExpr *assn_expr); //{this->assn_expr_ = assn_expr;}
  CCSTInitializer(CCSTInitializerList *init_list); //{this->init_list_ = init_list;}
  virtual void write(FILE *f);
};

class CCSTCompoundStatement : public CCSTStatement
{
  /*
    <compound-statement> ::= { {<declaration>}* {<statement>}* }
   */
  // is this a body? not necessarily
  std::vector<CCSTDeclaration*> declarations_;
  std::vector<CCSTStatement*> statements_;
 public:
  CCSTCompoundStatement(std::vector<CCSTDeclaration*> decs, std::vector<CCSTStatement*> s); //{this->declarations_ = decs; this->statements_ = s;}
  void add_statement(CCSTStatement *s) { this->statements_.push_back(s); }
  virtual void write(FILE *f);
};



class CCSTLabeledStatement : public CCSTStatement
{
  /*
    <labeled-statement> ::= <identifier> : <statement>
                      | case <constant-expression> : <statement>
                      | default : <statement>
   */
 public:
  virtual void write(FILE *f) = 0;
};

class CCSTDefaultLabelStatement : public CCSTLabeledStatement
{
  CCSTStatement* body_;
 public:
  CCSTDefaultLabelStatement(CCSTStatement* body);
  virtual void write(FILE *f);
};

class CCSTPlainLabelStatement : public CCSTLabeledStatement
{
  const char* id_;
  CCSTStatement *stmnt_;
 public:
  CCSTPlainLabelStatement(const char* id, CCSTStatement *stmnt); //{this->id_ = id; this->stmnt_ = stmnt;}
  virtual void write(FILE *f);
};

class CCSTCaseStatement : public CCSTLabeledStatement
{
  CCSTCondExpr *case_label_;
  CCSTStatement *body_;
 public:
  CCSTCaseStatement(CCSTCondExpr *c, CCSTStatement *body); //{this->case_label_ = c; this->body_ = body;}
  virtual void write(FILE *f);
};



class CCSTSelectionStatement : public CCSTStatement
{
  /*
    <selection-statement> ::= if ( <expression> ) <statement>
                        | if ( <expression> ) <statement> else <statement>
                        | switch ( <expression> ) <statement>
   */
  virtual void write(FILE *f) = 0;
};

class CCSTIfStatement : public CCSTSelectionStatement
{
  CCSTExpression *cond_;
  CCSTStatement *body_;
 public:
  CCSTIfStatement(CCSTExpression *cond, CCSTStatement *body); //{this->cond_ = cond; this->body_ = body;}
  virtual void write(FILE *f);
};

class CCSTIfElseStatement : public CCSTSelectionStatement
{
  CCSTExpression *cond_;
  CCSTStatement *if_body_;
  CCSTStatement *else_body_;
 public:
  CCSTIfElseStatement(CCSTExpression *cond, CCSTStatement *if_body, CCSTStatement *else_body); //{this->cond_ = cond; this->if_body_ = if_body; this->else_body_ = else_body;}
  virtual void write(FILE *f);
};

class CCSTSwitchStatement : public CCSTSelectionStatement
{
  CCSTExpression *expr_;
  CCSTStatement *body_;
 public:
  CCSTSwitchStatement(CCSTExpression *expr, CCSTStatement *body); //{this->expr_ = expr; this->body_ = body;}
  virtual void write(FILE *f);
};

class CCSTIterationStmnt : public CCSTStatement
{
  /*
    <iteration-statement> ::= while ( <expression> ) <statement>
                        | do <statement> while ( <expression> ) ;
                        | for ( {<expression>}? ; {<expression>}? ; {<expression>}? ) <statement>
   */
 public:
  virtual void write(FILE *f) = 0;
};

class CCSTWhileLoop : public CCSTIterationStmnt
{
  CCSTExpression *cond_;
  CCSTStatement *body_;
 public:
  CCSTWhileLoop(CCSTExpression *cond, CCSTStatement *body); //{this->cond_ = cond; this->body_ = body;}
  virtual void write(FILE *f);
};

class CCSTDoLoop : public CCSTIterationStmnt
{
  CCSTExpression *cond_;
  CCSTStatement * body_;
 public:
  CCSTDoLoop(CCSTStatement *body, CCSTExpression *cond); //{this->body_ = body; this->cond_ = cond;}
  virtual void write(FILE *f);
};

class CCSTForLoop : public CCSTIterationStmnt
{ 
  CCSTExpression *init_;
  CCSTExpression *cond_;
  CCSTExpression *up_;
  CCSTStatement *body_;
 public:
  CCSTForLoop(CCSTExpression *init, CCSTExpression *cond, CCSTExpression *up, CCSTStatement *body); //{ this->init_ = init; this->cond_ = cond; this->up_ = up; this->body_ = body;}
  virtual void write(FILE *f);
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
  virtual void write(FILE *f) = 0; //?
  
};

class CCSTGoto : public CCSTJumpStmnt
{
  const char* identifier_;
 public:
  CCSTGoto(const char* id); // {this->identifier_ = id;}
  virtual void write(FILE *f);
  
};

class CCSTContinue : public CCSTJumpStmnt
{
 public:
  CCSTContinue();
  virtual void write(FILE *f);
};

class CCSTBreak : public CCSTJumpStmnt
{
 public:
  CCSTBreak();
  virtual void write(FILE *f);
};

class CCSTReturn : public CCSTJumpStmnt
{
  CCSTExpression *expr_;
 public:
  CCSTReturn(CCSTExpression *expr);// {this->expr_ = expr;}
  CCSTReturn(); // {this->expr_ = NULL;}
  virtual void write(FILE *f);
};

#endif
