/*
 * module_create_klcd.c
 *
 * Load a kernel module and run it using
 * a dedicated thread. The dedicated thread
 * will run in "LCD mode".
 */

#include <liblcd/liblcd.h>
#include <lcd_domains/microkernel.h>
#include <lcd_domains/kliblcd.h>


struct module *lvd_create_module_klcd_no_thread(char *mdir, char *mname, cptr_t *klcd_out)
{
	struct module *m = NULL;
	struct lcd *lcd_struct;
	struct cnode *cnode;
	int ret;

	if (!lvd_create_module_klcd(mdir, mname, klcd_out)) {
		mutex_lock(&module_mutex);
		m = find_module(mname);
		mutex_unlock(&module_mutex);
	}

	ret = __lcd_get(current->lcd, *klcd_out, &cnode, &lcd_struct);
	if (ret)
		goto fail1;

	switch (lcd_struct->type) {
	case LCD_TYPE_NONISOLATED:
	case LCD_TYPE_ISOLATED:
	case LCD_TYPE_TOP:
		lcd_struct->m = m;
	}

	__lcd_put(current->lcd, cnode, lcd_struct);
fail1:
	return m;
}

int lcd_create_module_klcd(char *mdir, char *mname, cptr_t *klcd_out)
{
	cptr_t klcd;
	int ret;
	struct module *m;

	/*
	 * Load kernel module in host
	 */
	ret = __kliblcd_module_host_load(mdir, mname, &m);
	if (ret) {
		LIBLCD_ERR("module load failed");
		goto fail1;
	}
	/*
	 * Create "empty" kLCD
	 */
	ret = lcd_create_klcd(&klcd);
	if (ret) {
		LIBLCD_ERR("klcd create failed");
		goto fail2;
	}
	/*
	 * Configure kLCD thread to run the module's init
	 *
	 * We pass null values for most arguments, as these aren't
	 * used for kLCDs.
	 */
	ret = lcd_config_registers(klcd,
				__gva(hva_val(va2hva(m->init))),
				__gva(0),
				__gpa(0),
				__gpa(0));
	if (ret) {
		LCD_ERR("failed to config klcd");
		goto fail3;
	}

	*klcd_out = klcd;

	return 0;

fail3:
	lcd_cap_delete(klcd);
fail2:
	__kliblcd_module_host_unload(mname);
fail1:
	return ret;
}

int lvd_create_module_klcd(char *mdir, char *mname, cptr_t *klcd_out)
{
	cptr_t klcd;
	int ret;
	struct module *m;

	/*
	 * Load kernel module in host
	 */
	ret = __kliblcd_module_host_load(mdir, mname, &m);
	if (ret) {
		LIBLCD_ERR("module load failed");
		goto fail1;
	}
	/*
	 * Create "empty" kLCD
	 */
	ret = lvd_create_klcd(&klcd);
	if (ret) {
		LIBLCD_ERR("klcd create failed");
		goto fail2;
	}
	/*
	 * Configure kLCD thread to run the module's init
	 *
	 * We pass null values for most arguments, as these aren't
	 * used for kLCDs.
	 */
	ret = lcd_config_registers(klcd,
				__gva(hva_val(va2hva(m->init))), 
				__gva(0),
				__gpa(0),
				__gpa(0));
	if (ret) {
		LCD_ERR("failed to config klcd");
		goto fail3;
	}

	*klcd_out = klcd;
	
	return 0;

fail3:
	lcd_cap_delete(klcd);
fail2:
	__kliblcd_module_host_unload(mname);
fail1:
	return ret;
}

void lcd_destroy_module_klcd(cptr_t klcd, char *mname)
{
	/*
	 * We *must* delete the klcd first before unloading the module.
	 * Otherwise, if the klcd is still running, it will use the freed
	 * module pages.
	 */
	lcd_cap_delete(klcd);
	__kliblcd_module_host_unload(mname);
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_create_module_klcd);
EXPORT_SYMBOL(lvd_create_module_klcd);
EXPORT_SYMBOL(lcd_destroy_module_klcd);
EXPORT_SYMBOL(lvd_create_module_klcd_no_thread);
