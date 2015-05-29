#include "marshal_op.h"

M_rpc::M_rpc()
{
  calling_regs = new int[LCD_MAX_REGS];
  return_regs = new int[LCD_MAX_REGS];
  cap_regs = new int[LCD_MAX_CAP_REGS];
  init();
}

void M_rpc::init()
{
  int i;
  for(i = 0; i < LCD_MAX_REGS; i ++)
    calling_regs[i] = 0;
  for(i = 0; i < LCD_MAX_CAP_REGS; i ++)
    cap_regs[i] = 0;
  for(i = 0; i < LCD_MAX_REGS; i ++)
    return_regs[i] = 0;
}

std::vector<int> M_rpc::allocate_registers(Type *t)
{
  switch(t->num())
    {
    case 1: // typedef
      {
	Typedef *td = dynamic_cast<Typedef*>(t);
	return allocate_registers(td->type());
      }
    case 2: // Integer
      {
	IntegerType *it = dynamic_cast<IntegerType*>(t);
	std::vector<int> ret;
	ret.push_back(integer_allocate_register(it));
	return ret;
      }
    case 3: // Pointer type
      {
	PointerType *pt = dynamic_cast<PointerType*>(t);
	return allocate_registers(pt->type());
      }
    case 4: // Projection type
      {
	ProjectionType *pt = dynamic_cast<ProjectionType*>(t);
	return projection_allocate_registers(pt);
      }
    case 5: // Void type
      {
	std::vector<int> ret;
	return ret;
      }
    default:
      {
        Assert(1 == 0, "Error: Unknown type.\n");
      }
    }
}


std::vector<int> M_rpc::projection_allocate_registers(ProjectionType *pt)
{
  std::vector<ProjectionField*> tmp = pt->fields();

  std::vector<int> projection_registers;
  for(std::vector<ProjectionField*>::iterator it = tmp.begin(); it != tmp.end(); it ++)
    {
      ProjectionField *pf = *it;
      std::vector<int> pf_regs = proj_field_allocate_registers(pf);
      
      std::vector<int>::iterator it2 = projection_registers.end();
      projection_registers.insert(it2, pf_regs.begin(), pf_regs.end());
      // Each projection field should store its own register, registers?

      // Where else will they be stored?

      // In parameter, but storing in parameter could be complicated if want to arrange 
      // them in a weird way.

      // Storing register info in projection
      // supports idea of storing register info in Types. but that feels wrong.
    }
  return projection_registers;
}

std::vector<int> M_rpc::proj_field_allocate_registers(ProjectionField *pf)
{
  if(pf->in()) // "return" to caller
    {
      // Just assume real function may have modified these...
      // and "return" them
    } 
  if(pf->out()) // pass as parameter to callee.
    {
      // could be a projection.
      return allocate_registers(pf->type());
    }
  Assert(1 == 0, "Error: must be in or out\n");
}

int M_rpc::integer_allocate_register(IntegerType *t)
{
  switch(t->int_type())
    {
    case pt_char_t:
      {
	int r = next_free_calling_register();
	Assert(r != -1, "Error: no free registers\n");
	return r;
      }
    case pt_short_t:
      {
        int r = next_free_calling_register();
	Assert(r != -1, "Error: no free registers\n");
	return r;
      }
    case pt_int_t:
      {
        int r = next_free_calling_register();
	Assert(r != -1, "Error: no free registers\n");
	return r;
      }
    case pt_long_t:
      {
        int r = next_free_calling_register();
	Assert(r != -1, "Error: no free registers\n");
	return r;
      }
    case pt_longlong_t:
      {
	int r = next_free_calling_register();
	Assert(r != -1, "Error: no free registers\n");
	return r;
      }
    case pt_capability_t:
      {
        int r = next_free_calling_register();
	Assert(r != -1, "Error: no free registers\n");
	return r;
      }
    default:
      {
        Assert(1 == 0, "Error: Unknown integer type.\n");
      }
    }
}

/* 
 * Finds next free register, then
 * sets it as used
 */
int M_rpc::next_free_calling_register()
{
  int i;
  for(i = 0; i < LCD_MAX_REGS; i ++)
    {
      if(!calling_regs[i])
	{
	  calling_regs[i] = 1;
	  return i;
	}
    }
  return -1; // no free regs;
}

int M_rpc::next_free_return_register()
{
  int i;
  for(i = 0; i < LCD_MAX_REGS; i ++)
    {
      if(!return_regs[i])
	{
	  return_regs[i] = 1;
	  return i;
	}
    }
  return -1; // no free regs;
}

M_type::M_type()
{
  this->size_ = 0;
}

void M_type::set_size(size_t s)
{
  this->size_ = s;
}


