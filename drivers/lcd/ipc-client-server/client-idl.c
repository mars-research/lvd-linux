
#include "server-idl.h"

struct server_interface {
	int (*test_func1) (unsigned long a, long b, char c);
	int (*test_func2) (unsigned long a);
};


/* Caller stubs */
int register_server(struct server_interface *server) {


}


