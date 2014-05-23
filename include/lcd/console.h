/* 
 * Anton Burtsev, University of Utah, 2014
 */

#ifndef _LCD_CONSOLE_H_
#define _LCD_CONSOLE_H_

// %rax not getting saved automatically
static inline void
lcd_putc(char c)
{
	asm volatile("push %rax");
	asm volatile("movzx %0,%%rax" : : "r" (c));
	asm volatile("vmcall");
	asm volatile("pop %rax");
}

static inline void
lcd_printint(long xx, int base, int sgn)
{
    static char digits[] = "0123456789ABCDEF";
    char buf[16];
    int i, neg;
    u64 x;
    
    neg = 0;
    if(sgn && xx < 0){
        neg = 1;
        x = -xx;
    } else {
        x = xx;
    }
    
    i = 0;
    do{
        buf[i++] = digits[x % base];
       // lcd_putc('*');
    }while((x /= base) != 0);
    if(neg)
        buf[i++] = '-';
    
    while(--i >= 0)
        lcd_putc(buf[i]);
}


// Print to the given fd. Only understands %d, %x, %p, %s.
static inline void
lcd_printf(char *fmt, ...)
{
    va_list ap;
    char *s;
    int c, i, state;
    va_start(ap, fmt);
    
    state = 0;
    for(i = 0; fmt[i]; i++){
        c = fmt[i] & 0xff;
        if(state == 0){
            if(c == '%'){
                state = '%';
            } else {
                lcd_putc(c);
            }
        } else if(state == '%'){
            if(c == 'd'){
                lcd_printint(va_arg(ap, long), 10, 1);
            } else if(c == 'x' || c == 'p'){
                lcd_printint(va_arg(ap, long), 16, 0);
            } else if(c == 's'){
                s = va_arg(ap, char*);
                if(s == 0)
                    s = "(null)";
                while(*s != 0){
                    lcd_putc(*s);
                    s++;
                }
            } else if(c == 'c'){
                lcd_putc(va_arg(ap, uint));
            } else if(c == '%'){
                lcd_putc(c);
            } else {
                // Unknown % sequence.  Print it to draw attention.
                lcd_putc('%');
                lcd_putc(c);
            }
            state = 0;
        }
    }
}


#endif /* _LCD_CONSOLE_H_ */
