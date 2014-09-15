/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifndef LCD_PROTOTYPE_DEALER_IDL_H
#define LCD_PROTOTYPE_DEALER_IDL_H

enum dealer_interface {
	DEALER_BUY_CAR,
	DEALER_RETURN_CAR,
	DEALER_REGISTER_MANUFACTURER,
	DEALER_DIE,
};

#define DEALER_DEALER_INTERFACE_CAP 31
#define DEALER_MANUFACTURER_OBJ_CAP 30

#endif
