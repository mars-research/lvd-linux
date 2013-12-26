
typedef enum {
    TYPE_ENDPOINT,
    TYPE_ASYNCENDPOINT,
    TYPE_TCB,
    TYPE_CNODE
}objType_t;

typedef enum {ObjType_Untyped, ObjType_CNode=777, ObjType_EndPoint, ObjType_Null} LCD_Type;

/* caps with fixed slot potitions in the root CNode */
enum {
	LCD_CapNull                =  0, /* null cap */
	LCD_CapInitThreadTCB       =  1, /* initial thread's TCB cap */
	LCD_CapInitThreadCNode     =  2, /* initial thread's root CNode cap */
	LCD_CapBootInfoFrame       =  9, /* bootinfo frame cap */
	LCD_CapInitThreadIPCBuffer = 10  /* initial thread's IPC buffer frame cap */
}; 

typedef uint32_t LCD_Word;

/* 
An address in the capability space refers to a slot which may or may not contain a capability
A capability address is stored in a CPointer
*/
typedef LCD_Word LCD_CPtr;

typedef LCD_CPtr LCD_CNode;
typedef LCD_CPtr LCD_Untyped;
typedef LCD_CPtr LCD_TCB;

int LCD_Untyped_Retype(LCD_Untyped _service, LCD_Type objType, int size_bits, LCD_CNode root, int node_index, int node_depth, int node_offset, int num_objects);

/// Sets the specified number of low-order bits to 1
#define MASK(bits)      ((1UL << bits) - 1)
#define OBJBITS_CTE 7

typedef uint32_t LCD_CPtr;
typedef uint32_t CapRights;
typedef uint32_t un; /* unknown */


#define CAPRIGHTS_READ          (1 << 0)
#define CAPRIGHTS_WRITE         (1 << 1)
#define CAPRIGHTS_EXECUTE       (1 << 2)
#define CAPRIGHTS_GRANT         (1 << 3)
#define CAPRIGHTS_IDENTIFY      (1 << 4)
#define CAPRIGHTS_NUM           5

#define CAPRIGHTS_ALLRIGHTS     ((1 << CAPRIGHTS_NUM) - 1)
#define CAPRIGHTS_READ_WRITE    (CAPRIGHTS_READ | CAPRIGHTS_WRITE)
#define CAPRIGHTS_NORIGHTS      0

struct CapRights
{

};

typedef struct endpoint
{
	int a;
	int b;
} EndPoint;

typedef struct cnode
{
	int32_t bits;
	int32_t guard;
	int32_t guard_size;
	struct cte * cnode;
}CNode;

union capability_u{
	CNode cnode;
	EndPoint endpoint;
};

struct capability
{
	LCD_Type type;
	CapRights rights;
	union capability_u u;
};

struct cte
{
	struct capability cap;
};

struct cte * caps_locate_slot(struct cte * topCte, size_t offset);
void print_cnode(struct cte * myCte, int depth);
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
