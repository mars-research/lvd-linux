#include <lcd_config/pre_hook.h>

#include <liblcd/spinlock.h>

#include <lcd_config/post_hook.h>

void lcd_spin_lock_init(struct lcd_spinlock *lk)
{
        lk->locked = 0;
}

// Acquire the lock.
// Loops (spins) until the lock is acquired.
// Holding a lock for a long time may cause
// other CPUs to waste time spinning to acquire it.
void lcd_spin_lock(struct lcd_spinlock *lk)
{
        // The xchg is atomic.
        while(xchg(&lk->locked, 1) != 0)
                ;

        // Tell the C compiler and the processor to not move loads or stores
        // past this point, to ensure that the critical section's memory
        // references happen after the lock is acquired.
        __sync_synchronize();
}

// Release the lock.
void lcd_spin_unlock(struct lcd_spinlock *lk)
{
        // Tell the C compiler and the processor to not move loads or stores
        // past this point, to ensure that all the stores in the critical
        // section are visible to other cores before the lock is released.
        // Both the C compiler and the hardware may re-order loads and
        // stores; __sync_synchronize() tells them both not to.
        __sync_synchronize();

        // Release the lock, equivalent to lk->locked = 0.
        // This code can't use a C assignment, since it might
        // not be atomic. A real OS would use C atomics here.
        asm volatile("movl $0, %0" : "+m" (lk->locked) : );

}

