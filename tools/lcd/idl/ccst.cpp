#include "ccst.h"
#include <string.h>

CCSTFile::CCSTFile(std::vector<CCSTExDeclaration*> defs)
{
  this->defs_ = defs;
}

void CCSTFile::write(FILE *f, int indent)
{
  /*
    for(std::vector<T>::iterator it = v.begin(); it != v.end(); ++it) {
   */
  for(std::vector<CCSTExDeclaration*>::iterator it = defs_.begin(); it != defs_.end(); ++it)
    {
      CCSTExDeclaration *ex_dec = *it;
      ex_dec->write(f, 0);
    }
}

CCSTFuncDef::CCSTFuncDef(std::vector<CCSTDecSpecifier*> specifiers, CCSTDeclarator *ret, std::vector<CCSTDeclaration*> decs, CCSTCompoundStatement *body)
{
  this->specifiers_ = specifiers; 
  this->ret_ = ret; 
  this->decs_ = decs; 
  this->body_ = body;
}

void CCSTFuncDef::write(FILE *f, int indent)
{
  printf("here in function def\n");
  // fprintf(f, "\n");
  for(std::vector<CCSTDecSpecifier*>::iterator it = specifiers_.begin(); it != specifiers_.end(); ++it)
    {
      CCSTDecSpecifier *ds = *it;
      ds->write(f, indent);
    }
  this->ret_->write(f, 0);
  
  for(std::vector<CCSTDeclaration*>::iterator it = decs_.begin(); it != decs_.end(); ++it)
    {
      CCSTDeclaration *ds = *it;
      ds->write(f, 0);
    }
  // write body
  fprintf(f, "\n");
  fprintf(f, "{\n");
  this->body_->write(f, indent+1);
  fprintf(f, "\n");
  fprintf(f, "}");
  fprintf(f, "\n");
}

CCSTDeclaration::CCSTDeclaration(std::vector<CCSTDecSpecifier*> specifier, std::vector<CCSTInitDeclarator*> decs)
{
  this->specifier_ = specifier; 
  this->decs_ = decs;
}

CCSTStoClassSpecifier::CCSTStoClassSpecifier(sto_class_t val)
{
  this->val_ = val;
}

void CCSTStoClassSpecifier::write(FILE *f, int indent)
{
  fprintf(f, "%s", indentation(indent));
  switch (this->val_)
    {
    case auto_t:
      fprintf(f, "%s ", "auto");
      break;
    case register_local_t:
      fprintf(f, "%s ", "register");
      break;
    case static_t:
      fprintf(f, "%s ", "static");
      break;
    case extern_t:
      fprintf(f, "%s ", "extern");
      break;
    case typedef_t:
      fprintf(f, "%s ", "typedef");
      break;
    default:
      fprintf(f, "%s ", "error");
      break;
    } 

}

CCSTSimpleTypeSpecifier::CCSTSimpleTypeSpecifier(type_spec_t type)
{
  this->type_ = type;
}

void CCSTSimpleTypeSpecifier::write(FILE *f, int indent)
{
  fprintf(f, "%s", indentation(indent));
  switch (this->type_)
    {
    case void_t:
      fprintf(f, "%s ", "void");
      break;
    case char_t:
      fprintf(f, "%s ", "char");
      break;
    case short_t:
      fprintf(f, "%s ", "short");
      break;
    case int_t:
      fprintf(f, "%s ", "int");
      break;
    case long_t:
      fprintf(f, "%s ", "long");
      break;
    case float_t:
      fprintf(f, "%s ", "float");
      break;
    case double_t:
      fprintf(f, "%s ", "double");
      break;
    case signed_t:
      fprintf(f, "%s ", "signed");
      break;
    case unsigned_t:
      fprintf(f, "%s ", "unsigned");
      break;
    default:
      fprintf(f, "%s ", "error");
      break;
    } 
}

CCSTStructUnionSpecifier::CCSTStructUnionSpecifier(struct_union_t s_or_u, const char* id)
{
  this->s_or_u_ = s_or_u; 
  this->id_ = id;
}

CCSTStructUnionSpecifier::CCSTStructUnionSpecifier(struct_union_t s_or_u, const char* id, std::vector<CCSTStructDeclaration*> struct_dec)
{
  this->s_or_u_ = s_or_u; 
  this->id_ = id; 
  this->struct_dec_ = struct_dec;
}

CCSTStructUnionSpecifier::CCSTStructUnionSpecifier(struct_union_t s_or_u, std::vector<CCSTStructDeclaration*> struct_dec)
{
  this->s_or_u_ = s_or_u; 
  this->id_ = ""; 
  this->struct_dec_ = struct_dec;
}

void CCSTStructUnionSpecifier::write(FILE *f, int indent)
{
  switch (this->s_or_u_)
    {
    case struct_t:
      {
	//  <struct-or-union> <identifier> what about this case
	fprintf(f, "%s%s %s ", indentation(indent), "struct", this->id_);
	break;
      }
    case union_t:
      {
	fprintf(f, "%s%s %s ", indentation(indent), "union", this->id_);
	break;
      }
    default:
      printf("error");
      exit(0);
      break;
    }
  if(!this->struct_dec_.empty())
    {
      fprintf(f, "{\n");
      for(std::vector<CCSTStructDeclaration*>::iterator it = struct_dec_.begin(); it != struct_dec_.end(); ++it)
	{
	  CCSTStructDeclaration *ds = *it;
	  ds->write(f, indent+1);
	  fprintf(f, "\n");
	}
      fprintf(f, "%s}", indentation(indent));
    }
}

CCSTStructDeclaration::CCSTStructDeclaration(std::vector<CCSTSpecifierQual*> spec_qual, CCSTStructDecList *dec_list)
{
  this->spec_qual_ = spec_qual; 
  this->dec_list_ = dec_list;
}

void CCSTStructDeclaration::write(FILE *f, int indent)
{
  fprintf(f, "%s", indentation(indent));
  if(!this->spec_qual_.empty())
    {
      for(std::vector<CCSTSpecifierQual*>::iterator it = spec_qual_.begin(); it != spec_qual_.end(); ++it)
	{
	  CCSTSpecifierQual *ds = *it;
	  ds->write(f, 0);
	  fprintf(f, " ");
	  
	}
    }
  this->dec_list_->write(f, 0);
  
}

CCSTStructDecList::CCSTStructDecList()
{
  // todo
}

CCSTStructDecList::CCSTStructDecList(std::vector<CCSTStructDeclarator*> struct_decs)
{
  this->struct_decs_ = struct_decs;
}

void CCSTStructDecList::write(FILE *f, int indent)
{
  fprintf(f, "%s", indentation(indent));
  for(std::vector<CCSTStructDeclarator*>::iterator it = struct_decs_.begin(); it != struct_decs_.end(); ++it)
	{
	  CCSTStructDeclarator *ds = *it;
	  ds->write(f, 0);
	  fprintf(f, ", "); // if last do not write ,
	}
}
CCSTStructDeclarator::CCSTStructDeclarator()
{
  this->dec_ = NULL;
  this->expr_ = NULL;
}

CCSTStructDeclarator::CCSTStructDeclarator(CCSTDeclarator *dec)
{
  this->dec_ = dec; 
  this->expr_ = NULL;
}

CCSTStructDeclarator::CCSTStructDeclarator(CCSTDeclarator *dec, CCSTConstExpr *expr)
{
  this->dec_ = dec; 
  this->expr_ = expr;
}

CCSTStructDeclarator::CCSTStructDeclarator(CCSTConstExpr *expr)
{
  this->dec_ = NULL; 
  this->expr_ = expr;
}

void CCSTStructDeclarator::write(FILE *f, int indent)
{
  if(this->dec_ != NULL)
    {
      this->dec_->write(f, indent);
    }
  if(this->expr_ != NULL)
    {
      fprintf(f, " : ");
      this->expr_->write(f, 0);
    }
}

CCSTDeclarator::CCSTDeclarator(CCSTPointer *pointer, CCSTDirectDeclarator *d_dec)
{
  this->pointer_ = pointer; 
  this->d_dec_ = d_dec;
}

void CCSTDeclarator::write(FILE *f, int indent)
{
  
  if(this->pointer_ != NULL)
    {
      this->pointer_->write(f, indent);
    }
  if(this->d_dec_ == NULL)
    {
      printf("error");
      exit(0);
    }
  this->d_dec_->write(f, 0);
}

CCSTPointer::CCSTPointer(std::vector<type_qualifier> type_q, CCSTPointer *p)
{
  this->type_q_ = type_q; 
  this->p_ = p;
}

CCSTPointer::CCSTPointer()
{
  this->p_ = NULL;
}

CCSTPointer::CCSTPointer(std::vector<type_qualifier> type_q)
{
  this->type_q_ = type_q; 
  this->p_ = NULL;
}

CCSTPointer::CCSTPointer(CCSTPointer *p)
{
  this->p_ = p;
}

void CCSTPointer::write(FILE *f, int indent)
{
  fprintf(f,"%s*", indentation(indent));
  for(std::vector<type_qualifier>::iterator it = type_q_.begin(); it != type_q_.end(); ++it)
	{
	  type_qualifier tq =  *it;
	  switch (tq)
	    {
	    case const_t:
	      fprintf(f, "%s ", "const");
	      break;
	    case volatile_t:
	      fprintf(f, "%s ", "volatile");
	      break;
	    default:
	      break;
	    }
	}
  if(this->p_ != NULL)
    {
      this->p_->write(f, 0);
    }
}

CCSTDirectDecId::CCSTDirectDecId(const char* id)
{
  this->id_ = id;
}

void CCSTDirectDecId::write(FILE *f, int indent)
{
  fprintf(f, "%s%s", indentation(indent), this->id_);
}

CCSTDirectDecDec::CCSTDirectDecDec(CCSTDeclarator *dec)
{
  this->dec_ = dec;
}

void CCSTDirectDecDec::write(FILE *f, int indent)
{
  if(this->dec_ == NULL)
    {
      printf("Error\n");
      exit(0);
    }
  fprintf(f, "%s( ", indentation(indent)); // are there actually supposed to be parens?
  this->dec_->write(f, 0);
  fprintf(f, " )");
}

CCSTDirectDecConstExpr::CCSTDirectDecConstExpr(CCSTDirectDeclarator *direct_dec, CCSTConstExpr *const_expr)
{
  this->direct_dec_ = direct_dec; 
  this->const_expr_ = const_expr;
}

CCSTDirectDecConstExpr::CCSTDirectDecConstExpr(CCSTDirectDeclarator *direct_dec)
{
 this->direct_dec_ = direct_dec; 
 this->const_expr_ = NULL; 
}

void CCSTDirectDecConstExpr::write(FILE *f, int indent)
{
  if(this->direct_dec_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->direct_dec_->write(f, indent);
  fprintf(f, "[ ");
  if(this->const_expr_ != NULL)
    {
      this->const_expr_->write(f, 0);
    }
  fprintf(f, "]");
}

CCSTDirectDecParamTypeList::CCSTDirectDecParamTypeList(CCSTDirectDeclarator *direct_dec, CCSTParamTypeList *p_t_list)
{
  this->direct_dec_ = direct_dec; 
  this->p_t_list_ = p_t_list;
}

void CCSTDirectDecParamTypeList::write(FILE *f, int indent)
{
  if(this->direct_dec_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->direct_dec_->write(f, indent);
  fprintf(f, "( ");
  if(this->p_t_list_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->p_t_list_->write(f, 0);
  fprintf(f, ")");
}

CCSTDirectDecIdList::CCSTDirectDecIdList(CCSTDirectDeclarator *direct_dec, std::vector<char*> ids)
{
  this->direct_dec_ = direct_dec; 
  this->ids_ = ids;
}

void CCSTDirectDecIdList::write(FILE *f, int indent)
{
  if(this->direct_dec_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "%s( ", indentation(indent));
  for(std::vector<char*>::iterator it = ids_.begin(); it != ids_.end(); ++it)
	{
	  char *id = *it;
	  fprintf(f, " %s", id);
	  fprintf(f, ", "); // should i be printing commas
	}
  fprintf(f, " )");
}

CCSTCondExpr::CCSTCondExpr()
{
  // printf("incomplete cond expr\n"); 
  //todo
}

CCSTCondExpr::CCSTCondExpr(CCSTLogicalOrExpr *log_or_expr, CCSTExpression *expr, CCSTCondExpr *cond_expr)
{
  this->log_or_expr_ = log_or_expr; 
  this->expr_ = expr; 
  this->cond_expr_ = cond_expr;
}

void CCSTCondExpr::write(FILE *f, int indent)
{
  printf("incomplete cond expr\n");
  //todo
}

CCSTConstExpr::CCSTConstExpr(CCSTCondExpr *cond_expr)
{
  this->cond_expr_ = cond_expr;
}

void CCSTConstExpr::write(FILE *f, int indent)
{
  if(this->cond_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "const todo");
  
}

CCSTLogicalOrExpr::CCSTLogicalOrExpr()
{
  
  //todo
}

CCSTLogicalOrExpr::CCSTLogicalOrExpr(CCSTLogicalOrExpr *or__, CCSTLogicalAndExpr *and__)
{
  this->and_ = and__; 
  this->or_ = or__;
}

void CCSTLogicalOrExpr::write(FILE *f, int indent)
{
  if(this->or_ != NULL)
    {
      if(this->and_ == NULL)
	{
	  printf("error\n");
	  exit(0);
	}
      fprintf(f, "%s( ", indentation(indent));
      this->or_->write(f, 0);
      fprintf(f, " )");
      fprintf(f, " || ");
      fprintf(f, "( ");
      this->and_->write(f, 0);
      fprintf(f, " )");
    }
  else 
    {
      // fprintf(f, "( ");
      this->and_->write(f, indent);
      // fprintf(f, " )");
    }
}

CCSTLogicalAndExpr::CCSTLogicalAndExpr()
{
  //todo
}

CCSTLogicalAndExpr::CCSTLogicalAndExpr(CCSTLogicalAndExpr *and__, CCSTInclusiveOrExpr *or__)
{
  this->and_ = and__; this->or_ = or__;
}

void CCSTLogicalAndExpr::write(FILE *f, int indent)
{
  if(this->and_ != NULL)
    {
      if(this->or_ == NULL)
	{
	  printf("error\n");
	  exit(0);
	}
      fprintf(f, "%s( ", indentation(indent));
      this->and_->write(f, 0);
      fprintf(f, " )");
      fprintf(f, " && ");
      fprintf(f, "( ");
      this->or_->write(f, 0);
      fprintf(f, " )");
    }
  else
    {
      //  fprintf(f, "( ");
      this->or_->write(f, indent);
      //    fprintf(f, " )");
    }
}

CCSTInclusiveOrExpr::CCSTInclusiveOrExpr()
{
  //todo
}

CCSTInclusiveOrExpr::CCSTInclusiveOrExpr(CCSTInclusiveOrExpr *in_or, CCSTXorExpr *xor__)
{
  this->in_or_ = in_or; 
  this->xor_ = xor__;
}

void CCSTInclusiveOrExpr::write(FILE *f, int indent)
{
  if(this->in_or_ != NULL)
    {
      if(this->xor_ == NULL)
	{
	  printf("error\n");
	  exit(0);
	}
      fprintf(f, "%s( ", indentation(indent));
      this->in_or_->write(f, 0);
      fprintf(f, " )");
      fprintf(f, " | ");
      fprintf(f, "( ");
      this->xor_->write(f, 0);
      fprintf(f, " )");
    }
  else
    {
      this->xor_->write(f, indent);
    }
}

CCSTXorExpr::CCSTXorExpr()
{
  //todo
}

CCSTXorExpr::CCSTXorExpr(CCSTXorExpr *xor__, CCSTAndExpr *and__)
{
  this->xor_ = xor__; 
  this->and_ = and__;
}

void CCSTXorExpr::write(FILE *f, int indent)
{
  if(this->xor_ != NULL)
    {
      fprintf(f, "%s( ", indentation(indent));
      this->xor_->write(f, 0);
      fprintf(f, " )");
      fprintf(f, " ^ ");
      fprintf(f, "( ");
      this->and_->write(f, 0);
      fprintf(f, " )");
    }
  else
    {
      // the if else not needed, because and expression inherits from xor expr
      this->and_->write(f, indent);
    }
}

CCSTAndExpr::CCSTAndExpr()
{
  //todo
}

CCSTAndExpr::CCSTAndExpr(CCSTAndExpr *and__, CCSTEqExpr *eq)
{
  this->and_ = and__; 
  this->eq_ = eq;
}

void CCSTAndExpr::write(FILE *f, int indent)
{
  if(this->and_ == NULL || this->eq_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "%s( ", indentation(indent));
  this->and_->write(f, 0);
  fprintf(f, " )");
  fprintf(f, " & ");
  fprintf(f, "( ");
  this->eq_->write(f, 0);
  fprintf(f, " )");
}

CCSTEqExpr::CCSTEqExpr()
{
  //todo
}

CCSTEqExpr::CCSTEqExpr(bool equal, CCSTEqExpr *eq_expr, CCSTRelationalExpr *r_expr)
{
  this->equal_ = equal; 
  this->eq_expr_ = eq_expr; 
  this->r_expr_ = r_expr;
}

void CCSTEqExpr::write(FILE *f, int indent)
{
  if(this->eq_expr_ == NULL || this->r_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  if(equal_)
    {
      fprintf(f, "%s( ", indentation(indent));
      this->eq_expr_->write(f, 0);
      fprintf(f, " )");
      fprintf(f, " == ");
      fprintf(f, "( ");
      this->r_expr_->write(f, 0);
      fprintf(f, " )");
    }
  else
    {
      fprintf(f, "%s( ", indentation(indent));
      this->eq_expr_->write(f, 0);
      fprintf(f, " )");
      fprintf(f, " != ");
      fprintf(f, "( ");
      this->r_expr_->write(f, 0);
      fprintf(f, " )");
    }
}

CCSTRelationalExpr::CCSTRelationalExpr()
{
  //todo
}

CCSTRelationalExpr::CCSTRelationalExpr(relational_op op, CCSTRelationalExpr *r_expr, CCSTShiftExpr *s_expr)
{
  this->op_ = op; 
  this->r_expr_ = r_expr; 
  this->s_expr_ = s_expr;
}

void CCSTRelationalExpr::write(FILE *f, int indent)
{
  if(this->r_expr_ == NULL || this->s_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "%s( ", indentation(indent));
  this->r_expr_->write(f, 0);
  fprintf(f, " )");
  switch (this->op_)
    {
    case lessthan_t:
      fprintf(f, " < ");  
      break;
    case greaterthan_t:
      fprintf(f, " > ");
      break;
    case lessthaneq_t:
      fprintf(f, " <= ");
      break;
    case greaterthaneq_t:
      fprintf(f, " >= ");
      break;
    default:
      {
	printf("error\n");
	exit(0);
      }
    }
  fprintf(f, "( ");
  this->s_expr_->write(f, 0);
  fprintf(f, " )");
}

CCSTShiftExpr::CCSTShiftExpr()
{
  //todo
}

CCSTShiftExpr::CCSTShiftExpr(shift_op shift, CCSTShiftExpr *s_expr, CCSTAdditiveExpr *a_expr)
{
  this->shift_ = shift; 
  this->s_expr_ = s_expr; 
  this->a_expr_ = a_expr;
}

void CCSTShiftExpr::write(FILE *f, int indent)
{
  if(this->s_expr_ == NULL || this->a_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "%s( ", indentation(indent));
  this->s_expr_->write(f, 0);
  fprintf(f, " )");
  switch (this->shift_)
    {
    case leftshift_t:
      fprintf(f, " << ");
      break;
    case rightshift_t:
      fprintf(f, " >> ");
      break;
    default:
      {
	printf("error\n");
	exit(0);
      } 
    }
  fprintf(f, "( ");
  this->a_expr_->write(f, 0);
  fprintf(f, " )");
}

CCSTAdditiveExpr::CCSTAdditiveExpr()
{
  //todo
}

CCSTAdditiveExpr::CCSTAdditiveExpr(additive_op op, CCSTAdditiveExpr *a_expr, CCSTMultExpr *m_expr)
{
  this->op_ = op; 
  this->a_expr_ = a_expr; 
  this->m_expr_ = m_expr;
}

void CCSTAdditiveExpr::write(FILE *f, int indent)
{ 
  if(this->a_expr_ == NULL || this->m_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "%s( ", indentation(indent));
  this->a_expr_->write(f, 0);
  fprintf(f, " )");
  switch (this->op_)
    {
    case plus_t:
      fprintf(f, " + ");
      break;
    case minus_t:
      fprintf(f, " - ");
      break;
    default:
      {
	printf("error\n");
	exit(0);
      }
    }
  fprintf(f, "( ");
  this->m_expr_->write(f, 0);
  fprintf(f, " )");
}

CCSTMultExpr::CCSTMultExpr()
{
  //todo
}

CCSTMultExpr::CCSTMultExpr(mult_op op, CCSTMultExpr *m_expr, CCSTCastExpr *c_expr)
{
  this->op_ = op; 
  this->m_expr_ = m_expr; 
  this->c_expr_ = c_expr;
}

void CCSTMultExpr::write(FILE *f, int indent)
{
  if(this->m_expr_ == NULL || this->c_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "%s( ", indentation(indent));
  this->m_expr_->write(f, 0);
  fprintf(f, " )");
  switch (this->op_)
    {
    case multiply_t:
      fprintf(f, " * ");
      break;
    case divide_t:
      fprintf(f, " / ");
      break;
    case mod_t:
      fprintf(f, " %s ", "%");
      break;
    default:
      {
	printf("error\n");
	exit(0);
      }
    }
  fprintf(f, "( ");
  this->c_expr_->write(f, 0);
  fprintf(f, " )");
}

CCSTCastExpr::CCSTCastExpr()
{
  //todo
}

CCSTCastExpr::CCSTCastExpr(CCSTTypeName *cast_type, CCSTCastExpr *cast_expr)
{
  this->cast_type_ = cast_type; 
  this->cast_expr_ = cast_expr;
}

void CCSTCastExpr::write(FILE *f, int indent)
{
  if(this->cast_type_ == NULL || this->cast_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "%s( ", indentation(indent));
  this->cast_type_->write(f, 0);
  fprintf(f, " )");
  this->cast_expr_->write(f, 0);
}

CCSTUnaryExprCastExpr::CCSTUnaryExprCastExpr(CCSTUnaryOp *unary_op, CCSTCastExpr *cast_expr)
{
  this->unary_op_ = unary_op; 
  this->cast_expr_ = cast_expr;
}

void CCSTUnaryExprCastExpr::write(FILE *f, int indent)
{
  if(this->unary_op_ == NULL || this->cast_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->unary_op_->write(f, indent);
  this->cast_expr_->write(f, 0);
}

CCSTUnaryExprOpOp::CCSTUnaryExprOpOp(incr_decr_ops op, CCSTUnaryExpr *unary_expr)
{
  this->unary_expr_ = unary_expr; 
  this->op_ = op;
}

void CCSTUnaryExprOpOp::write(FILE *f, int indent)
{
  if(this->unary_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  switch (this->op_)
    {
    case increment_t:
      fprintf(f, "%s++ ", indentation(indent));
      break;
    case decrement_t:
      fprintf(f, "%s-- ", indentation(indent));
      break;
    default:
      {
	printf("error\n");
	exit(0);
      }
    }
  this->unary_expr_->write(f, 0);
}

CCSTUnaryExprSizeOf::CCSTUnaryExprSizeOf(CCSTUnaryExpr *unary_expr)
{
  this->unary_expr_ = unary_expr; 
  this->type_name_ = NULL;
}

CCSTUnaryExprSizeOf::CCSTUnaryExprSizeOf(CCSTTypeName *type_name)
{
  this->type_name_  = type_name; 
  this->unary_expr_ = NULL;
}

void CCSTUnaryExprSizeOf::write(FILE *f, int indent)
{
  if(this->unary_expr_ != NULL)
    {
      fprintf(f, "%ssizeof", indentation(indent));
      fprintf(f, "( ");
      this->unary_expr_->write(f, 0);
      fprintf(f, " )");
    }
  else if(this->type_name_ != NULL)
    {
      fprintf(f, "%ssizeof", indentation(indent));
      fprintf(f, "( ");
      this->type_name_->write(f, 0);
      fprintf(f, " )");
    }
  else
    {
      printf("error\n");
      exit(0);
    }
}

CCSTPostFixExprOpOp::CCSTPostFixExprOpOp(CCSTPostFixExpr *post_fix_expr, incr_decr_ops op)
{
  this->post_fix_expr_ = post_fix_expr; 
  this->op_ = op;
}

// is this correct?
void CCSTPostFixExprOpOp::write(FILE *f, int indent)
{
  if(this->post_fix_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->post_fix_expr_->write(f, indent);
  fprintf(f, " ");
  switch (this->op_)
    {
    case increment_t:
      fprintf(f, "++");
      break;
    case decrement_t:
      fprintf(f, "--");
      break;
    default:
      {
	printf("error\n");
	exit(0);
      }
    }
}

CCSTPostFixExprAccess::CCSTPostFixExprAccess(CCSTPostFixExpr *post_fix_expr, accessor op, const char * id)
{
  this->post_fix_expr_ = post_fix_expr; 
  this->op_ = op;
  this->id_ = id;
}

void CCSTPostFixExprAccess::write(FILE *f, int indent)
{
  printf("in post fix\n");
  if(this->post_fix_expr_ == 0x0 || this->id_ == "")
    {
      printf("error\n");
      exit(0);
    }
  this->post_fix_expr_->write(f, indent);
  switch (this->op_)
    {
    case pointer_access_t:
      fprintf(f, "->");
      break;
    case object_access_t:
      fprintf(f, ".");
      break;
    default:
      {
	printf("error\n");
	exit(0);
      }
    }
  printf("at end\n");
  printf("%s", this->id_);
  fprintf(f, "%s", this->id_);
  printf("At real end\n");
}

CCSTPostFixExprExpr::CCSTPostFixExprExpr(CCSTPostFixExpr *post_fix_expr, CCSTExpression *expr)
{
  this->post_fix_expr_ = post_fix_expr; 
  this->expr_ = expr;
}

void CCSTPostFixExprExpr::write(FILE *f, int indent)
{
  if(this->post_fix_expr_ == NULL || this->expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->post_fix_expr_->write(f, indent);
  fprintf(f, "[ ");
  this->expr_->write(f, 0);
  fprintf(f, " ]");
}

CCSTPostFixExprAssnExpr::CCSTPostFixExprAssnExpr(CCSTPostFixExpr *post_fix_expr, std::vector<CCSTAssignExpr*> args)
{
  this->post_fix_expr_ = post_fix_expr; 
  this->args_ = args;
}

void CCSTPostFixExprAssnExpr::write(FILE *f, int indent)
{
  if(this->post_fix_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  
  this->post_fix_expr_->write(f, indent);
  fprintf(f, "(");
  std::vector<CCSTAssignExpr*> args = this->args_;
  if(!args.empty())
    {
      args.at(0)->write(f, 0);
      
      for(std::vector<CCSTAssignExpr*>::iterator it = args.begin()+1; it != args.end(); ++it)
	{
	  fprintf(f,", ");
	  CCSTAssignExpr *arg = *it;
	  arg->write(f, 0);
	 }
    }
  fprintf(f, ")");
  
}

CCSTPrimaryExpr::CCSTPrimaryExpr()
{
  //todo
}

CCSTPrimaryExpr::CCSTPrimaryExpr(CCSTExpression *expr)
{
  this->expr_ = expr;
}

void CCSTPrimaryExpr::write(FILE *f, int indent)
{
  if(this->expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "%s( ", indentation(indent));
  this->expr_->write(f, 0);
  fprintf(f, " )");
}

CCSTString::CCSTString(const char* string)
{
  this->string_ = string;
}

CCSTString::CCSTString()
{
  //todo
}

void CCSTString::write(FILE *f, int indent)
{
  // how should this be stored exactly?
  fprintf(f, "%s%s", indentation(indent), this->string_);
}

CCSTPrimaryExprId::CCSTPrimaryExprId()
{
  //todo
}

CCSTPrimaryExprId::CCSTPrimaryExprId(const char* id)
{
  this->id_ = id;
}

void CCSTPrimaryExprId::write(FILE *f, int indent)
{
  fprintf(f, "%s%s", indentation(indent), this->id_);
}

CCSTInteger::CCSTInteger()
{
  this->integer_ = -1;
}

CCSTInteger::CCSTInteger(int i)
{
  this->integer_ = i;
}

void CCSTInteger::write(FILE *f, int indent)
{
  fprintf(f, "%s%d", indentation(indent), this->integer_);
}

CCSTChar::CCSTChar()
{
  //todo
}

CCSTChar::CCSTChar(char c)
{
  this->c_ = c;
}

void CCSTChar::write(FILE *f, int indent)
{
  fprintf(f, "%s%c", indentation(indent), this->c_);
}

CCSTFloat::CCSTFloat(float f)
{
  this->f_ = f;
  this->float_ = true;
}

CCSTFloat::CCSTFloat(double d)
{
  this->d_ = d;
  this->float_ = false;
}

CCSTFloat::CCSTFloat()
{
  //todo
}

void CCSTFloat::write(FILE *f, int indent)
{
  if(float_)
    {
      fprintf(f, "%s%f", indentation(indent), this->f_);
    }
  else
    {
      fprintf(f, "%s%f", indentation(indent), this->d_);
    }
}

CCSTEnumConst::CCSTEnumConst(const char* enum_val)
{
  this->enum_val_ = enum_val;
}

CCSTEnumConst::CCSTEnumConst()
{
  //todo
}

void CCSTEnumConst::write(FILE *f, int indent)
{
  fprintf(f, "%s%s", indentation(indent), this->enum_val_);
}

CCSTExpression::CCSTExpression()
{
  //todo
}

CCSTExpression::CCSTExpression(std::vector<CCSTAssignExpr*> assn)
{
  this->assn_exprs_ = assn;
}

void CCSTExpression::write(FILE *f, int indent)
{
   for(std::vector<CCSTAssignExpr*>::iterator it = assn_exprs_.begin(); it != assn_exprs_.end(); ++it)
	{
	  CCSTAssignExpr *assn = *it;
	  assn->write(f, indent);
	}
}

CCSTAssignExpr::CCSTAssignExpr()
{
  this->unary_expr_ = 0x0; 
  this->assn_op_ = 0x0; 
  this->assn_expr_ = 0x0;
}

CCSTAssignExpr::CCSTAssignExpr(CCSTUnaryExpr *unary_expr, CCSTAssignOp *assn_op, CCSTAssignExpr *assn_expr)
{
  this->unary_expr_ = unary_expr; 
  this->assn_op_ = assn_op; 
  this->assn_expr_ = assn_expr;
}

void CCSTAssignExpr::write(FILE *f, int indent)
{
  if(this->unary_expr_ == NULL || this->assn_op_ == NULL || this->assn_expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->unary_expr_->write(f, indent);
  fprintf(f, " ");
  this->assn_op_->write(f, 0);
  fprintf(f, " ");
  this->assn_expr_->write(f, 0);
  fprintf(f, ";\n");
}

CCSTAssignOp::CCSTAssignOp(assign_op op)
{
  this->op_ = op;
}

CCSTAssignOp::CCSTAssignOp()
{
  //todo
}

void CCSTAssignOp::write(FILE *f, int indent)
{
  switch (this->op_)
    {
    case equal_t:
      fprintf(f, "=");
      break;
    case mult_eq_t:
      fprintf(f, "*=");
      break;
    case div_eq_t:
      fprintf(f, "/=");
      break;
    case mod_eq_t:
      fprintf(f, "%s", "%=");
      break;
    case plus_eq_t:
      fprintf(f, "+=");
      break;
    case minus_eq_t:
      fprintf(f, "-=");
      break;
    case lshift_eq_t:
      fprintf(f, "<<=");
      break;
    case rshift_eq_t:
      fprintf(f, ">>=");
      break;
    case and_eq_t:
      fprintf(f, "&=");
      break;
    case xor_eq_t:
      fprintf(f, "^=");
      break;
    case or_eq_t:
      fprintf(f, "|=");
      break;
    default:
      {
	printf("error\n");
	exit(0);
      }
    }
}

CCSTUnaryOp::CCSTUnaryOp(unary_op op)
{
  this->op_ = op;
}

CCSTUnaryOp::CCSTUnaryOp()
{
  
}

void CCSTUnaryOp::write(FILE *f, int indent)
{
  switch (this->op_)
    {
    case unary_bit_and_t:
      fprintf(f, "&");
      break;
    case unary_mult_t:
      fprintf(f, "*");
      break;
    case unary_plus_t:
      fprintf(f, "+");
      break;
    case unary_minus_t:
      fprintf(f, "-");
      break;
    case unary_tilde_t:
      fprintf(f, "~");
      break;
    case unary_bang_t:
      fprintf(f, "!");
      break;
    default:
      {
	printf("error\n");
	exit(0);
      }
    }
}

CCSTTypeName::CCSTTypeName()
{
  //todo
}

CCSTTypeName::CCSTTypeName(std::vector<CCSTSpecifierQual*> spec_quals, CCSTAbstDeclarator *abs_dec)
{
  this->spec_quals_ = spec_quals; 
  this->abs_dec_ = abs_dec;
}

void CCSTTypeName::write(FILE *f, int indent)
{
   for(std::vector<CCSTSpecifierQual*>::iterator it = spec_quals_.begin(); it != spec_quals_.end(); ++it)
	{
	  CCSTSpecifierQual *qual = *it;
	  qual->write(f, 0);
	  fprintf(f, " ");
	}
   if(this->abs_dec_ != NULL)
     {
       this->abs_dec_->write(f, 0);
     }
}

CCSTParamTypeList::CCSTParamTypeList()
{
  //todo
}

CCSTParamTypeList::CCSTParamTypeList(CCSTParamList *p_list, bool ellipsis)
{
  this->p_list_ = p_list;
  this->ellipsis_ = ellipsis;
}

void CCSTParamTypeList::write(FILE *f, int indent)
{
  if(this->p_list_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->p_list_->write(f, indent);
  fprintf(f, " , ");
  fprintf(f, "...");
}

CCSTParamList::CCSTParamList()
{
  //todo
}

CCSTParamList::CCSTParamList(std::vector<CCSTParamDeclaration*> p_dec)
{
  this->p_dec_ = p_dec;
}

void CCSTParamList::write(FILE *f, int indent)
{
  if(!p_dec_.empty())
    {
      p_dec_.at(0)->write(f, indent);
      
      for(std::vector<CCSTParamDeclaration*>::iterator it = p_dec_.begin()+1; it != p_dec_.end(); ++it)
	{
	  fprintf(f,", ");
	  CCSTParamDeclaration *dec = *it;
	  dec->write(f, 0);
	}
    }
}

CCSTParamDeclaration::CCSTParamDeclaration()
{
  //todo
}

CCSTParamDeclaration::CCSTParamDeclaration(std::vector<CCSTDecSpecifier*> dec_specs)
{
  this->dec_specs_ = dec_specs;
  this->dec_ = NULL;
  this->abs_dec_ = NULL;
}

CCSTParamDeclaration::CCSTParamDeclaration(std::vector<CCSTDecSpecifier*> dec_specs, CCSTDeclarator *dec)
{
  this->dec_specs_ = dec_specs; 
  this->dec_ = dec; 
  this->abs_dec_ = NULL;
}

CCSTParamDeclaration::CCSTParamDeclaration(std::vector<CCSTDecSpecifier*> dec_specs, CCSTAbstDeclarator *abs_dec)
{
  this->dec_specs_ = dec_specs; 
  this->abs_dec_ = abs_dec; 
  this->dec_ = NULL;
}

void CCSTParamDeclaration::write(FILE *f, int indent)
{
  for(std::vector<CCSTDecSpecifier*>::iterator it = dec_specs_.begin(); it != dec_specs_.end(); ++it)
    {
      CCSTDecSpecifier *spec = *it;
      spec->write(f, 0);
      fprintf(f, " ");
    }
  
  if(this->dec_ == NULL && this->abs_dec_ == NULL)
    {
      // write nothing
    }
  else if(this->dec_ == NULL)
    {
      if(this->abs_dec_ == NULL)
 	{
	  printf("error\n");
	  exit(0);
	}
      this->abs_dec_->write(f, 0);
    }
  else
    {
      this->dec_->write(f, 0);
    }
  
}

CCSTAbstDeclarator::CCSTAbstDeclarator()
{
  //todo
}

CCSTAbstDeclarator::CCSTAbstDeclarator(CCSTPointer *p, CCSTDirectAbstDeclarator *d_abs_dec)
{
  this->p_ = p; 
  this->d_abs_dec_ = d_abs_dec;
}

void CCSTAbstDeclarator::write(FILE *f, int indent)
{
  if(this->p_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->p_->write(f, indent);
  fprintf(f, " ");
  if(this->d_abs_dec_ != NULL)
    {
      this->d_abs_dec_->write(f, 0);
    }
}

CCSTDirectAbstDeclarator::CCSTDirectAbstDeclarator()
{
  //todo
}

CCSTDirectAbstDeclarator::CCSTDirectAbstDeclarator(CCSTAbstDeclarator *abs_dec)
{
  this->abs_dec_ = abs_dec;
}

CCSTDirectAbstDeclarator::CCSTDirectAbstDeclarator(CCSTDirectAbstDeclarator *d_abs_dec, CCSTConstExpr *const_expr)
{
  this->d_abs_dec_ = d_abs_dec; 
  this->const_expr_ = const_expr;
}

CCSTDirectAbstDeclarator::CCSTDirectAbstDeclarator(CCSTDirectAbstDeclarator *d_abs_dec, CCSTParamTypeList *param_type_list)
{
  this->d_abs_dec_ = d_abs_dec; 
  this->param_type_list_ = param_type_list;
}

void CCSTDirectAbstDeclarator::write(FILE *f, int indent)
{
  /*
    <direct-abstract-declarator> ::=  ( <abstract-declarator> )
    | {<direct-abstract-declarator>}? [ {<constant-expression>}? ]
    | {<direct-abstract-declarator>}? ( {<parameter-type-list>|? )
  */
  // TODO
  if(this->d_abs_dec_ == NULL && this->const_expr_ == NULL && this->param_type_list_ == NULL)
    {
      fprintf(f, "%s( ", indentation(indent));
      this->abs_dec_->write(f, 0);
      fprintf(f, " )");
    }
  else
    {
      if(this->d_abs_dec_ == NULL)
	{
	  printf("error\n");
	  exit(0);
	}
      this->d_abs_dec_->write(f, indent);
      if(this->const_expr_ == NULL)
	{
	  
	  if(this->param_type_list_ == NULL)
	    {
	      printf("error\n");
	      exit(0);
	    }
	  fprintf(f, "%s( ", indentation(indent));
	  this->param_type_list_->write(f, 0);
	  fprintf(f, " )");
	}
      else
	{
	  fprintf(f, "%s[ ", indentation(indent));
	  this->const_expr_->write(f, 0);
	  fprintf(f, " ] ");
	}
    }
}

CCSTEnumSpecifier::CCSTEnumSpecifier(const char* id, CCSTEnumeratorList *el)
{
  this->id_ = id; 
  this->el_ = el;
}

CCSTEnumSpecifier::CCSTEnumSpecifier(const char* id)
{
  this->id_ = id;
  this->el_ = NULL;
}

CCSTEnumSpecifier::CCSTEnumSpecifier(CCSTEnumeratorList *el)
{
  this->el_ = el;
  this->id_ = "";
}

CCSTEnumSpecifier::CCSTEnumSpecifier()
{
  //todo
}

void CCSTEnumSpecifier::write(FILE *f, int indent)
{
  if(this->el_ == NULL)
    {
      fprintf(f, "%senum ", indentation(indent));
      fprintf(f, "%s", this->id_);
    }
  else
    {
      fprintf(f, "%senum ", indentation(indent));
      fprintf(f, "%s", this->id_);
      fprintf(f, " { ");
      this->el_->write(f, 0);
      fprintf(f, " }");
    }
}

CCSTEnumeratorList::CCSTEnumeratorList()
{
  //todo
}

CCSTEnumeratorList::CCSTEnumeratorList(std::vector<CCSTEnumerator*> *list)
{
  this->list_ = list;
}

void CCSTEnumeratorList::write(FILE *f, int indent)
{
  if(!list_->empty())
    {
      list_->at(0)->write(f, indent);
      for(std::vector<CCSTEnumerator*>::iterator it = list_->begin()+1; it != list_->end(); ++it)
	{
	  fprintf(f, ",\n");
	  CCSTEnumerator *l = *it;
	  l->write(f, 0);
	}
    }
}

CCSTEnumerator::CCSTEnumerator(const char* id, CCSTConstExpr *ce)
{
  this->id_ = id; 
  this->ce_ = ce;
}

CCSTEnumerator::CCSTEnumerator(const char* id)
{
  this->id_ = id; 
  this->ce_ = NULL;
}

void CCSTEnumerator::write(FILE *f, int indent)
{
  if(this->ce_ == NULL)
    {
      fprintf(f, "%s%s", indentation(indent), this->id_);
    }
  else
    {
      fprintf(f, "%s%s", indentation(indent), this->id_);
      fprintf(f, " = ");
      this->ce_->write(f, 0);
    }
}

CCSTTypedefName::CCSTTypedefName(const char* name)
{
  this->id_ = name;
}

void CCSTTypedefName::write(FILE *f, int indent)
{
  fprintf(f, "%s%s", this->id_, indentation(indent));
}


void CCSTDeclaration::write(FILE *f, int indent)
{
  for(std::vector<CCSTDecSpecifier*>::iterator it = specifier_.begin(); it != specifier_.end(); ++it)
    {
      CCSTDecSpecifier *dec_spec = *it;
      dec_spec->write(f, indent);
      fprintf(f, " ");
    }
  for(std::vector<CCSTInitDeclarator*>::iterator it = decs_.begin(); it != decs_.end(); ++it)
    {
      CCSTInitDeclarator *init_dec = *it;
      init_dec->write(f, 0);
      fprintf(f, " ");
    }
  fprintf(f, ";");
  fprintf(f, "\n\n");
}

CCSTInitDeclarator::CCSTInitDeclarator(CCSTDeclarator *dec, CCSTInitializer *init)
{
  this->dec_ = dec; 
  this->init_ = init;
}

CCSTInitDeclarator::CCSTInitDeclarator(CCSTDeclarator *dec)
{
  this->dec_ = dec; 
  this->init_ = NULL;
}

void CCSTInitDeclarator::write(FILE *f, int indent)
{
  // does inheritence cover just declarator case?
  if(this->dec_ == NULL || this->init_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->dec_->write(f, indent);
  fprintf(f, " = ");
  this->init_->write(f, 0);
}

CCSTInitializer::CCSTInitializer(CCSTAssignExpr *assn_expr)
{
  this->assn_expr_ = assn_expr;
  this->init_list_ = 0x0;
}

CCSTInitializer::CCSTInitializer(CCSTInitializerList *init_list)
{
  this->init_list_ = init_list;
  this->assn_expr_ = 0x0;
}

void CCSTInitializer::write(FILE *f, int indent)
{
  /*
    
<initializer> ::= <assignment-expression>
                | { <initializer-list> }
                | { <initializer-list> , }
   */

  // TODO
  if(this->assn_expr_ == NULL)
    {
      if(this->init_list_ == NULL)
	{
	  printf("error\n");
	  exit(0);
	}
      fprintf(f, "%s{ ", indentation(indent));
      this->init_list_->write(f, 0);
      fprintf(f, " }");
    }
  else
    {
      this->assn_expr_->write(f, indent);
    }
}

CCSTInitializerList::CCSTInitializerList()
{
  //todo
}

CCSTInitializerList::CCSTInitializerList(std::vector<CCSTInitializer*> init_list)
{
  this->init_list_ = init_list;
}

void CCSTInitializerList::write(FILE *f, int indent)
{
  // TODO
  for(std::vector<CCSTInitializer*>::iterator it = init_list_.begin(); it != init_list_.end(); ++it)
    {
      CCSTInitializer *init = *it;
      init->write(f, 0);
      fprintf(f, ", ");
    }
  
}

CCSTCompoundStatement::CCSTCompoundStatement(std::vector<CCSTDeclaration*> decs, std::vector<CCSTStatement*> s)
{
  this->declarations_ = decs; 
  this->statements_ = s;
}

void CCSTCompoundStatement::write(FILE *f, int indent)
{

  for(std::vector<CCSTDeclaration*>::iterator it = this->declarations_.begin(); it != declarations_.end(); ++it)
    {
      CCSTDeclaration *dec = *it;
      
      dec->write(f, indent);
      fprintf(f, "\n");
    }
  for(std::vector<CCSTStatement*>::iterator it = this->statements_.begin(); it != statements_.end(); ++it)
    {
      CCSTStatement *state = *it;
      state->write(f, indent);
      fprintf(f, "\n");
    }
}

CCSTPlainLabelStatement::CCSTPlainLabelStatement(const char* id, CCSTStatement *stmnt)
{
  this->id_ = id; 
  this->stmnt_ = stmnt;
}

void CCSTPlainLabelStatement::write(FILE *f, int indent)
{
  //todo
  fprintf(f, "%s%s: ", indentation(indent), this->id_);
  this->stmnt_->write(f, 0);
  fprintf(f, ";");
}

CCSTCaseStatement::CCSTCaseStatement(CCSTCondExpr *c, CCSTStatement *body)
{
  this->case_label_ = c; 
  this->body_ = body;
}

void CCSTCaseStatement::write(FILE *f, int indent)
{
  fprintf(f, "%scase ", indentation(indent));
  this->case_label_->write(f, 0);
  fprintf(f, ": ");
  fprintf(f, "{\n");
  this->body_->write(f, indent+1);
  fprintf(f, "\n%s}", indentation(indent));
}

void CCSTDefaultLabelStatement::write(FILE *f, int indent)
{
  fprintf(f, "%sdefault: ", indentation(indent));
  fprintf(f, "{\n");
  this->body_->write(f, indent+1);
  fprintf(f, "\n%s}", indentation(indent));
}

CCSTDefaultLabelStatement::CCSTDefaultLabelStatement(CCSTStatement* body)
{
  this->body_ = body;
}

CCSTExprStatement::CCSTExprStatement()
{
}

CCSTExprStatement::CCSTExprStatement(CCSTExpression *expr)
{
  this->expr_ = expr;
}

void CCSTExprStatement::write(FILE *f, int indent)
{
  // weird why the semicolon with no expression
  if(this->expr_ != NULL)
    {
      this->expr_->write(f, indent);
    }
  fprintf(f, ";");
}

CCSTIfStatement::CCSTIfStatement(CCSTExpression *cond, CCSTStatement *body)
{
  this->cond_ = cond; 
  this->body_ = body;
}

void CCSTIfStatement::write(FILE *f, int indent)
{
  fprintf(f, "%sif", indentation(indent));
  fprintf(f, "( ");
  if(this->cond_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->cond_->write(f, 0);
  fprintf(f, " )");
  fprintf(f, " {\n");
  this->body_->write(f, indent+1);
  fprintf(f, "\n");
  fprintf(f, "%s}", indentation(indent));
  fprintf(f, "\n");
}

CCSTIfElseStatement::CCSTIfElseStatement(CCSTExpression *cond, CCSTStatement *if_body, CCSTStatement *else_body)
{
  this->cond_ = cond; 
  this->if_body_ = if_body; 
  this->else_body_ = else_body;
}

void CCSTIfElseStatement::write(FILE *f, int indent)
{
  fprintf(f, "%sif", indentation(indent));
  fprintf(f, "( ");
  if(this->cond_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  this->cond_->write(f, 0);
  fprintf(f, " )");
  fprintf(f, " {\n");
  this->if_body_->write(f, indent+1);
  fprintf(f, "\n");
  fprintf(f, "%s}", indentation(indent));
  fprintf(f, " else");
  fprintf(f, " {\n");
  this->else_body_->write(f, indent+1);
  fprintf(f, "\n%s}\n", indentation(indent));
}

CCSTSwitchStatement::CCSTSwitchStatement(CCSTExpression *expr, CCSTStatement *body)
{
  this->expr_ = expr; 
  this->body_ = body;
}

void CCSTSwitchStatement::write(FILE *f, int indent)
{
  if(this->expr_ == NULL)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "%sswitch ", indentation(indent));
  fprintf(f, "( ");
  this->expr_->write(f, 0);
  fprintf(f, " )");
  fprintf(f, " {\n");
  this->body_->write(f, indent+1); // all cases?
  fprintf(f, "\n%s}\n", indentation(indent));
}

CCSTWhileLoop::CCSTWhileLoop(CCSTExpression *cond, CCSTStatement *body)
{
  this->cond_ = cond; 
  this->body_ = body;
}

void CCSTWhileLoop::write(FILE *f, int indent)
{
  fprintf(f, "%swhile", indentation(indent));
  fprintf(f, "( ");
  this->cond_->write(f, 0);
  fprintf(f, " )");
  fprintf(f, " {\n");
  this->body_->write(f, indent+1);
  fprintf(f, "\n");
  fprintf(f, "%s}", indentation(indent));
}

CCSTDoLoop::CCSTDoLoop(CCSTStatement *body, CCSTExpression *cond)
{
  this->body_ = body; 
  this->cond_ = cond;
}

void CCSTDoLoop::write(FILE *f, int indent)
{
  fprintf(f, "%sdo ", indentation(indent));
  fprintf(f, "{\n");
  this->body_->write(f, indent+1);
  fprintf(f, "\n%s} ", indentation(indent));
  fprintf(f, "while");
  fprintf(f, "( ");
  this->cond_->write(f, 0);
  fprintf(f, " )");
  fprintf(f, ";\n");
}

CCSTForLoop::CCSTForLoop(CCSTExpression *init, CCSTExpression *cond, CCSTExpression *up, CCSTStatement *body)
{
  this->init_ = init; 
  this->cond_ = cond; 
  this->up_ = up; 
  this->body_ = body;
}

void CCSTForLoop::write(FILE *f, int indent)
{
  // write for (
  fprintf(f, "%sfor", indentation(indent));
  fprintf(f, "( ");
  if(this->init_ != NULL)
    this->init_->write(f, 0);
  fprintf(f, ";");
  if(this->cond_ != NULL)
    this->cond_->write(f, 0);
  fprintf(f, ";");
  if(this->up_ != NULL)
    this->up_->write(f, 0);
  fprintf(f, " ) ");
  fprintf(f, "{\n");
  this->body_->write(f, indent+1);
  fprintf(f, "\n%s}", indentation(indent));
}

CCSTGoto::CCSTGoto(const char* id)
{
  this->identifier_ = id;
}

void CCSTGoto::write(FILE *f, int indent)
{
  fprintf(f, "%sgoto ", indentation(indent));
  fprintf(f, "%s", this->identifier_);
  fprintf(f, ";");
}

CCSTContinue::CCSTContinue()
{
}

void CCSTContinue::write(FILE *f, int indent)
{
  fprintf(f, "%scontinue", indentation(indent));// write continue ;
}

CCSTBreak::CCSTBreak()
{
}

void CCSTBreak::write(FILE *f, int indent)
{
  fprintf(f, "%sbreak", indentation(indent));
}

CCSTReturn::CCSTReturn()
{
  this->expr_ = NULL;
}
CCSTReturn::CCSTReturn(CCSTExpression *expr)
{
  this->expr_ = expr;
}

void CCSTReturn::write(FILE *f, int indent)
{
  if(this->expr_ == NULL)
    {
      fprintf(f, "%sreturn;", indentation(indent));
    }
  else
    {
      fprintf(f, "%sreturn", indentation(indent));
      this->expr_->write(f, 0);
      fprintf(f, ";");
    }
}

const char* indentation(int level)
{
  int length = level*INDENT;
  
  char *spacing = (char*) malloc(sizeof(char)*(length+1));

  std::ostringstream total;
  int i = 0;
  while(i < length) {
    total << " ";
    i++;
  }
  strncpy(spacing, total.str().c_str(), length+1);
  return spacing;
}
