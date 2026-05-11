# BULKHEAD vs Linux v6.1 — Change Summary

This directory contains a side-by-side analysis of the **BULKHEAD** research kernel
tree (`/Volumes/git/BULKHEAD`) against vanilla **Linux v6.1** (this checkout at
`/Volumes/git/lvd-6.8`, currently at tag `v6.1` / commit `830b3c68c1fb`).

BULKHEAD is a flat, history-less source dump (the upstream repo declares it as
*"BULKHEAD: Secure, Scalable, and Efficient Kernel Compartmentalization with
PKS"*). It builds **kernel compartmentalization** on top of Intel **Protection
Keys for Supervisor (PKS)**, exemplified by isolating subsystems such as IPv6,
netfilter, and dm-zero.

## Files in this directory

| File | Purpose |
|------|---------|
| `bulkhead.patch` | Full unified diff (BULKHEAD vs vanilla v6.1), build artifacts excluded. ~33k lines, 155 files. |
| `diffstat.txt` | `diffstat` output for the patch (per-file +/- counts). |
| `README.md` | This file. |

### Reproducing the patch

```bash
diff -urN \
  --exclude=.git --exclude='*.o' --exclude='*.cmd' --exclude='*.bc' \
  --exclude='*.ko' --exclude='*.symvers' --exclude='*.lst' \
  --exclude='built-in.a' --exclude='*.a' --exclude='*.mod' --exclude='*.mod.c' \
  --exclude='*.o.d' --exclude='.*.d' --exclude='*.d' \
  --exclude='modules.order' --exclude='modules.builtin*' \
  --exclude='vmlinux*' --exclude='System.map' --exclude='.tmp_*' \
  --exclude='*.bin' --exclude='*.elf' --exclude='*.gz' --exclude='*.so*' \
  --exclude='*.pem' --exclude='*.x509' --exclude='*.genkey' \
  --exclude='*.asn1.c' --exclude='*.asn1.h' \
  --exclude='lex.lex.c' --exclude='parse.tab.*' --exclude='parser.tab.*' \
  --exclude='generated' --exclude='config' \
  --exclude='kcpuid' --exclude='objtool' --exclude='fixdep' \
  --exclude='*.config' --exclude='*.s' \
  --exclude='.clang-format' --exclude='.cocciconfig' --exclude='.gitignore' \
  --exclude='.gitattributes' --exclude='.mailmap' --exclude='.rustfmt.toml' \
  --exclude='.get_maintainer.ignore' --exclude='CLAUDE.md' \
  --exclude='push.sh' --exclude='patches' --exclude='bulkhead-analysis' \
  --exclude='Kconfig.lvds' \
  /Volumes/git/lvd-6.8 /Volumes/git/BULKHEAD > bulkhead.patch
```

Notes on what is intentionally excluded:
* All build artifacts in BULKHEAD (it ships with `built-in.a`, `*.o`, dep files, etc.).
* Generated files (asn1 parsers, lex/yacc output, signing keys, vDSO blobs).
* Dotfiles and tooling specific to the local lvd-6.8 checkout (`CLAUDE.md`,
  `Kconfig.lvds`, `patches/`, `push.sh`).

## Tree fingerprint

* **Vanilla:** Linux 6.1.0, tag `v6.1`, commit `830b3c68c1fb`.
* **BULKHEAD:** `Makefile` declares `VERSION=6 PATCHLEVEL=1`. README adds a
  "BULKHEAD" headline. No git history available.

## Patch shape

```
155 files changed, 34668 insertions(+), 174 deletions(-)
```

Breakdown by area:

| Area | Files | Notable |
|------|-------|---------|
| PKS core (asm + mm/pkeys.c + headers) | ~15 | New `pks.h`, `pks-keys.h`, `pkeys_common.h` |
| x86 entry / boot / asm | ~12 | pt_regs auxiliary, PKRU push/pop, head_64 PKE init |
| Memory management (slab, pgtable, set_memory) | ~20 | `kmalloc_pks`, `set_memory_pks`, grouped page cache |
| New test/demo modules | 6 | `efi_mem_test`, `pks_monitor`, `wrpkrs_test` (all under `arch/x86/mm/`) |
| New switch-gate infra | 3 | `include/linux/sgtable.h`, `arch/x86/mm/sgtable.c`, module loader hook |
| net/ipv6 PKS consumer | ~92 | Every IPv6 file references `ipv6_entry_gid`/`ipv6_exit_gid`; new `CONFIG_PKS_IPV6` |
| device-mapper PKS consumer | 3 | New `CONFIG_PKS_DM_ZERO`; dm-zero/dm-target instrumentation |
| KVM | 2 | PKRU saved into `current_pt_regs()->pkru`; experimental EPTP-list code in vmx.c |
| Documentation / Kconfig / build | 5 | Expanded `protection-keys.rst`, new Kconfigs, clang static-analysis pass loader |
| Config snapshots | 3 | `config-allmod.txt`, `config-cycles.txt`, `config-lkm.txt` (saved `.config`s) |
| ARM64 (incidental) | 2 | Rename `irqentry_exit_cond_resched` → `*_internal` to mirror x86 hook |

## Architectural overview

BULKHEAD is a complete top-to-bottom PKS isolation stack. The pieces fit
together as follows:

```
                      +-----------------------------+
   per-thread PKRS -->|       struct pt_regs        |
                      | (with pt_regs_extended.aux) |
                      +-------------+---------------+
                                    |
                  save/restore on   |   ctx switch via
                  exception entry   |   x86_pkrs_load(next)
                                    v
   +---------+   +-------------------------------+   +----------+
   |   CPU   |<--+   MSR_IA32_PKRS / CR4.PKS     +-->| init_mm  |
   |         |   +-------------------------------+   | PTEs w/  |
   +---------+                                       | pkey bits|
                                                     +----------+
                          ^
                          |  pks_set_*, pks_update_*
                          |  pks_handle_key_fault()
                          |
                +-------------------+    +-------------------+
                | core PKS API      |    | grouped page cache|
                | (include/linux/   |    | (read-only PT     |
                |  pks.h, pks-keys) |    |  pool + shrinker) |
                +---------+---------+    +---------+---------+
                          ^                        ^
                          |                        |
                +---------+--------+    +----------+----------+
                | PKS consumers    |    | PKS-protected slab  |
                | * IPv6           |    | (kmalloc_pks)       |
                | * netfilter      |    | * tracked per-pkey  |
                | * dm-zero        |    | * set_memory_pks on |
                | * loadable mods  |    |   each new slab     |
                |   (PKS_LKM20)    |    +---------------------+
                +------------------+
                          |
                          v
                +-----------------------------+
                | switch_gate_table (sgtable) |
                | per-module call/return gate |
                | pks_switch(), pks_module_   |
                |    setup() at insmod time   |
                +-----------------------------+
```

## 1. Core PKS infrastructure

### Hardware abstraction
* `MSR_IA32_PKRS` (MSR `0x6E1`) added in `arch/x86/include/asm/msr-index.h`.
* `X86_FEATURE_PKS` added in `arch/x86/include/asm/cpufeatures.h:225`.
* `X86_CR4_PKS` (CR4 bit 24) added in `arch/x86/include/uapi/asm/processor-flags.h`.
* `pks_setup()` called from `identify_cpu()` (`arch/x86/kernel/cpu/common.c:1864`)
  writes `PKS_INIT_VALUE` and sets `CR4.PKS`.

### Keys & groups
* `arch/x86/include/asm/pkeys_common.h` — defines `PKS_NUM_PKEYS = 16` and the
  AD/WD bit layout per key.
* `include/linux/pks-keys.h` — defines well-known keys
  (`PKS_KEY_DEFAULT = 0`, `PKS_KEY_MONITOR = 1`) plus `PKS_INIT_VALUE` and the
  helper `PKEY2PKRS(pkey)` macro.

### Public API (`include/linux/pks.h`)
| Function | Purpose |
|----------|---------|
| `pks_available()` | runtime gate on `X86_FEATURE_PKS` |
| `pks_set_readwrite()` / `pks_set_noaccess()` / `pks_set_nowrite()` | adjust the current thread's PKRS for a given key |
| `pks_update_protection()` / `pks_update_exception()` | bulk update PKRS (the latter operates on `regs->aux.pkrs`) |
| `pks_key_callback` (function pointer) | per-key fault callback registered through `pks_key_callbacks[]` |

### Save / restore plumbing
* New `struct pt_regs_auxiliary { u32 pkrs; }` and
  `struct pt_regs_extended { aux; pt_regs }` in `arch/x86/include/asm/ptrace.h`.
* `arch/x86/entry/calling.h` adds `PUSH_PTREGS_AUXILIARY` / `POP_PTREGS_AUXILIARY`
  macros wrapping the existing register-push/pop sequences. Under `CONFIG_PKK`
  the macros also `rdpkru` / `wrpkru` to handle user PKRU.
* `arch/x86/entry/entry_64.S` and `entry_64_compat.S` insert the auxiliary
  push/pop around every C handler call (idtentry, NMI, SYSENTER/SYSCALL, etc.).
* `kernel/entry/common.c` calls `arch_save_aux_pt_regs()` /
  `arch_restore_aux_pt_regs()` (which delegate to `pks_save_pt_regs()` /
  `pks_restore_pt_regs()` in `arch/x86/mm/pkeys.c:410-442`) on irqentry
  enter/exit so handlers run with the *default* PKRS rather than the
  interrupted thread's value.
* `arch/x86/kernel/process_64.c:616` calls `x86_pkrs_load(next)` during
  `__switch_to`, restoring PKRS from `next->thread.pkrs`
  (`arch/x86/include/asm/processor.h:531-540`).

### Page-table / fault integration
* PTE flag bits `_PAGE_PKEY_BIT0..3` are repurposed as a 4-bit key index, with
  helpers `_PAGE_PKEY(pkey)`, `PAGE_KERNEL_PKEY(pkey)`, and
  `__PAGE_KERNEL_XOM` in `arch/x86/include/asm/pgtable_types.h:67-96`.
* `arch/x86/mm/fault.c:1179-1191` no longer WARNs on a kernel-mode `X86_PF_PK`;
  it dispatches to `pks_handle_key_fault()` (`arch/x86/mm/pkeys.c:289-356`)
  which walks `init_mm`, extracts the offending pkey via
  `pte_flags_pkey(pte_val(pte))`, and invokes the registered callback.
* PKRS writes are routed through a per-CPU cache (`pkrs_cache`,
  `previous_pkrs`) in `arch/x86/mm/pkeys.c:378-391` to avoid redundant MSR
  writes.

### Boot
* `arch/x86/kernel/head_64.S` initializes `PKRU=0` and sets `CR4.PKE` on both
  the boot path (`.Lon_kernel_cs`) and secondary CPUs; the early exception
  handler (`do_early_exception`) gets the same auxiliary push/pop.
* `arch/x86/boot/compressed/head_64.S` adds an `adjust_pku()` helper used
  before/after relocation; `ident_map_64.c` provides no-op
  `enable_pgtable_write()` / `disable_pgtable_write()` for the decompressor.

## 2. Memory management changes (beyond core PKS)

### PKS-protected slab (`CONFIG_PKS_HEAP`)
* `mm/slab_common.c:702-737` adds a parallel
  `kmalloc_pks_caches[NR_KMALLOC_TYPES][KMALLOC_SHIFT_HIGH+1]` array.
* `mm/slub.c:3428+` adds parallel slow-paths
  (`allocate_slab_pks`, `new_slab_pks`, `___slab_alloc_pks`) that invoke
  `set_memory_pks(addr, 1<<order, 0)` on every freshly-allocated slab folio.
* New API in `include/linux/slab.h:450-453`:
  `__kmalloc_pks`, `kmem_cache_alloc_pks`, `kmem_cache_zalloc_pks`,
  plus the user-facing `kmalloc_pks()` wrappers (lines 571-591).

### PKS-protected page tables (`CONFIG_PKS_MONITOR`)
* New generic `alloc_table()` / `free_table()` / `alloc_table_node()` stubs
  in `include/asm-generic/pgalloc.h:10-23`.
* `arch/x86/mm/pgtable.c:44-86` routes `pte_alloc_one`, `pmd_alloc_one`,
  `pud_alloc_one`, `p4d_alloc_one`, and `_pgd_alloc()` (lines 481-487) through
  a `gpc_pks` "grouped page cache" once `pks_tables_inited()` is true.
* `enable_pgtable_write()` / `disable_pgtable_write()` guards wrap every
  page-table mutation site (e.g. `pte_young`, `pmd_young`, EFI page-table
  copies in `arch/x86/platform/efi/efi_64.c:119-152`).

### `set_memory_pks()` and the grouped page cache
* `arch/x86/mm/pat/set_memory.c:2084+` exports
  `int set_memory_pks(unsigned long addr, int numpages, int key)` built on
  the existing CPA path.
* `arch/x86/mm/pat/set_memory.c:1184-1198` makes `__split_large_page()` use
  `alloc_dmap_table()` / `free_dmap_table()` so that the page tables created
  during CPA splits are themselves PKS-protected.
* `arch/x86/include/asm/set_memory.h:92-111` defines
  `struct grouped_page_cache { shrinker; list_lru; gfp; ... }`, with API
  `init_grouped_page_cache()`, `get_grouped_page[_atomic]()`,
  `free_grouped_page()`. Backed by `mm/list_lru.c` enhancements
  (`list_lru_add_node`, `list_lru_get_mru` —
  `include/linux/list_lru.h:103-116`).

### Switch-gate table (sgtable, NEW)
* `include/linux/sgtable.h` defines:
  * `struct switch_gate_entry { u32 source_pkrs, target_pkrs; ... }`
  * `struct switch_gate_table { entries[]; cursor; lock; }`
  * `pks_switch(bool enter, gate_id)`, `register_switch_gate()`,
    `lookup_switch_gate()`, `init_switch_gate_table()`,
    `pks_module_setup()`, `init_private_stacks()`.
* `arch/x86/mm/sgtable.c` implements the table (initialized with
  `set_memory_pks(..., PKS_KEY_MONITOR)`), exports per-consumer GIDs
  (`ipv6_entry_gid`, `ipv6_exit_gid`, `dm_zero_entry_gid`, `dm_zero_exit_gid`,
  …), and contains `pks_module_setup()` (lines 399-545) which under
  `CONFIG_ASID_SWITCH4PKS` allocates a fresh PGD per module to combine PKS
  with PCID-style ASID separation.
* `kernel/module/main.c:2527+` calls `pks_module_setup(mod)` after each
  module is live (gated on `CONFIG_PKS_LKM20` or specific module names:
  `ipv6`, `nf_tables`, `dm_zero`).

### Misc mm helpers
* `mm/swap.c:1010-1015` routes `PageTable()` folios through `free_table()`
  on release.
* `mm/Kconfig` adds the umbrella menu: `ARCH_HAS_SUPERVISOR_PKEYS`,
  `ARCH_ENABLE_SUPERVISOR_PKEYS`, `PKK`, `ISKIOS_XOM`,
  `ASID_SWITCH4PKS`, `PKS_LKM20`, `PKS_STACK`, `PKS_HEAP`, `PKS_MONITOR`,
  `PKS_MONITOR_SGT`, `PKS_HIT`.

## 3. New test / demo modules

All three live under `arch/x86/mm/` and are out-of-tree-style modules
(not wired into `arch/x86/mm/Makefile` itself).

| Module | Purpose | Notes |
|--------|---------|-------|
| `wrpkrs_test/` | Microbenchmark for `wrmsr` to `MSR_IA32_PKRS` (the `wrpkrs`-equivalent). | 1000-iter `rdtsc`+`cpuid` loop, prints cycles to dmesg. |
| `pks_monitor/` | Placeholder for PKS table-init monitor. | Body is currently a stub; commented references to `pks_tables_init`, `init_pks_dmap_tables`, `init_private_stacks` (the `CONFIG_PKS_STACK` branch). |
| `efi_mem_test/` | Functional test of EFI/OVMF memory attributes. | Walks PTE for hard-coded address `0x3FAE9000` via `lookup_address()`, ioremaps, attempts a volatile write, prints the PTE. |

All three are authored by `Yinggang` per `MODULE_AUTHOR`.

## 4. PKS consumers

### IPv6 (`CONFIG_PKS_IPV6`)
* New Kconfig: `net/ipv6/Kconfig:343-347`. Selects `ARCH_ENABLE_PKS_CONSUMER`.
* `include/net/ipv6.h:17` now `#include <linux/sgtable.h>`.
* Roughly 92 of 105 IPv6 `.c` files are touched; the canonical edit (e.g.
  `net/ipv6/af_inet6.c:1080`, `tcp_ipv6.c:2237`, `udp.c:1788`,
  `route.c:6689`, `ip6_fib.c:2451`) inserts at module-init time:
  ```c
  unsigned long ii = ipv6_entry_gid + ipv6_exit_gid;
  // pr_info("ipv6_entry_gid: %lu\n", ipv6_entry_gid);
  // pr_info("ipv6_exit_gid: %lu\n", ipv6_exit_gid);
  ```
  This is a *static instrumentation* — it only forces the linker to keep the
  GID symbols visible. The `pr_info` calls are commented out.
* Real PKS use is conditional and confined to two spots:
  * `net/ipv6/route.c:648,657,6595` swaps `kmalloc(...)` for
    `kmalloc_pks(...)` under `CONFIG_PKS_HEAP`.
  * `net/ipv6/addrconf.c:4068-4234` switches RSP to an isolated stack and
    writes `MSR_IA32_PKRS` directly inside `addrconf_dad_work()` under
    `CONFIG_PKS_STACK`.

### Netfilter
* `net/netfilter/Kconfig:487` selects `ARCH_ENABLE_PKS_CONSUMER`.

### device-mapper (`CONFIG_PKS_DM_ZERO`)
* `drivers/md/Kconfig` adds the new option.
* `drivers/md/dm-zero.c` references `dm_zero_entry_gid` /
  `dm_zero_exit_gid` and probes `X86_FEATURE_PCID` / `CR4.PCIDE` /
  `X86_FEATURE_LA57` at init.
* `drivers/md/dm-target.c` carries commented scaffolding for allocating a
  fresh `pgd_t` and copying from `swapper_pg_dir` — i.e. preparation for
  per-target PGD isolation that is not yet wired up.

### KVM
* `arch/x86/kvm/x86.c` modifies `kvm_load_guest_xsave_state()` /
  `kvm_read_guest_xsave_state()` so that under `CONFIG_PKK` the saved guest
  PKRU is parked in `current_pt_regs()->pkru` instead of being written
  directly with `wrpkru` — i.e. the supervisor PKS layer mediates the write.
* `arch/x86/kvm/vmx/vmx.c` contains *unrelated* experimental code: an EPTP
  list (`EPTP_LIST_ADDRESS`) is allocated and populated in
  `vmx_set_cr3()` / `vmx_setup_initial_vmcs()` with `pr_info` debug traces.
  This looks like prototype work for VMFUNC-based EPT switching and is not
  obviously connected to the PKS path.

## 5. Scheduler / preempt rename (incidental)

`kernel/sched/core.c` renames `irqentry_exit_cond_resched` →
`irqentry_exit_cond_resched_internal` across all preempt modes. The same
rename appears in `arch/arm64/include/asm/preempt.h` and
`arch/arm64/kernel/entry-common.c`. The intent (consistent with the PKS
work) is to leave room for an external wrapper that restores supervisor
PKS context before reschedule.

## 6. Documentation / build

* `Documentation/core-api/protection-keys.rst` is roughly doubled in size:
  it now distinguishes PKU vs PKS, lists hardware support (Sapphire Rapids,
  Alder Lake, QEMU), documents `MSR_IA32_PKRS` ordering, and adds a "Kernel
  API for PKS Support" section pointing at `pks_set_*` and the
  `ARCH_ENABLE_PKS_CONSUMER` pattern.
* `arch/x86/Kconfig` makes `X86_INTEL_MEMORY_PROTECTION_KEYS` select
  `ARCH_HAS_SUPERVISOR_PKEYS` and exposes
  `CONFIG_ARCH_HAS_PTREGS_AUXILIARY` (auto-enabled for x86_64 +
  `ARCH_ENABLE_SUPERVISOR_PKEYS`).
* `tools/arch/x86/include/asm/disabled-features.h` mirrors a
  `DISABLE_PKS_MONITOR` knob into `DISABLED_MASK7`.
* Top-level `Makefile` adds (commented-leaning) clang plumbing for a custom
  static-analysis pass: `-Xclang -load -Xclang
  /home/yg/project/pks-static-analysis/instrument/build/lib/libInstrumentTest.so`
  — i.e. the BULKHEAD project ships an external LLVM pass that the kernel
  build is wired to load.
* `config-allmod.txt`, `config-cycles.txt`, `config-lkm.txt` are saved
  `.config` snapshots. `config-cycles.txt` enables
  `CONFIG_ARCH_ENABLE_PKS_CONSUMER=y` and
  `CONFIG_ARCH_ENABLE_SUPERVISOR_PKEYS=y` while leaving `CONFIG_PKK`,
  `CONFIG_PKS_MONITOR_SGT`, `CONFIG_PKS_DM_ZERO` unset — i.e. it is the
  config used to measure `wrpkrs` cycles.

## Caveats & observations

1. **Static instrumentation, not runtime enforcement** — most of the IPv6
   change set is "touch every file so the GID symbols are referenced". No
   actual `wrpkrs` is issued by IPv6 module init. The interesting runtime
   PKS consumers are the conditional `kmalloc_pks` / `addrconf_dad_work`
   stack switch.
2. **Several Kconfigs are clearly research-grade** (`PKS_LKM20`, `PKS_HIT`,
   `ASID_SWITCH4PKS`, `ISKIOS_XOM`, `PKK`). Naming is inconsistent (`PKK` vs
   `PKS_…`).
3. **VMX EPTP-list edits** in `arch/x86/kvm/vmx/vmx.c` look like an
   independent VMFUNC experiment that was committed alongside the PKS work.
4. **Build system depends on an absolute host path** (`/home/yg/project/...`)
   for the clang static-analysis pass; this won't build on any other host
   without editing `Makefile`.
5. **Diff timestamps** in the patch reflect filesystem timestamps from this
   working copy (May 2026), not BULKHEAD's authorship dates.
6. The patch does *not* convert into `git apply`-able form because BULKHEAD
   has no git history. Apply it with plain `patch -p1` from a v6.1 worktree:
   ```bash
   cd <fresh v6.1 worktree>
   patch -p1 < bulkhead.patch
   ```
   (The leading paths in the diff are absolute, so `patch -p4` against a
   directory created at `/Volumes/git/BULKHEAD` would also work; with `-p1`
   you'll be prompted for the file to patch, or run with
   `--directory=$PWD --strip=4`.)
