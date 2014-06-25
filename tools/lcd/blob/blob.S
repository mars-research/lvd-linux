#
# IMPORTANT: A yield hypercall needs to terminate the code.
#            Otherwise, the hypervisor will happily
#            continue executing into neighboring
#            memory.
#
.text
	# yield
	movq $6, %rax
	vmcall
