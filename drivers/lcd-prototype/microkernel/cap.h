


/*
 * Look up the cnode at c in cspace. No locking required by caller.
 * Returns non-zero on error.
 */
int lcd_cnode_lookup(struct cspace *cspace, cptr_t c, struct cnode **out);

/*
 * Confirms src_cptr and dest_cptr point at valid slots. Confirms src
 * has grant rights to cap at src_cptr. Transfers cap to dest_cptr, updates
 * cdt. No locking required by caller. Returns non-zero on error.
 */
int lcd_cnode_grant(struct cspace *src, struct cspace *dest,
		cptr_t src_cptr, cptr_t dest_cptr, unsigned int rights);

