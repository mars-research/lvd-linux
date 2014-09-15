/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifndef LCD_PROTOTYPE_DEALER_H
#define LCD_PROTOTYPE_DEALER_H

#include "../include/dstore.h"

struct engine {
	int cylinders;
	dsptr_t self;	
};

struct automobile {
	struct engine *engine;
	int doors;
	dsptr_t self;	
};

struct manufacturer_interface {
	struct engine * (*mk_engine) (int cylinders);
	struct automobile * (*mk_automobile) (struct engine *e, int doors);
	void (*free_engine)(struct engine *e);
	void (*free_automobile)(struct automobile *a);
};

int dealer_register_manufacturer(struct manufacturer_interface *s);
struct automobile * dealer_buy_car(void);
void dealer_return_car(struct automobile *a);

#endif
