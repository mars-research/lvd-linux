#include "cast.h"

void CCSTFile::write(FILE *f)
{
  /*
    for(std::vector<T>::iterator it = v.begin(); it != v.end(); ++it) {
   */
  for(std::vector<CCSTExDeclaration*>::iterator it = defs_.begin(); it != defs_.end(); ++it)
    {
      CCSTExDeclaration *ex_dec = *it;
      ex_dec->write(f);
    }
}

void CCSTFuncDef::write(FILE *f)
{
  for(std::vector<CCSTDecSpecifier*>::iterator it = specifiers_.begin(); it != specifiers_.end(); ++it)
    {
      CCSTDecSpecifier *ds = *it;
      ds->write(f);
    }
  this->ret_->write(f);
  
  for(std::vector<CCSTDeclaration*>::iterator it = decs_.begin(); it != decs_.end(); ++it)
    {
      CCSTDeclaration *ds = *it;
      ds->write(f);
    }
  // write body
  this->body_->write(f);
}

void CCSTDeclaration::write(FILE *f)
{
  for(std::vector<CCSTDecSpecifier*>::iterator it = specifier_.begin(); it != specifier_.end(); ++it)
    {
      CCSTDecSpecifier *ds = *it;
      ds->write(f);
    }
  if(this->decs_ != null)
    {
      for(std::vector<CCSTInitDeclarator*>::iterator it = decs_.begin(); it != decs_.end(); ++it)
	{
	  CCSTInitDeclarator *ds = *it;
	  ds->write(f);
	}
    }
  // anything else
}

void CCSTStoClassSpecifier::write(FILE *f)
{
  switch (this->val_)
    {
    case auto_t:
      fprintf(f, "%s ", "auto");
      break;
    case register_t:
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
    case default:
      fprintf(f, "%s ", "error");
      break;
    } 

}

void CCSTSimpleTypeSpecifier::write(FILE *f)
{
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
    case default:
      fprintf(f, "%s ", "error");
      break;
    } 
}

void CCSTStructUnionSpecifier::write(FILE *f)
{
  switch (this->s_or_u_)
    {
    case struct_t:
      {
	//  <struct-or-union> <identifier> what about this case
	fprintf(f, "%s %s ", "struct", this->id_);
	break;
      }
    case union_t:
      {
	fprintf(f, "%s %s ", "union", this->id_);
	break;
      }
    case default:
      printf("error");
      exit(0);
      break;
    }
  if(this->struct_dec_ != null)
    {
      fprintf(f, "{\n");
      for(std::vector<CCSTStructDeclaration*>::iterator it = struct_dec_.begin(); it != struct_dec_.end(); ++it)
	{
	  CCSTStructDeclaration *ds = *it;
	  ds->write(f);
	  fprintf(f, "\n");
	}
      fprintf(f, "};\n");
    }
}

void CCSTStructDeclaration::write(FILE *f)
{
  if(this->spec_qual_ != null)
    {
      for(std::vector<CCSTSpecifierQual*>::iterator it = spec_qual_.begin(); it != spec_qual_.end(); ++it)
	{
	  CCSTSpecifierQual *ds = *it;
	  ds->write(f);
	  fprintf(f, " ");
	  
	}
    }
  this->dec_list_->write(f);
  
}

void CCSTStructDecList::write(FILE *f)
{
  for(std::vector<CCSTStructDeclarator*>::iterator it = struct_decs_.begin(); it != struct_decs_.end(); ++it)
	{
	  CCSTStructDeclarator *ds = *it;
	  ds->write(f);
	  fprintf(f, ", "); // if last do not write ,
	}
}

void CCSTStructDeclarator::write(FILE *f)
{
  if(this->dec_ != null)
    {
      this->dec_->write(f);
    }
  if(this->expr_ != null)
    {
      fprintf(f, " : ");
      this->expr_->write(f);
    }
}

void CCSTDeclarator::write(FILE *f)
{
  if(this->pointer_ != null)
    {
      this->pointer_->write(f);
    }
  if(this->d_dec_ == null)
    {
      printf("error");
      exit(0);
    }
  this->d_dec_->write(f);
}

void CCSTPointer::write(FILE *f)
{
  fprintf(f,"*");
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
	    case default:
	      break;
	    }
	}
  if(this->p_ != null)
    {
      this->p_->write(f);
    }
}

void CCSTDirectDecId::write(FILE *f)
{
  fprintf(f, " %s ", this->id_);
}

void CCSTDirectDecDec::write(FILE *f)
{
  if(this->dec_ == null)
    {
      printf("Error\n");
      exit(0);
    }
  fprintf(f, "( "); // are there actually supposed to be parens?
  this->dec_->write(f);
  fprintf(f, " )");
}

void CCSTDirectDecConstExpr::write(FILE *f)
{
  if(this->direct_dec_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->direct_dec_->write();
  fprintf(f, "[ ");
  if(this->const_expr_ != null)
    {
      this->const_expr_->write(f);
    }
  fprintf(f, "]");
}

void CCSTDirectDecParamTypeList::write()
{
  if(this->direct_dec_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->direct_dec_->write();
  fprintf(f, "( ");
  if(this->p_t_list_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->p_t_list_->write(f);
  fprintf(f, ")");
}

void CCSTDirectDecIdList::write()
{
  if(this->direct_dec_ == null)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "( ");
  for(std::vector<char*>::iterator it = ids_.begin(); it != ids_.end(); ++it)
	{
	  char *id = *it;
	  fprintf(f, " %s", id);
	  fprintf(f, ", "); // should i be printing commas
	}
  fprintf(f, " )");
}

void CCSTConstExpr::write(FILE *f)
{
  if(this->cond_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  
}

void CCSTLogicalOrExpr::write(FILE *f)
{
  if(this->or_ != null)
    {
      if(this->and_ == null)
	{
	  printf("error\n");
	  exit(0);
	}
      fprintf(f, "( ");
      this->or_->write(f);
      fprintf(f, " )");
      fprintf(f, " || ");
      fprintf(f, "( ");
      this->and_->write(f);
      fprintf(f, " )");
    }
  else 
    {
      // fprintf(f, "( ");
      this->and_->write(f);
      // fprintf(f, " )");
    }
}

void CCSTLogicalAndExpr::write(FILE *f)
{
  if(this->and_ != null)
    {
      if(this->or_ == null)
	{
	  printf("error\n");
	  exit(0);
	}
      fprintf(f, "( ");
      this->and_->write(f);
      fprintf(f, " )");
      fprintf(f, " && ");
      fprintf(f, "( ");
      this->or_->write(f);
      fprintf(f, " )");
    }
  else
    {
      //  fprintf(f, "( ");
      this->or_->write(f);
      //    fprintf(f, " )");
    }
}

void CCSTInclusiveOrExpr::write(FILE *f)
{
  if(this->in_or_ != null)
    {
      if(this->xor_ == null)
	{
	  printf("error\n");
	  exit(0);
	}
      fprintf(f, "( ");
      this->in_or_->write(f);
      fprintf(f, " )");
      fprintf(f, " | ");
      fprintf(f, "( ");
      this->xor_->write(f);
      fprintf(f, " )");
    }
  else
    {
      this->xor_->write();
    }
}

void CCSTXorExpr::write(FILE *f)
{
  if(this->xor_ != null)
    {
      fprintf(f, "( ");
      this->xor_->write(f);
      fprintf(f, " )");
      fprintf(f, " ^ ");
      fprintf(f, "( ");
      this->and_->write(f);
      fprintf(f, " )");
    }
  else
    {
      // the if else not needed, because and expression inherits from xor expr
      this->and_->write(f);
    }
}

void CCSTAndExpr::write(FILE *f)
{
  if(this->and_ == null || this->eq_ == null)
    {
      fprintf("error\n");
      exit(0);
    }
  fprintf(f, "( ");
  this->and_->write(f);
  fprintf(f, " )");
  fprintf(f, " & ");
  fprintf(f, "( ");
  this->eq_->write(f);
  fprintf(f, " )");
}

void CCSTEqExpr::write(FILE *f)
{
  if(this->eq_expr_ == null || this->r_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  if(equal_)
    {
      fprintf(f, "( ");
      this->eq_expr_->write(f);
      fprintf(f, " )");
      fprintf(f, " == ");
      fprintf(f, "( ");
      this->r_expr_->write(f);
      fprintf(f, " )");
    }
  else
    {
      fprintf(f, "( ");
      this->eq_expr_->write(f);
      fprintf(f, " )");
      fprintf(f, " != ");
      fprintf(f, "( ");
      this->r_expr_->write(f);
      fprintf(f, " )");
    }
}

void CCSTRelationalExpr::write(FILE *f)
{
  if(this->r_expr_ == null || this->s_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "( ");
  this->r_expr_->write(f);
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
    case default:
      {
	printf("error\n");
	exit(0);
      }
    }
  fprintf(f, "( ");
  this->s_expr_->write(f);
  fprintf(f, " )");
}

void CCSTShiftExpr::write(FILE *f)
{
  if(this->s_expr_ == null || this->a_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "( ");
  this->s_expr_->write(f);
  fprintf(f, " )");
  switch (this->shift_)
    {
    case leftshift_t:
      fprintf(f, " << ");
      break;
    case rightshift_t:
      fprintf(f, " >> ");
      break;
    case default:
      {
	printf("error\n");
	exit(0);
      } 
    }
  fprintf(f, "( ");
  this->a_expr_->write(f);
  fprintf(f, " )");
}

void CCSTAdditiveExpr::write(FILE *f)
{ 
  if(this->a_expr_ == null || this->m_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "( ");
  this->a_expr_->write(f);
  fprintf(f, " )");
  switch (this->op_)
    {
    case plus_t:
      fprintf(f, " + ");
      break;
    case minus_t:
      fprintf(f, " - ");
      break;
    case default:
      {
	printf("error\n");
	exit(0);
      }
    }
  fprintf(f, "( ");
  this->m_expr_->write(f);
  fprintf(f, " )");
}

void CCSTMultExpr::write(FILE *f)
{
  if(this->m_expr_ == null || this->c_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "( ");
  this->m_expr_->write(f);
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
      fprintf(f, " % ");
      break;
    case default:
      {
	printf("error\n");
	exit(0);
      }
    }
  fprintf(f, "( ");
  this->c_expr_->write(f);
  fprintf(f, " )");
}

void CCSTCastExpr::write(FILE *f)
{
  if(this->cast_type_ == null || this->cast_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "( ");
  this->cast_type_->write(f);
  fprintf(f, " )");
  this->cast_expr_->write(f);
}

void CCSTUnaryExprCastExpr::write(FILE *f)
{
  if(this->unary_op_ == null || this->cast_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->unary_op_->write(f);
  fprintf(f, " ( ");
  this->cast_expr_->write(f);
  fprintf(f, " )");
}

void CCSTUnaryExprOpOp::write(FILE *f)
{
  if(this->unary_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  switch (this->op_)
    {
    case increment_t:
      fprintf(f, "++ ");
      break;
    case decrement_t:
      fprintf(f, "-- ");
      break;
    case default:
      {
	printf("error\n");
	exit(0);
      }
    }
  this->unary_expr_->write(f):
}

void CCSTUnaryExprSizeOf::write(FILE *f)
{
  if(this->unary_expr_ != null)
    {
      fprintf(f, "sizeof");
      fprintf(f, "( ");
      this->unary_expr_->write(f);
      fprintf(f, " )");
    }
  else if(this->type_name_ != null)
    {
      fprint(f, "sizeof");
      fprintf(f, "( ");
      this->type_name_->write(f);
      fprintf(f, " )");
    }
  else
    {
      printf("error\n");
      exit(0);
    }
}

void CCSTPostFixExprOpOp::write(FILE *f)
{
  if(this->post_fix_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->post_fix_expr->write(f);
  fprintf(f, " ");
  switch (this->op_)
    {
    case increment_t:
      break;
    case decrement_t:
      break;
    case default:
      {
	printf("error\n");
	exit(0);
      }
    }
}

void CCSTPostFixExprAccess::write(FILE *f)
{
  if(this->post_fix_expr_ == null || this->id_ == "")
    {
      printf("error\n");
      exit(0);
    }
  this->post_fix_expr_->write(f);
  switch (this->op_)
    {
    case pointer_access_t:
      fprintf(f, "->");
      break;
    case object_access_t:
      fprintf(f, ".");
      break;
    case default:
      {
	printf("error\n");
	exit(0);
      }
    }
  fprintf(f, "%s", this->id_);
}

void CCSTPostFixExprExpr::write(FILE *f)
{
  if(this->post_fix_expr_ == null || this->expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->post_fix_expr_->write(f);
  fprintf(f, "[ ");
  this->expr_->write(f);
  fprintf(f, " ]");
}

void CCSTPostFixExprAssnExpr::write(FILE *f)
{
  if(this->post_fix_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  // TODO really unsure
}

void CCSTPrimaryExpr::write(FILE *f)
{
  if(this->expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "( ");
  this->expr_->write(f);
  fprintf(f, " )");
}

void CCSTString::write(FILE *f)
{
  // how should this be stored exactly?
  fprintf(f, "%s", this->string_);
}

void CCSTPrimaryExprId::write(FILE *f)
{
  fprintf(f, "%s", this->id_);
}



void CCSTInteger::write(FILE *f)
{
  // todo
}

void CCSTChar::write(FILE *f)
{
  fprintf(f, "%c", this->char_);
}

void CCSTFloat::write(FILE *f)
{
  if(float_)
    {
      fprintf(f, "%f", this->f_);
    }
  else
    {
      fprintf(f, "%f", this->d_);
    }
}

void CCSTEnumConst::write(FILE *f)
{
  //TODO
  // unsure
}

void CCSTExpression::write(FILE *f)
{
   for(std::vector<CCSTAssignExpr*>::iterator it = assn_exprs_.begin(); it != assn_exprs_.end(); ++it)
	{
	  CCSTAssignExpr *assn = *it;
	  assn->write(f);
	}
}

void CCSTAssignExpr::write()
{
  if(this->unary_expr_ == null || this->assn_op_ == null || this->assn_expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->unary_expr_->write(f);
  fprintf(f, " ");
  this->assn_op_->write(f);
  fprintf(f, " ");
  this->assn_expr_->write(f);
}

void CCSTAssignOp::write()
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
      fprintf(f, "%=");
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
    case default:
      {
	printf("error\n");
	exit(0);
      }
    }
}

void CCSTUnaryOp::write(FILE *f)
{
  switch (this->op_)
    {
    case bit_and_t:
      fprintf(f, "&");
      break;
    case mult_t:
      fprintf(f, "*");
      break;
    case plus_t:
      fprintf(f, "+");
      break;
    case minus_t:
      fprintf(f, "-");
      break;
    case tilde_t:
      fprintf(f, "~");
      break;
    case bang_t:
      fprintf(f, "!");
      break;
    case default:
      {
	printf("error\n");
	exit(0);
      }
    }
}

void CCSTTypeName::write(FILE *f)
{
   for(std::vector<CCSTSpecifierQual*>::iterator it = spec_quals_.begin(); it != spec_quals_.end(); ++it)
	{
	  CCSTSpecifierQual *qual = *it;
	  qual->write(f);
	  fprintf(f, " ");
	}
   if(this->abs_dec_ != null)
     {
       this->abs_dec_->write(f);
     }
}

void CCSTParamTypeList::write(FILE *f)
{
  if(this->p_list_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->p_list_->write(f);
  fprintf(f, " , ");
  fprintf(f, "...");
}

void CCSTParamList::write(FILE *f)
{
   for(std::vector<CCSTParamDeclaration*>::iterator it = p_dec_.begin(); it != p_dec_.end(); ++it)
	{
	  CCSTParamDeclaration *dec = *it;
	  dec->write(f);
	  fprintf(f, ", "); // should i be printing commas
	}
}

void CCSTParamDeclaration::write(FILE *f)
{
  for(std::vector<CCSTDecSpecifier*>::iterator it = ids_.begin(); it != ids_.end(); ++it)
    {
      CCSTDecSpecifier *spec = *it;
      spec->write(f);
      fprintf(f, " ");
    }
  
  if(this->dec_ == null && this->abs_dec_ == null)
    {
      // write nothing
    }
  else if(this->dec_ == null)
    {
      if(this->abs_dec_ == null)
	{
	  printf("error\n");
	  exit(0);
	}
      this->abs_dec_->write(f);
    }
  else
    {
      this->dec_->write(f);
    }
  
}

void CCSTAbstDeclarator::write(FILE *f)
{
  if(this->p_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->p_->write(f);
  fprintf(f, " ");
  if(this->d_abs_dec_ != null)
    {
      this->d_abs_dec->write(f);
    }
}

void CCSTDirectAbstDeclarator::write(FILE *f)
{
  /*
    <direct-abstract-declarator> ::=  ( <abstract-declarator> )
                               | {<direct-abstract-declarator>}? [ {<constant-expression>}? ]
                               | {<direct-abstract-declarator>}? ( {<parameter-type-list>|? )
   */
  // TODO
  if(this->d_abs_dec_ == null && this->const_expr_ == null && this->param_type_list_ == null)
    {
      fprintf(f, "( ");
      this->abs_dec_->write(f);
      fprintf(f, " )");
    }
  else
    {
      if(this->d_abs_dec_ == null)
	{
	  printf("error\n");
	  exit(0);
	}
      this->d_abs_dec->write(f);
      if(this->const_expr_ == null)
	{
	  
	  if(this->param_type_list_ == null)
	    {
	      printf("error\n");
	      exit(0);
	    }
	  fprintf(f, " ( ");
	  this->param_type_list_->write(f);
	  fprintf(f, " )");
      else
	{
	  fprintf(f, " [ ");
	  this->const_expr_->write(f);
	  fprintf(f, " ] ");
	}
    }
}

void CCSTEnumSpecifier::write(FILE *f)
{
  if(this->el_ == null)
    {
      fprintf(f, "enum ");
      fprintf(f, "%s", this->id_);
    }
  else
    {
      fprintf(f, "enum ");
      fprintf(f, "%s", this->id_);
      fprintf(f, "{ ");
      this->el_->write(f);
      fprintf(F, " }");
    }
}

void CCSTEnumeratorList::write(FILE *f)
{
  for(std::vector<CCSTEnumerator*>::iterator it = list_.begin(); it != list_.end(); ++it)
    {
      CCSTEnumerator *l = *it;
      l->write(f);
      fprintf(f, ", ");
    }
}

void CCSTEnumerator::write(FILE *f)
{
  if(this->ce_ == null)
    {
      fprintf(f, "%s", this->id_);
    }
  else
    {
      fprintf(f, "%s", this->id_);
      fprintf(f, " = ");
      this->ce_->write(f);
    }
}

void CCSTTypedefName::write(FILE *f)
{
  fprintf(f, "%s", this->id_);
}

void CCSTDeclaration::write(FILE *f)
{
  //?x
  for(std::vector<CCSTDecSpecifier*>::iterator it = dec_spec_.begin(); it != dec_spec_.end(); ++it)
    {
      CCSTDecSpecifier *dec_spec = *it;
      dec_spec->write(f);
      fprintf(f, " ");
    }
  for(std::vector<CCSTInitDeclarator*>::iterator it = init_dec_.begin(); it != init_dec_.end(); ++it)
    {
      CCSTInitDeclarator *init_dec = *it;
      init_dec->write(this->out_file_);
      fprintf(f, " ");
    }
}

void CCSTInitDeclarator::write(FILE *f)
{
  // does inheritence cover just declarator case?
  if(this->dec_ == null || this->init_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->dec_->write(f);
  fprintf(f, " = ");
  this->init_->write(f);
}

void CCSTInitializer::write(FILE *f)
{
  /*
    
<initializer> ::= <assignment-expression>
                | { <initializer-list> }
                | { <initializer-list> , }
   */

  // TODO
  if(this->assn_expr_ == null)
    {
      if(this->init_list_ == null)
	{
	  printf("error\n");
	  exit(0);
	}
      fprintf(f, "{ ");
      this->init_list_->write(f);
      fprintf(f, " }");
    }
  else
    {
      this->assn_expr_->write(f);
    }
}

void CCSTInitializerList::write(FILE *f)
{
  // TODO
  for(std::vector<CCSTInitializer*>::iterator it = init_list_.begin(); it != init_list_.end(); ++it)
    {
      CCSTInitializer *init = *it;
      init->write(f);
      fprintf(f, ", ");
    }
  
}

void CCSTCompoundStatement::write(FILE *f)
{
  fprintf(f, "{\n");
  for(std::vector<CCSTDeclaration*>::iterator it = declarations_.begin(); it != declarations_.end(); ++it)
    {
      CCSTDeclaration *dec = *it;
      dec->write(f);
      fprintf(f, "\n");
    }
  for(std::vector<CCSTStatement*>::iterator it = statements_.begin(); it != statements_.end(); ++it)
    {
      CCSTStatement *state = *it;
      state->write(f);
      fprintf(f, "\n");
    }
  fprintf(f, "}");
}

void CCSTPlainLabelStatement::write(FILE *f)
{
  //todo
  fprintf(f, "%s: ", this->id_);
  this->stmnt_->write(f);
  fprintf(f, ";");
}

void CCSTCaseStatement::write(FILE *f)
{
  fprintf(f, "case ");
  this->case_label_->write(f);
  fprintf(f, ":\n");
  fprintf(f, "{\n");
  this->body_->write(f);
  fprintf(f, "\n}");
}

void CCSTExprStatement::write(FILE *f)
{
  // weird why the semicolon with no expression
  if(this->expr_ != null)
    {
      this->expr_->write(f);
    }
  fprintf(f, ";");
}

void CCSTIfStatement::write(FILE *f)
{
  fprintf(f, "if");
  fprintf(f, "( ");
  if(this->cond_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->cond_->write(f);
  fprintf(f, " )");
  fprintf(f, "\n");
  fprintf(f, "{\n");
  this->body_->write(f);
  fprintf(f, "\n");
  fprintf(f, "}");
  fprintf(f, "\n");
}

void CCSTIfElseStatement::write(FILE *f)
{
  fprintf(f, "if");
  fprintf(f, "( ");
  if(this->cond_ == null)
    {
      printf("error\n");
      exit(0);
    }
  this->cond_->write(f);
  fprintf(f, " )");
  fprintf(f, "\n");
  fprintf(f, "{\n");
  this->if_body_->write(f);
  fprintf(f, "\n");
  fprintf(f, "}");
  fprintf(f, "\n");
  fprintf(f, "else");
  fprintf(f, "\n");
  fprintf(f, "{\n");
  this->else_body_->write(f);
  fprintf(f, "\n}");
}

void CCSTSwitchStatement::write(FILE *f)
{
  if(this->expr_ == null)
    {
      printf("error\n");
      exit(0);
    }
  fprintf(f, "switch ");
  fprintf(f, "( ");
  this->expr_->write(f);
  fprintf(f, " )");
  fprintf(f, "\n");
  fprintf(f, "{\n");
  this->body_->write(); // all cases?
  fprintf(f, "\n}");
}

void CCSTWhileLoop::write(FILE *f)
{
  fprintf(f, "while");
  fprintf(f, "( ");
  this->cond_->write();
  fprintf(f, " )");
  fprintf(f, "\n");
  fprintf(f, "{");
  this->body_->write();
  fprintf(f, "\n");
  fprintf(f, "}");
}

void CCSTDoLoop::write(FILE *f)
{
  fprintf(f, "do\n");
  fprintf(f, "{\n");
  this->body_->write();
  fprintf(f, "\n}\n");
  fprintf(f, "while");
  fprintf(f, "( ");
  this->cond_->write();
  fprintf(f, " )");
  fprintf(f, ";");
}

void CCSTForLoop::write(FILE *f)
{
  // write for (
  fprintf(f, "for");
  fprintf(f, "( ");
  if(this->init_ != null)
    this->init_->write();
  fprintf(f, ";");
  if(this->cond_ != null)
    this->cond_->write();
  fprintf(f, ";");
  if(this->up_ != null)
    this->up_->write();
  fprintf(f, " )\n");
  fprintf(f, "{\n");
  this->body_->write();
  fprintf(f, "\n}");
}

void CCSTGoto::write(FILE *f)
{
  fprintf(f, "goto ");
  fprintf(f, "%s", this->identifier_);
  fprintf(f, ";");
}

void CCSTContinue::write(FILE *f)
{
  fprintf(f, "continue");// write continue ;
}

void CCSTBreak::write(FILE *f)
{
  fprintf(f, "break");
}

void CCSTReturn::write(FILE *f)
{
  if(this->expr_ == null)
    {
      fprintf(f, "return;");
    }
  else
    {
      fprintf(f, "return");
      this->expr_->write();
      fprintf(f, ";");
    }
}
