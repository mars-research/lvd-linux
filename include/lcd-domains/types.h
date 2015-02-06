/* 
 * types.h
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */
#ifndef LCD_DOMAINS_TYPES_H
#define LCD_DOMAINS_TYPES_H

#include <asm/page.h>

/* CPTRs -------------------------------------------------- */

typedef struct { unsigned long cptr; } cptr_t;

static inline cptr_t __cptr(unsigned long cptr)
{
	return (cptr_t){ cptr };
}
static inline unsigned long cptr_val(cptr_t c)
{
	return c.cptr;
}

/*
 * Reserved cnodes:
 *
 * cptr = 0 is always null
 * cptr = 1 is the lcd's call endpoint
 * cptr = 2 points to an endpoint if the lcd did a receive, and the sender did
 *          a call (so the lcd can do a reply)
 *
 * So, if lcd A does a call on endpoint #1 and lcd B does a receive on endpoint
 * #1, the endpoint at LCD_CAP_CALL_ENDPOINT in A's cspace will be granted
 * to lcd B in B's cspace at cptr LCD_CAP_REPLY_ENDPOINT. lcd B can do a reply
 * (one time and then it's revoked).
 */
#define LCD_CPTR_NULL __cptr(0)
#define LCD_CPTR_CALL_ENDPOINT __cptr(1)
#define LCD_CPTR_REPLY_ENDPOINT __cptr(2)


/* ADDRESS SPACE TYPES ---------------------------------------- */

/* XXX: Assumes host and guest run in 64-bit mode */

typedef struct { unsigned long gva; } gva_t;
typedef struct { unsigned long hva; } hva_t;
typedef struct { unsigned long gpa; } gpa_t;
typedef struct { unsigned long hpa; } hpa_t;

static inline gva_t __gva(unsigned long gva)
{
	return (gva_t){ gva };
}
static inline unsigned long gva_val(gva_t gva)
{
	return gva.gva;
}
static inline unsigned long * gva_ptr(gva_t * gva)
{
	return &(gva->gva);
}
static inline gva_t gva_add(gva_t gva, unsigned long off)
{
	return __gva(gva_val(gva) + off);
}
static inline hva_t __hva(unsigned long hva)
{
	return (hva_t){ hva };
}
static inline unsigned long hva_val(hva_t hva)
{
	return hva.hva;
}
static inline unsigned long * hva_ptr(hva_t * hva)
{
	return &(hva->hva);
}
static inline hva_t hva_add(hva_t hva, unsigned long off)
{
	return __hva(hva_val(hva) + off);
}
static inline gpa_t __gpa(unsigned long gpa)
{
	return (gpa_t){ gpa };
}
static inline unsigned long gpa_val(gpa_t gpa)
{
	return gpa.gpa;
}
static inline unsigned long * gpa_ptr(gpa_t * gpa)
{
	return &(gpa->gpa);
}
static inline gpa_t gpa_add(gpa_t gpa, unsigned long off)
{
	return __gpa(gpa_val(gpa) + off);
}
static inline hpa_t __hpa(unsigned long hpa)
{
	return (hpa_t){ hpa };
}
static inline unsigned long hpa_val(hpa_t hpa)
{
	return hpa.hpa;
}
static inline unsigned long * hpa_ptr(hpa_t * hpa)
{
	return &(hpa->hpa);
}
static inline hpa_t hpa_add(hpa_t hpa, unsigned long off)
{
	return __hpa(hpa_val(hpa) + off);
}
static inline hpa_t pa2hpa(unsigned long pa)
{
	return (hpa_t){ pa };
}
static inline hpa_t va2hpa(void *va)
{
	return (hpa_t){ __pa(va) };
}
static inline void * hpa2va(hpa_t hpa)
{
	return __va(hpa_val(hpa));
}
static inline hva_t hpa2hva(hpa_t hpa)
{
	return (hva_t){ (unsigned long)__va(hpa.hpa) };
}
static inline void * hva2va(hva_t hva)
{
	return (void *)hva_val(hva);
}
static inline hva_t va2hva(void *va)
{
	return __hva((unsigned long)va);
}
static inline hpa_t hva2hpa(hva_t hva)
{
	return (hpa_t){ (unsigned long)__pa(hva2va(hva)) };
}

#endif   /* LCD_DOMAINS_TYPES_H */
