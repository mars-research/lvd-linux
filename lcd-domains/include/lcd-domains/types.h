/*
 * types.h
 *
 * Common types and macros used throughout the interface and microkernel.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_TYPES_H
#define LCD_DOMAINS_TYPES_H

/**
 * Return value to signal that a host/non-isolated resource (like a page)
 * is not part of the capability system (i.e., is not in any thread's
 * cspace).
 */
#define LCD_ERR_NOT_VOLUNTEERED (-512)

/*
 * Extra reserved cnodes:
 *
 * cptr = 1 is the LCD's call endpoint
 * cptr = 2 points to an endpoint if the lcd did a receive, and the sender did
 *          a call (so the lcd can do a reply)
 *
 * So, if lcd A does a call on endpoint #1 and lcd B does a receive on endpoint
 * #1, the endpoint at LCD_CPTR_CALL_ENDPOINT in A's cspace will be granted
 * to lcd B in B's cspace at cptr LCD_CPTR_REPLY_ENDPOINT. lcd B can do a reply
 * (one time and then it's revoked).
 */
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

/* MACRO HELPERS -------------------------------------------------- */

/* This may not be the ideal place for this stuff. */

/* Motivated by original module name max */
#define LCD_MODULE_NAME_MAX (64 - sizeof(unsigned long))

/* 
 * Max absolute path size to lcd kernel module .ko, i.e., a full
 * path like:
 *
 *    /foo/bar/baz/the_module.ko
 *
 * Note that the name part (the_module) must be less or equal to
 * LCD_MODULE_NAME_MAX.
 */
#define LCD_MPATH_SIZE 256

#define LCD_TEST_MODS_PATH \
	"/local/disk2/xcap-git/lcd-domains/test-mods"

#define LCD_DIR(subpath) \
	LCD_TEST_MODS_PATH "/" subpath


#endif /* LCD_DOMAINS_TYPES_H */
