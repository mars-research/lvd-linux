/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifndef LCD_IPC_AUTO_DEALER_H
#define LCD_IPC_AUTO_DEALER_H

struct engine {
	int cylinders;
};

struct automobile {
	struct engine *engine;
	int doors;
};

struct manufacturer_interface {
	struct engine * (*mk_engine) (int cylinders);
	struct automobile * (*mk_automobile) (struct engine *e, int doors);
	void (*free_engine)(struct engine *e);
	void (*free_automobile)(struct automobile *a);
};

int dealer_register_manufacturer(struct server_interface *s);
struct automobile * dealer_buy_car(void);

#endif
