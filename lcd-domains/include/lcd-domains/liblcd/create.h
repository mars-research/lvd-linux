/*
 * create.h
 *
 * Functions for creating and configuring LCDs.
 *
 *    - lcd_create - creates a bare LCD
 *
 *    - lcd_config_registers - set the LCD's %rip, %rsp, etc.
 *
 *    - lcd_page_grant_and_map - configure LCD's guest physical address
 *                               space; set up LCD's guest physical address 
 *                               space
 *
 *    - lcd_cap_grant - configure LCD's initial cspace; 
 *                      grant LCD a capability (e.g. to a sync endpoint)
 *
 *    - to_boot_info  - get bootstrap memory for LCD; you can store
 *                      data inside this memory, and the LCD will be able
 *                      to access it when it boots; see boot.h
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_CREATE_H
#define LCD_DOMAINS_CREATE_H

#include <linux/list.h>
#include <libcap.h>
#include <lcd-domains/types.h>


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
 * lcd_page_grant_and_map -- Grant LCD access to pages, and map it in its
 *                           *guest physical* address space
 * @lcd: the LCD to configure
 * @page: cptr to pages capability in *caller* cspace
 * @dest_slot: slot in LCD's cspace where pages capability copied to
 * @base: starting guest physical address where chunk of pages mapped
 *
 * This function combines two operations because we want to enforce the
 * invariant that an LCD has a page mapped in its address space only if
 * it has a capability to that page. (Of course, it can have capabilities
 * to pages that aren't mapped.)
 *
 * The caller is responsible for knowing what slots are free in the LCD's
 * cspace.
 *
 * If the chunk of pages referenced by the pages capability does not fit
 * in the LCD's guest physical address space, starting at @base, then
 * this function fails.
 */
int lcd_page_grant_and_map(cptr_t lcd, cptr_t page, cptr_t dest_slot,
				   gpa_t base);

/**
 * lcd_cap_grant -- Grant the LCD a capability (config its cspace)
 * @lcd: the LCD to configure
 * @src: cptr to capability in the *caller* cspace to grant
 * @dest: slot in LCD's cspace where capability should be stored
 */
int lcd_cap_grant(cptr_t lcd, cptr_t src, cptr_t dest);

/**
 * lcd_run -- Run an LCD
 * @lcd: the LCD to run
 */
int lcd_run(cptr_t lcd);

/* HIGHER-LEVEL LCD CREATE FROM KERNEL MODULE ------------------------------ */

struct lcd_boot_info;

/**
 * This context is used during LCD creation / destruction.
 */
struct lcd_create_ctx {
	/*
	 * Module name
	 */
	char mname[LCD_MODULE_NAME_MAX];
	/*
	 * Where to point the program counter to run the module's init
	 */
	gva_t init;
	/*
	 * List of the module's pages
	 */
	struct list_head mpages_list;
	/*
	 * Temporary cptr cache for properly setting up the LCD's cspace
	 */
	struct cptr_cache *cache;
	/*
	 * Pointer to beginning of boot pages
	 */
	char *boot_page_base;
	/*
	 * Boot mem page infos
	 */
	struct list_head boot_mem_list;
	/*
	 * Stack page infos
	 */
	struct list_head stack_mem_list;
	/*
	 * Paging mem page infos
	 */
	struct list_head paging_mem_list;
	/*
	 * Free mem page infos
	 */
	struct list_head free_mem_list;
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
 * to_boot_info -- Extract lcd_boot_info from create context
 * @ctx: the lcd create ctx returned from lcd_create_module_lcd
 *
 * Use this to pull out the struct. Then you can store your own values
 * in the fields for dumping the info.
 */
static inline struct lcd_boot_info * to_boot_info(struct lcd_create_ctx *ctx)
{
	return (struct lcd_boot_info *)ctx->boot_page_base;
}

/**
 * lcd_dump_boot_info -- Dump information in lcd_create_ctx into bootstrap
 * @ctx: the lcd create context returned from lcd_create_module_lcd
 *
 * This will dump information the LCD needs in order to bootstrap itself -
 * cptrs for capabilities in its cspace, the initial cptr cache, and so on.
 *
 * You should call this before running the LCD.
 */
int lcd_dump_boot_info(struct lcd_create_ctx *ctx);

/**
 * lcd_destroy_module_lcd -- Tear down an LCD that had a kernel module loaded
 * @lcd: cptr to the lcd capability (the LCD)
 * @ctx: the lcd_create_ctx used during lcd create (returned from
 *       lcd_create_module_lcd)
 *
 * This will also free up the @ctx.
 */
void lcd_destroy_module_lcd(cptr_t lcd, struct lcd_create_ctx *ctx);

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
