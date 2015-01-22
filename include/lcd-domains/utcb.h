#ifndef LCD_DOMAINS_UTCB_H
#define LCD_DOMAINS_UTCB_H

/*
 * User thread control block
 * =========================
 *
 * This is accessible (mapped) inside the lcd.
 */
struct lcd_utcb {
	u64 mr[8];
};

#endif /* LCD_DOMAINS_UTCB_H */
