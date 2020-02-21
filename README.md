
Lightweight Virtualized Domains
===============================

Commodity operating systems execute core kernel subsystems in a single address space along with hundreds of dynamically loaded extensions and device drivers. Lack of isolation within the kernel implies that a vulnerability in any of the kernel subsystems or device drivers opens a way to mount a successful attack on the entire kernel.

Historically, isolation within the kernel remained prohibitive due to the high cost of hardware isolation primitives. Recent CPUs, however, bring a new set of mechanisms. Extended page-table (EPT) switching with VM functions and memory protection keys (MPKs) provide memory isolation and invocations across boundaries of protection domains with overheads comparable to system calls. Unfortunately, neither MPKs nor EPT switching provide architectural support for isolation of privileged ring 0 kernel code, i.e., control of privileged instructions and well-defined entry points to securely restore state of the system on transition between isolated domains.

LVDs develop a collection of techniques for lightweight isolation of privileged kernel code. To control execution of privileged instructions, we rely on a minimal hypervisor that transparently deprivileges the system into a non-root VT-x guest. We develop a new isolation boundary that leverages extended page table (EPT) switching with the VMFUNC instruction. We define a set of invariants that allows us to isolate kernel components in the face of an intricate execution model of the kernel, e.g., provide isolation of preemptable, concurrent interrupt handlers. To minimize overheads of virtualization, we develop support for exitless interrupt delivery across isolated domains.

More information about LVDs can be found at https://mars-research.github.io/lvds/

# Publications

Vikram Narayanan, Yongzhe Huang, Gang Tan, Trent Jaeger, and Anton Burtsev. **Lightweight Kernel Isolation with Virtualization and VM Functions**. In _16th ACM SIGPLAN/SIGOPS International Conference on Virtual Execution Environments (VEE 20)_, March 2020. [pdf](https://mars-research.github.io/doc/lvds-vee20.pdf)

# Code

LVDs are available as two git repositories:

The hypervisor: [lvd-bareflank](https://github.com/mars-research/lvd-bflank)

LVDs-compatible Linux kernel: [lvd-linux](https://github.com/mars-research/lvd-linux)

