
/* COMPILER: These are generated for every trampoline in the callee
 * code. */
 
SECTIONS
{
	.new_file_trampoline.text : {
	    *(.new_file_trampoline.text)
	    new_file_trampoline_end = .;
	}
	.rm_file_trampoline.text : {
	    *(.rm_file_trampoline.text)
	    rm_file_trampoline_end = .;
	}
}
INSERT AFTER .text;
