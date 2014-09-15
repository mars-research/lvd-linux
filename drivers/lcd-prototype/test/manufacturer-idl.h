/*
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */

#ifndef LCD_PROTOTYPE_MANUFACTURER_IDL_H
#define LCD_PROTOTYPE_MANUFACTURER_IDL_H

enum manufacturer_interface_enum {
	MANUFACTURER_MK_ENGINE,
	MANUFACTURER_MK_AUTOMOBILE,
	MANUFACTURER_FREE_ENGINE,
	MANUFACTURER_FREE_AUTOMOBILE,
	MANUFACTURER_DIE,
};

/* Locations of manufacturer's boot cptrs */
#define MANUFACTURER_DEALER_INTERFACE_CAP 31

#endif
