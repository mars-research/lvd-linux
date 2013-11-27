#ifndef LCD_LCD_H
#define LCD_LCD_H

int lcd_move_module(struct module *mod);
int lcd_init_module(void __user * umod, unsigned long len, const char __user * uargs);
int lcd_load_module(struct load_info *info, const char __user *uargs, int flags);

#endif
