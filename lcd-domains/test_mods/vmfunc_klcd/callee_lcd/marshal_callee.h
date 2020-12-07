#ifndef __MARSHAL_CALLEE_H_
#define __MARSHAL_CALLEE_H_

#include <libfipc.h>

int marshal_int_callee(struct fipc_message *msg);
int marshal_array_callee(struct fipc_message *msg);
int marshal_string_callee(struct fipc_message *msg);
int marshal_voidptr_callee(struct fipc_message *msg);

#endif /* __MARSHAL_CALLEE_H_ */
