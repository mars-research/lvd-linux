/*
 * irq.h
 *
 * Enable and disable IRQs.
 *
 * Motivation: You want to configure the LCD so that
 * external interrupts do not cause a VM exit. Note that
 * if you invoke a hypercall that causes a VM exit, the
 * microkernel may choose to enable interrupts, and may
 * get preempted at that point.
 */
#ifdef LIBLCD_IRQ_H
#define LIBLCD_IRQ_H

/**
 * lcd_irq_disable
 *
 * For isolated code, disables interrupts and prevents interrupts
 * from causing a VM exit. Note that NMIs will still trigger an
 * exit.
 *
 * This is a no-op for non-isolated code. You should use the
 * regular local_irq_disable.
 */
void lcd_irq_disable(void);
/**
 * lcd_irq_enable
 *
 * For isolated code, unconditionally re-enables interrupts. Interrupts
 * will cause a VM exit now.
 *
 * Again, this is a no-op for non-isolated code.
 */
void lcd_irq_enable(void);

#endif /* LIBLCD_IRQ_H */
