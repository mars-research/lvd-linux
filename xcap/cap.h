typedef enum {
    TYPE_ENDPOINT,
    TYPE_ASYNCENDPOINT,
    TYPE_TCB,
    TYPE_CNODE
}objType_t;

static int LCD_Create_CSpace(void);
static int LCD_Create_Object(objType_t type);
