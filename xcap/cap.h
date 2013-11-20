
typedef enum {
    TYPE_ENDPOINT,
    TYPE_ASYNCENDPOINT,
    TYPE_TCB,
    TYPE_CNODE
}objType_t;

/* caps with fixed slot potitions in the root CNode */
enum {
	LCD_CapNull                =  0, /* null cap */
	LCD_CapInitThreadTCB       =  1, /* initial thread's TCB cap */
	LCD_CapInitThreadCNode     =  2, /* initial thread's root CNode cap */
	LCD_CapBootInfoFrame       =  9, /* bootinfo frame cap */
	LCD_CapInitThreadIPCBuffer = 10  /* initial thread's IPC buffer frame cap */
}; 

typedef uint32_t LCD_Word;

typedef LCD_Word LCD_CPtr;

typedef LCD_CPtr LCD_CNode;
typedef LCD_CPtr LCD_TCB;

/*
static int LCD_TCB_SetSpace(LCD_CPtr LCD_CapInitThreadCNode);
static int LCD_Create_Object(objType_t type);

*/
#if 0
#define IWANA_MAX_LEVELS  3

struct iwana_cspace {
	/* The caps to the current vertical slice of the cspace we are allocating
	 * out of. */
	struct {
		seL4_CPtr cptr;
		int offset;
	} cnodes[IWANA_MAX_LEVELS];

	/* Size (in bits) of each level in the CNode. */
	int depths[IWANA_MAX_LEVELS];

	/*
	 * Size (in bits) of each level, taking into account guards.
	 *
	 * This is used when:
	 *
	 *    (i)  We are trying to address a slot in a CNode of this level; and
	 *
	 *    (ii) That CNode was possibly created by an external party, with
	 *         its cap having a guard.
	 */
	int guarded_depths[IWANA_MAX_LEVELS];

	/* Upper limit on the size of the CSpace. */
	unsigned long cspace_size_bits;

	/* Number of guard bits on the top level. */
	int guard_bits;

	/* Number of levels in the cspace. */
	int levels;

	/* First free cap available in the client's CSpace. */
	seL4_CPtr next_cap;
};
#endif
