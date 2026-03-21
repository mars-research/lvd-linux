# LCD Domains

## Quick Start

To build see: https://github.com/mars-research/lvd-linux/wiki

If you want to add a new LCD you have to edit `scripts/Kbuild.test_mods`

---

# Build System Architecture

This document explains the build system architecture for LCD (Lightweight Capability Domains), particularly the configuration hook system and dual-build environment.

## Overview

The LCD domains system consists of two execution environments:

1. **Isolated LCDs** - Lightweight domains with restricted kernel functionality
2. **Non-Isolated Kernel** - Host kernel and microkernel with full kernel APIs

Code in this repository can be compiled for either environment, and some code (in `common/`) is compiled for **both**.

## Configuration Hook System

### The Hook Files

The system uses "configuration hooks" to allow the same source code to compile differently based on the target environment:

```
lcd-domains/
├── config/
│   ├── isolated/
│   │   └── lcd_config/
│   │       ├── pre_hook.h    # Definitions for isolated environment
│   │       └── post_hook.h   # Cleanup for isolated environment
│   └── non_isolated/
│       └── lcd_config/
│           ├── pre_hook.h    # Empty placeholder
│           └── post_hook.h   # Empty placeholder
```

### How Hooks Work

Source files that need environment-specific behavior include these hooks around their header includes:

```c
#include <lcd_config/pre_hook.h>

#include <linux/slab.h>
#include <linux/module.h>
// ... other headers ...

#include <lcd_config/post_hook.h>

// Implementation code follows
```

**The build system selects which hooks to use via the include path:**

- **Isolated builds**: `-I$(LCD_DOMAINS_TOP_SRC_DIR)/config/isolated`
- **Non-isolated builds**: `-I$(LCD_DOMAINS_TOP_SRC_DIR)/config/non_isolated`

The preprocessor resolves `<lcd_config/pre_hook.h>` based on the include path, loading the appropriate version for that build.

### Why Use This Pattern?

This approach provides:

1. **Clean source code** - No `#ifdef` clutter around hook includes
2. **Build-time polymorphism** - Configuration selected by Makefile, not source code
3. **Separation of concerns** - Build system handles configuration, source stays clean
4. **Null object pattern** - Non-isolated gets "do nothing" hooks that maintain the interface

## Build Targets and Configurations

### 1. liblcd (Isolated Environment)

**Build output**: `liblcd_build/liblcd.a`

**Configuration**: `ISOLATED_CFLAGS`
- Include path: `config/isolated`
- Defines: `-DLCD_ISOLATE -DMODULE -DOTHER_DOMAIN=0`

**What gets compiled**:
- All of `liblcd/` (kernel utility functions for isolated LCDs)
  - `liblcd/lib/` - String, memory, data structure utilities
  - `liblcd/mm/` - Memory management (slab allocator, etc.)
  - `liblcd/net/` - Network stack helpers
  - `liblcd/block/` - Block device helpers
  - `liblcd/locking/` - Synchronization primitives
- Shared code from `common/` (first compilation)

**Makefile**: `scripts/Makefile.liblcd`

**Kbuild**: `scripts/Kbuild.liblcd`

### 2. Microkernel (Non-Isolated Environment)

**Build output**: `microkernel_build/lcd_domains.ko`

**Configuration**: `NONISOLATED_CFLAGS` (aliased as `MICROKERNEL_CFLAGS`)
- Include path: `config/non_isolated`
- Defines: `-DLINUX_KERNEL_MODULE -DOTHER_DOMAIN=1`

**What gets compiled**:
- `microkernel/` - Core LCD management in host kernel
  - `microkernel/create.o` - LCD creation
  - `microkernel/mem.o` - Memory management
  - `microkernel/vmfunc*.o` - VMFUNC-based IPC
- `kliblcd/` - Kernel-side LCD library functions
- Shared code from `common/` (second compilation)
- Libraries: libcap, libfipc, libasync

**Makefile**: `scripts/Makefile.microkernel`

**Kbuild**: `scripts/Kbuild.microkernel`

### 3. Shared Code (common/)

Files in the `common/` directory are **compiled twice** with different configurations:

**Files in common/**:
- `resource_tree.c` - Address to capability translation
- `module_create.c` - LCD module loading
- `module_create_lvd.c` - LVD-specific module creation
- `sync_ipc_poll.c` - Synchronous IPC polling
- `vmfunc_calls.c` - VMFUNC call wrappers

**Which files use hooks**:
- `module_create.c` - Uses pre/post hooks
- `resource_tree.c` - Uses pre/post hooks
- `sync_ipc_poll.c` - Uses pre/post hooks
- `vmfunc_calls.c` - Uses pre/post hooks
- `module_create_lvd.c` - Uses pre/post hooks

**Build process**:

1. **First compilation** (for liblcd.a):
   - Uses `ISOLATED_CFLAGS`
   - Include path: `config/isolated`
   - Gets actual hook definitions
   - Linked into `liblcd.a`

2. **Second compilation** (for lcd_domains.ko):
   - Uses `NONISOLATED_CFLAGS`
   - Include path: `config/non_isolated`
   - Gets empty hook placeholders
   - Linked into `lcd_domains.ko`

This is why `config/non_isolated` exists - it's used when compiling `common/` files for the microkernel.

## Test Modules

Test modules in `test_mods/` demonstrate the dual-environment architecture:

**Typical structure**:
```
test_mods/nullnet_gen/
├── boot/           # Boot/initialization (NONISOLATED_CFLAGS)
├── net_klcd/       # Kernel-side glue code (NONISOLATED_CFLAGS)
└── dummy_lcd/      # LCD-side driver code (ISOLATED_CFLAGS)
```

**Build variables** (from `scripts/Makefile.test_mods`):
```makefile
export NONISOLATED_CFLAGS=$(MICROKERNEL_CFLAGS)
export ISOLATED_CFLAGS=$(LIBLCD_CFLAGS)
```

Each Kbuild file specifies which configuration to use:
```makefile
# In dummy_lcd/Kbuild
ccflags-y += $(ISOLATED_CFLAGS)

# In net_klcd/Kbuild
ccflags-y += $(NONISOLATED_CFLAGS)
```

## Conditional Compilation

Beyond the hook system, code uses `#ifndef LCD_ISOLATE` for environment-specific logic:

```c
#ifndef LCD_ISOLATE
    // Non-isolated code path (full kernel functionality)
    mempool_free(p, bs->bio_pool);
#else
    // Isolated code path (restricted functionality)
    kfree(bio);
#endif
```

The `LCD_ISOLATE` macro is defined only in `ISOLATED_CFLAGS`, so:
- Isolated builds: Code in `#ifndef LCD_ISOLATE` is excluded
- Non-isolated builds: Code in `#ifndef LCD_ISOLATE` is included

This allows a single source file to have different implementations for different environments.

## Build Process Summary

```
┌─────────────────────────────────────────────────────────────┐
│ Source Code in liblcd/                                      │
│ - Includes <lcd_config/pre_hook.h>                          │
│ - Includes standard headers                                 │
│ - Includes <lcd_config/post_hook.h>                         │
│ - May contain #ifndef LCD_ISOLATE blocks                    │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│ Compiled with ISOLATED_CFLAGS                               │
│ - Include path: config/isolated                             │
│ - Defines: -DLCD_ISOLATE                                    │
│ - Gets real hook definitions                                │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
                    ┌───────────────┐
                    │  liblcd.a     │
                    └───────────────┘
                            │
                            ▼
                    Linked into LCD modules
                    (dummy_lcd, nvme_lcd, etc.)


┌─────────────────────────────────────────────────────────────┐
│ Source Code in common/                                      │
│ - Includes <lcd_config/pre_hook.h>                          │
│ - Includes standard headers                                 │
│ - Includes <lcd_config/post_hook.h>                         │
└─────────────────────────────────────────────────────────────┘
                            │
                ┌───────────┴────────────┐
                ▼                        ▼
    ┌──────────────────────┐  ┌──────────────────────┐
    │ ISOLATED_CFLAGS      │  │ NONISOLATED_CFLAGS   │
    │ config/isolated      │  │ config/non_isolated  │
    │ Real hooks           │  │ Empty hooks          │
    └──────────────────────┘  └──────────────────────┘
                │                        │
                ▼                        ▼
        ┌───────────────┐        ┌───────────────┐
        │  liblcd.a     │        │ lcd_domains.ko│
        └───────────────┘        └───────────────┘
```

## Key Takeaways

1. **Two build environments**: Isolated (LCDs) and Non-isolated (kernel/microkernel)

2. **Hook system**: Pre/post hooks allow environment-specific header manipulation without source code changes

3. **Include path selection**: Build system chooses configuration by setting include paths, not `#ifdef`

4. **Shared code**: `common/` files compile twice, once for each environment

5. **Non-isolated hooks exist** for compiling `common/` files into the microkernel, even though they're currently empty

6. **Dual compilation**: Same source, different behaviors based on build-time configuration

## Building

```bash
# Build everything
make

# Build just liblcd
make liblcd

# Build just microkernel
make microkernel

# Build test modules
make test_mods
```

See the top-level Makefile for more build targets.
