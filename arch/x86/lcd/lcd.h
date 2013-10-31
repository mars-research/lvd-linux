#ifndef X86_LCD_H
#define X86_LCD_H

/* Address of the physical address that exists at the start of a LCD. */
#define LCD_BOOT_PHY_ADDR_START ((unsigned long)(_AC(1, UL) << 32))

/* 1MB physical memory allocated at the start of LCD for it's page table. */
#define LCD_BOOT_PHY_MEM_SIZE ((unsigned long)(_AC(1, UL) << 21))

struct lcd {
  /* TBD */
};

struct lcd* lcd_create();

#endif
