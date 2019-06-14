#ifndef _LCD_SPINLOCK_H
#define _LCD_SPINLOCK_H

typedef struct lcd_spinlock {
	unsigned int locked;  // Is the lock held?
} lcd_spinlock_t;

void lcd_spin_lock_init(struct lcd_spinlock *lk);
void lcd_spin_lock(struct lcd_spinlock *lk);
void lcd_spin_unlock(struct lcd_spinlock *lk);

#endif /* _LCD_SPINLOCK_H */
