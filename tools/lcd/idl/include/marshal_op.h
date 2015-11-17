#ifndef MARSHAL_OP_H
#define MARSHAL_OP_H

#define LCD_MAX_REGS 64
#define LCD_MAX_CAP_REGS 8

/* DEFINITIONS OF FUNCTIONS TO ACCESS REGISTERS */
#define REG0 "lcd_r0"
#define REG1 "lcd_r1"
#define REG2 "lcd_r2"
#define REG3 "lcd_r3"
#define REG4 "lcd_r4"
#define REG5 "lcd_r5"
#define REG6 "lcd_r6"
#define REG7 "lcd_r7"
#define REG8 "lcd_r8"
#define REG9 "lcd_r9"
#define REG10 "lcd_r10"
#define REG11 "lcd_r11"
#define REG12 "lcd_r12"
#define REG13 "lcd_r13"
#define REG14 "lcd_r14"
#define REG15 "lcd_r15"
#define REG16 "lcd_r16"
#define REG17 "lcd_r17"
#define REG18 "lcd_r18"
#define REG19 "lcd_r19"
#define REG20 "lcd_r20"
#define REG21 "lcd_r21"
#define REG22 "lcd_r22"
#define REG23 "lcd_r23"
#define REG24 "lcd_r24"
#define REG25 "lcd_r25"
#define REG26 "lcd_r26"
#define REG27 "lcd_r27"
#define REG28 "lcd_r28"
#define REG29 "lcd_r29"
#define REG30 "lcd_r30"
#define REG31 "lcd_r31"
#define REG32 "lcd_r32"
#define REG33 "lcd_r33"
#define REG34 "lcd_r34"
#define REG35 "lcd_r35"
#define REG36 "lcd_r36"
#define REG37 "lcd_r37"
#define REG38 "lcd_r38"
#define REG39 "lcd_r39"
#define REG40 "lcd_r40"
#define REG41 "lcd_r41"
#define REG42 "lcd_r42"
#define REG43 "lcd_r43"
#define REG44 "lcd_r44"
#define REG45 "lcd_r45"
#define REG46 "lcd_r46"
#define REG47 "lcd_r47"
#define REG48 "lcd_r48"
#define REG49 "lcd_r49"
#define REG50 "lcd_r50"
#define REG51 "lcd_r51"
#define REG52 "lcd_r52"
#define REG53 "lcd_r53"
#define REG54 "lcd_r54"
#define REG55 "lcd_r55"
#define REG56 "lcd_r56"
#define REG57 "lcd_r57"
#define REG58 "lcd_r58"
#define REG59 "lcd_r59"
#define REG60 "lcd_r60"
#define REG61 "lcd_r61"
#define REG62 "lcd_r62"
#define REG63 "lcd_r63"
/* END FUNCTIONS TO ACCESS REGISTERS*/

/* DEFINITIONS OF FUNCTIONS TO STORE IN REGISTERS */
#define STORE_REG0 "lcd_store_r0"
#define STORE_REG1 "lcd_store_r1"
#define STORE_REG2 "lcd_store_r2"
#define STORE_REG3 "lcd_store_r3"
#define STORE_REG4 "lcd_store_r4"
#define STORE_REG5 "lcd_store_r5"
#define STORE_REG6 "lcd_store_r6"
#define STORE_REG7 "lcd_store_r7"
#define STORE_REG8 "lcd_store_r8"
#define STORE_REG9 "lcd_store_r9"
#define STORE_REG10 "lcd_store_r10"
#define STORE_REG11 "lcd_store_r11"
#define STORE_REG12 "lcd_store_r12"
#define STORE_REG13 "lcd_store_r13"
#define STORE_REG14 "lcd_store_r14"
#define STORE_REG15 "lcd_store_r15"
#define STORE_REG16 "lcd_store_r16"
#define STORE_REG17 "lcd_store_r17"
#define STORE_REG18 "lcd_store_r18"
#define STORE_REG19 "lcd_store_r19"
#define STORE_REG20 "lcd_store_r20"
#define STORE_REG21 "lcd_store_r21"
#define STORE_REG22 "lcd_store_r22"
#define STORE_REG23 "lcd_store_r23"
#define STORE_REG24 "lcd_store_r24"
#define STORE_REG25 "lcd_store_r25"
#define STORE_REG26 "lcd_store_r26"
#define STORE_REG27 "lcd_store_r27"
#define STORE_REG28 "lcd_store_r28"
#define STORE_REG29 "lcd_store_r29"
#define STORE_REG30 "lcd_store_r30"
#define STORE_REG31 "lcd_store_r31"
#define STORE_REG32 "lcd_store_r32"
#define STORE_REG33 "lcd_store_r33"
#define STORE_REG34 "lcd_store_r34"
#define STORE_REG35 "lcd_store_r35"
#define STORE_REG36 "lcd_store_r36"
#define STORE_REG37 "lcd_store_r37"
#define STORE_REG38 "lcd_store_r38"
#define STORE_REG39 "lcd_store_r39"
#define STORE_REG40 "lcd_store_r40"
#define STORE_REG41 "lcd_store_r41"
#define STORE_REG42 "lcd_store_r42"
#define STORE_REG43 "lcd_store_r43"
#define STORE_REG44 "lcd_store_r44"
#define STORE_REG45 "lcd_store_r45"
#define STORE_REG46 "lcd_store_r46"
#define STORE_REG47 "lcd_store_r47"
#define STORE_REG48 "lcd_store_r48"
#define STORE_REG49 "lcd_store_r49"
#define STORE_REG50 "lcd_store_r50"
#define STORE_REG51 "lcd_store_r51"
#define STORE_REG52 "lcd_store_r52"
#define STORE_REG53 "lcd_store_r53"
#define STORE_REG54 "lcd_store_r54"
#define STORE_REG55 "lcd_store_r55"
#define STORE_REG56 "lcd_store_r56"
#define STORE_REG57 "lcd_store_r57"
#define STORE_REG58 "lcd_store_r58"
#define STORE_REG59 "lcd_store_r59"
#define STORE_REG60 "lcd_store_r60"
#define STORE_REG61 "lcd_store_r61"
#define STORE_REG62 "lcd_store_r62"
#define STORE_REG63 "lcd_store_r63"
/* END FUNCTIONS TO STORE IN REGISTERS */


#include <stdlib.h>
#include "assert.h"
#include "lcd_ast.h"
#include "ccst.h"

const char* access_register_mapping(int register_index);

const char* store_register_mapping(int register_index);

class Type;
class FunctionPointer;
class Parameter;
class IntegerType;
class ProjectionType;
class ProjectionField;
class VoidType;
class PointerType;
class Typedef;
class Rpc;
class File;
class TypeVisitor;
class CCSTCompoundStatement;
class ReturnVariable;

class Registers
{
  int regs_ [LCD_MAX_REGS];
  int cap_regs_ [LCD_MAX_CAP_REGS];
  void init();
 public:
  Registers();
  // finds next free register and set it as allocated;
  int allocate_next_free_register();
  
};

class Marshal_type
{
 public:
  virtual void set_register(int r) = 0;
  virtual int get_register() = 0;
};

class Marshal_projection : public Marshal_type
{
 public:
  Marshal_projection();
  virtual void set_register( int r);
  virtual int get_register();
};

class Marshal_integer : public Marshal_type
{
  int register_;
 public:
  Marshal_integer(int r);
  virtual void set_register(int r);
  virtual int get_register();};

class Marshal_void : public Marshal_type
{
 public:
  Marshal_void();
  virtual void set_register(int r);
  virtual int get_register();
};

class Marshal_typedef : public Marshal_type
{
  Marshal_type *true_type_;
 public:
  Marshal_typedef(Marshal_type *type);
  virtual void set_register(int r);
  virtual int get_register();
};


class MarshalPrepareVisitor
{
  Registers *registers_;
 public:
  MarshalPrepareVisitor(Registers *r);
  Marshal_type* visit(FunctionPointer *fp);
  Marshal_type* visit(Typedef *td);
  Marshal_type* visit(VoidType *vt);
  Marshal_type* visit(IntegerType *it);
  Marshal_type* visit(ProjectionType *pt);
};



#endif
