/*
 * create.h
 *
 * Functions for creating and configuring LCDs.
 *
 *    - lcd_create - creates a bare LCD
 *
 *    - lcd_config_registers - set the LCD's %rip, %rsp, etc.
 *
 *    - lcd_memory_grant_and_map - configure LCD's guest physical address
 *                                 space; set up LCD's guest physical address 
 *                                 space
 *
 *    - lcd_cap_grant - configure LCD's initial cspace; 
 *                      grant LCD a capability (e.g. to a sync endpoint)
 *
 *    - lcd_to_boot_info  - get bootstrap memory for LCD; you can store
 *                          data inside this memory, and the LCD will be able
 *                          to access it when it boots; see boot_info.h
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_CREATE_H
#define LCD_DOMAINS_CREATE_H

#include <linux/list.h>
#include <libcap.h>
#include <lcd_domains/types.h>
#include <liblcd/boot_info.h>


/* LOW-LEVEL CREATE, CONFIG, RUN ---------------------------------------- */

/**
 * lcd_create -- Create an empty, unconfigured LCD
 * @lcd: out param, cptr to lcd capability
 *
 * Creates a bare LCD with an empty cspace and address space (nothing is
 * mapped it - except the dedicated page for the LCD's UTCB - see sync_ipc.h).
 */
int lcd_create(cptr_t *lcd);

/**
 * lcd_create_klcd -- Create an LCD that runs in the non-isolated host
 * @klcd: out param, cptr to klcd capability
 *
 * Create a non-isolated `LCD'. This essentially initializes everything
 * except the low-level hardware VM container.
 *
 * The klcd can be configured etc. just like a regular lcd (program counter,
 * stack - though you should probably use the same stack that was set up
 * by the kernel when the kthread was created, and so on). You can grant
 * the klcd capabilities too.
 *
 * Obviously, only non-isolated code can do this. For isolated code, this
 * function returns an error.
 */
int lcd_create_klcd(cptr_t *klcd);

/**
 * lcd_config_registers -- Set some of the LCD's registers with initial values
 * @lcd: cptr to lcd capability
 * @pc: initial program counter when LCD boots, as guest virtual address
 * @sp: initial stack pointer when LCD boots, as guest virtual address
 * @gva_root: root of guest virtual paging hierarchy, as guest *physical* addr
 * @utcb_page: guest physical address of LCD's sync IPC message buffer (the
 *             microkernel allocates and manages this page so it can't
 *             be freed from underneath it during sync IPC transfers)
 *
 * Use this function to set the LCD's initial program counter (%rip),
 * stack pointer, and so on.
 *
 * For now, we assume lcd will boot with a guest virtual address space.
 */
int lcd_config_registers(cptr_t lcd, gva_t pc, gva_t sp, gpa_t gva_root,
				 gpa_t utcb_page);

/**
 * lcd_memory_grant_and_map -- Grant LCD access to memory object, and map it 
 *                             in its *guest physical* address space
 * @lcd: the LCD to configure
 * @mo: cptr to memory object capability in *caller* cspace
 * @dest_slot: slot in LCD's cspace where capability copied/granted to
 * @base: starting guest physical address where memory should be mapped
 *
 * This function combines two operations because we want to enforce the
 * invariant that an LCD has a memory mapped in its address space only if
 * it has a capability to that memory. (Of course, it can have capabilities
 * to memory that isn't mapped.)
 *
 * The caller is responsible for knowing what slots are free in the LCD's
 * cspace.
 *
 * If the memory referenced by the capability does not fit
 * in the LCD's guest physical address space, starting at @base, then
 * this function fails.
 */
int lcd_memory_grant_and_map(cptr_t lcd, cptr_t mo, cptr_t dest_slot,
			gpa_t base);

/**
 * lcd_cap_grant -- Grant the LCD a capability (config its cspace)
 * @lcd: the LCD to configure
 * @src: cptr to capability in the *caller* cspace to grant
 * @dest: slot in LCD's cspace where capability should be stored
 */
int lcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest);
/**
 * lcd_set_struct_module_hva -- Remember the hva of the struct module copy
 * @lcd: LCD that is using the module
 * @hva: the host virtual address of the struct module copy
 *
 * Motivation: We unload the modules from the host now after they have been
 * loaded into the LCD. But the struct module is still around, its just in
 * the dup pages. The creator (some non-isolated code) should tell us
 * where. Then, in stack traces, we can use the struct module to resolve
 * addresses -> symbols.
 *
 * If @lcd is not isolated, this returns an error.
 *
 * For isolated code calling this function, also returns error.
 */
int lcd_set_struct_module_hva(cptr_t lcd, hva_t hva);
/**
 * lcd_run -- Run an LCD
 * @lcd: the LCD to run
 */
int lcd_run(cptr_t lcd);

/* HIGHER-LEVEL LCD CREATE FROM KERNEL MODULE ------------------------------ */

/**
 * This context is used during LCD creation / destruction.
 */
struct lcd_create_ctx {
	/*
	 * Module name
	 */
	char mname[LCD_MODULE_NAME_MAX];
	/*
	 * All of these are *creator-relative* addresses
	 */
	struct lcd_boot_info *lcd_boot_info;
	void *gv_pg_tables;
	void *stack;
	void *m_init_bits;
	void *m_core_bits;
};

/**
 * lcd_create_module_lcd -- Create an LCD and load a kernel module in it
 * @mdir: host absolute path to directory containing kernel module
 * @mname: kernel module file name
 * @lcd: out param, cptr to created lcd capability
 * @ctx: out param, lcd_create_ctx, allocated and used by this function
 *
 * IMPORTANT: You must also call lcd_dump_boot_info before running the LCD.
 * Create and dump boot info are decoupled so that you can store values
 * yourself in the LCD's bootstrap pages *after* create but *before* running
 * it.
 * 
 * Big routine to automatically create an lcd, load a module inside it,
 * and configure it per the address space layout on the wiki.
 *
 * The LCD's cspace will contain capabilities to the pages that
 * contain the kernel module, guest virtual paging tables, bootstrap
 * pages, and stack pages. (The LCD *does not* get a capability to the
 * UTCB page. This page cannot disappear under the microkernel's toes
 * during an IPC transfer, so we don't allow LCD's to manage it.)
 *
 * Also returns an lcd_create_ctx that you should pass to
 * lcd_destroy_module_lcd to tear everything down (destroy the LCD,
 * unload the kernel module, etc.) So long as you don't pass the capability 
 * to any other lcd, etc., this will stop and destroy the lcd.
 */
int lcd_create_module_lcd(char *mdir, char *mname, cptr_t *lcd,
			struct lcd_create_ctx **ctx);

/**
 * lcd_to_boot_info -- Extract lcd_boot_info from create context
 * @ctx: the lcd create ctx returned from lcd_create_module_lcd
 *
 * Use this to pull out the struct. Then you can store your own values
 * in the fields for dumping the info.
 */
static inline struct lcd_boot_info * 
lcd_to_boot_info(struct lcd_create_ctx *ctx)
{
	return ctx->lcd_boot_info;
}
/**
 * lcd_to_boot_cptr_cache -- Extract new LCD's bootstrap cptr cache
 * @ctx: the lcd create ctx returned from lcd_create_module_lcd
 */
static inline struct cptr_cache * 
lcd_to_boot_cptr_cache(struct lcd_create_ctx *ctx)
{
	return &ctx->lcd_boot_info->cptr_cache;
}
/**
 * lcd_dump_virt_addr_space -- Print the virtual addr space in @ctx
 *
 * This is for debugging the virtual address space set up. Invoking this
 * function will dump the virtual page tables set up for the LCD (the data
 * in @ctx->gv_page_tables). This does not do page walks via %cr3. In the
 * future, this function could be implemented differently for isolated
 * and non-isolated code. Non-isolated code could use the EPTs to resolve
 * guest physical to host physical, and do a full %cr3 based traversal (this
 * isn't possible for isolated code since it can't access EPTs).
 */
void lcd_dump_virt_addr_space(struct lcd_create_ctx *ctx);
/**
 * lcd_destroy_create_ctx -- Tear down lcd_create_ctx returned from
 *                           lcd_create_module_lcd
 * @ctx: the ctx to destroy
 *
 * This *releases* all of the memory used to initialize the new LCD.
 * It will not destroy the new LCD itself, and since the LCD was granted
 * capabilities to the boot and stack pages, module pages, and so on,
 * it will continue to have access to them. Only when you delete
 * the lcd capability will this trigger the full tear down (so long
 * as no one else was granted the lcd capability).
 */
void lcd_destroy_create_ctx(struct lcd_create_ctx *ctx);

/* What about lcd_destroy_module_lcd? Use lcd_cap_delete on the lcd
 * capability. */

/* HIGHER-LEVEL KLCD CREATE FROM KERNEL MODULE -------------------- */

/**
 * lcd_create_module_klcd -- Create a non-isolated LCD
 * @mdir: the host directory that contains the module to load
 * @mname: the name of the kernel module (e.g., foo.ko)
 * @klcd: out param, points to new klcd in caller's cspace
 *
 * This basically loads a kernel module (with name mname) and creates a 
 * kernel thread to run its module init. This allows the caller to grant 
 * capabilities to a non-isolated thread (since it will have a capability
 * to the container).
 *
 * Obviously, isolated code cannot create a klcd, so this function
 * fails for isolated code.
 *
 * IMPORTANT: Unlike the isolated LCD version, you must call 
 * lcd_destroy_module_klcd to kill the kLCD (this tells the kLCD
 * thread to stop, waits for it to exit, and then it unloads the
 * kernel module). If instead you just do lcd_cap_delete on the klcd
 * capability, the module would remain loaded in the host.
 */
int lcd_create_module_klcd(char *mdir, char *mname, cptr_t *klcd);

/**
 * lcd_destroy_module_klcd -- Destroys non-isolated LCD / thread 
 * @klcd: cptr to klcd capability in caller's cspace
 * @mname: the name of the kernel module that was loaded into the KLCD
 *
 * Note: This will have to wait until the kthread exits the module's init, 
 * so this may block for an arbitrarily long period of time (recall that
 * klcd's are trusted). Unloads the module with name mname. It's up to you 
 * to make sure you pass the same module name that you used in 
 * lcd_create_module_klcd.
 */
void lcd_destroy_module_klcd(cptr_t klcd, char *mname);

#endif /* LCD_DOMAINS_CREATE_H */
