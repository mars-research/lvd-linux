
typedef enum {ObjType_Untyped, ObjType_CNode=777, ObjType_EndPoint=888, ObjType_Null} LCD_Type;

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
typedef LCD_CPtr LCD_Untyped;
typedef LCD_CPtr LCD_TCB;

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

/* internal functions */
int caps_lookup_slot(struct capability *cnode_cap, LCD_CPtr cptr, uint8_t vbits, uint8_t resolved_bits, struct cte **ret, CapRights rights);
struct cte * caps_locate_slot(struct cte * topCte, size_t offset);
void print_cnode(struct cte * myCte, int depth);
LCD_CPtr create_CNode(LCD_Untyped _service, int size_bits, LCD_CNode root, int node_index, int node_depth, int node_offset, int num_objects);
LCD_CPtr create_EndPoint(LCD_Untyped _service, int size_bits, LCD_CNode root, int node_index, int node_depth, int node_offset, int num_objects);
int cnode_create_internal(struct capability *src_cap, int size_bits);

/* externel interface */
static int LCD_Create_CSpace(void);
int LCD_Untyped_Retype(LCD_Untyped _service, LCD_Type objType, int size_bits, LCD_CNode root, int node_index, int node_depth, int node_offset, int num_objects);
static int LCD_CNode_Delete(LCD_CNode _service, int node_index, int node_depth);
static void *LCD_CNode_Search(LCD_CNode _service, int node_index, int node_depth);
