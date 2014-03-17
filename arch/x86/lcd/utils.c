#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <uapi/asm/kvm.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/tboot.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <asm/vmx.h>

#include <uapi/linux/elf.h>

#include "lcd_defs.h"

int lcd_read_mod_file(const char* filepath,
                      void** filecontent, long* size) {
  struct file* filp = NULL;
  mm_segment_t oldfs;
  int err = 0;
  loff_t pos = 0;

  oldfs = get_fs();
  set_fs(get_ds());

  filp = filp_open(filepath, O_RDONLY, 0);
  if (IS_ERR(filp)) {
    err = PTR_ERR(filp);
    printk(KERN_ERR "Error when opening file %d\n", err);
    goto error_out;
  }

  *size = i_size_read(file_inode(filp));
  if (*size < 0) {
    err = (int)*size;
    printk(KERN_ERR "File size error\n");
    goto after_open_error_out;
  }
  *filecontent = vmalloc(*size);
  if (!*filecontent) {
    printk(KERN_ERR "Failed to allocate %lu mem for file\n", *size);
    goto after_open_error_out;
  }

  err = vfs_read(filp, *filecontent, *size, &pos);
  if (err != (int)*size) {
    printk(KERN_ERR "Failed to read %s content\n", filepath);
    vfree(*filecontent);
    *filecontent = NULL;
    goto after_open_error_out;
  } else
    err = 0;
  
after_open_error_out:
  filp_close(filp, NULL);
error_out:
  set_fs(oldfs);
  return err; 
}


void process_vmlinux_note(lcd_struct *lcd, Elf64_Phdr *hdr,
                          struct file *filp) {
  char *name, *desc;
  u64 ofs;
  Elf64_Nhdr nhdr;
  int ret;
  u64 pos;

  /* REQ: Name and Desc size less than half page  */
  name = (char*)__get_free_page(GFP_KERNEL);
  if (!name) {
    printk(KERN_ERR "Out of mem for note\n");
    return;
  }
  desc = name + (PAGE_SIZE > 1);

  ofs = hdr->p_offset;
  pos = ofs;
  while (ofs < hdr->p_offset + hdr->p_filesz) {
    ret = vfs_read(filp, (char*)&nhdr, sizeof(nhdr), &pos);
    if (ret != sizeof(nhdr)) {
      printk(KERN_ERR "Error reading vmlinux note\n");
      goto after_mem_out;
    }

    if (nhdr.n_namesz > 0) {
      ret = vfs_read(filp, name,
                     round_up(nhdr.n_namesz, hdr->p_align),
                     &pos);
      if (ret != round_up(nhdr.n_namesz, hdr->p_align)) {
        printk(KERN_ERR "Error reading vmlinux note name\n");
        goto after_mem_out;
      }
    }

    if (nhdr.n_descsz > 0) {
      ret = vfs_read(filp, desc,
                     round_up(nhdr.n_descsz, hdr->p_align),
                     &pos);
      if (ret != round_up(nhdr.n_descsz, hdr->p_align)) {
        printk(KERN_ERR "Error reading vmlinux note desc\n");
        goto after_mem_out;
      }
    }

    /* TODO: use Xen notes */
    
  }

after_mem_out:
  free_page((u64)name);      
}

void load_vmlinux_segment(lcd_struct *lcd, Elf64_Phdr *hdr,
                          struct file *filp) {
  u64 sz, left;
  int ret;
  u64 hva, gpa;
  u64 pos;

  pos = hdr->p_offset;
  left = hdr->p_filesz;

  gpa = round_down(hdr->p_paddr, PAGE_SIZE);

  sz = PAGE_SIZE - (hdr->p_paddr - gpa);
  if (sz > hdr->p_filesz)
    sz = hdr->p_filesz;

  while (left > 0) {
    ret = lcd_find_hva_by_gpa(lcd, gpa, &hva);
    if (ret == -ENOENT) {
      hva = __get_free_page(GFP_KERNEL);
      if (!hva) {
        printk(KERN_ERR "Out of memory for vmlinux\n");
        return;
      }
      if (lcd_map_gpa_to_hpa(lcd, gpa, __pa(hva), 0)) {
        printk(KERN_ERR "Map ept page failed\n");
        free_page(hva);
        return;
      }
      if (lcd_map_gva_to_gpa(lcd, gpa, gpa, 1, 0)) {
        printk(KERN_ERR "Map gva to gpa failed\n");
        return;
      }
    } else if (ret) {
      printk(KERN_ERR "Find hva error %p\n", (void*)gpa);
      return;
    }

    if (left == hdr->p_filesz)
      hva += hdr->p_paddr - gpa;
    
    ret = vfs_read(filp, (void*)hva, sz, &pos);
    if (ret != sz) {
      printk(KERN_ERR "Error reading vmlinux %d\n", ret);
      return;
    }
    
    left -= sz;
    sz = (left > PAGE_SIZE)? PAGE_SIZE:left;
    gpa += PAGE_SIZE;
  }  
}

int lcd_load_vmlinux(const char* kfile, lcd_struct *lcd,
                     u64 *elf_entry) {
  Elf64_Ehdr hdr;
  Elf64_Phdr *phdrs;
  mm_segment_t oldfs;
  struct file* filp;
  loff_t pos = 0;
  int err = 0;
  int i = 0;

  oldfs = get_fs();
  set_fs(get_ds());

  filp = filp_open(kfile, O_RDONLY, 0);
  if (IS_ERR(filp)) {
    err = PTR_ERR(filp);
    printk(KERN_ERR "Error when opening vmlinux file %d\n", err);
    goto error_out;
  }

  err = vfs_read(filp, (char*)&hdr, sizeof(hdr), &pos);
  if (err != sizeof(hdr)) {
    printk(KERN_ERR "Error when reading vmlinux %d\n", err);
    goto after_open_error_out;
  }
  err = 0;

  if (memcmp(hdr.e_ident, ELFMAG, SELFMAG) != 0) {
    err = -EINVAL;
    printk(KERN_ERR "Invalide vmlinux file\n");
    goto after_open_error_out;
  }

  phdrs = kmalloc(sizeof(Elf64_Phdr)*hdr.e_phnum, GFP_KERNEL);
  if (!phdrs) {
    err = -ENOMEM;
    printk(KERN_ERR "Out of mem\n");
    goto after_open_error_out;
  }

  pos = hdr.e_phoff;
  err = vfs_read(filp, (char*)phdrs, sizeof(Elf64_Phdr)*hdr.e_phnum, &pos);
  if (err != sizeof(Elf64_Phdr)*hdr.e_phnum) {
    printk(KERN_ERR "Error when reading p hdrs %d\n", err);
    goto after_mem_error_out;
  }
  err = 0;

  for (i = 0; i < hdr.e_phnum; ++i) {
    if (phdrs[i].p_type != PT_LOAD) {
      if (phdrs[i].p_type != PT_NOTE)
        continue;
      else {
        process_vmlinux_note(lcd, &phdrs[i], filp);
      }
    } else {
      load_vmlinux_segment(lcd, &phdrs[i], filp);
    }
  }

  *elf_entry = hdr.e_entry;

after_mem_error_out:
  kfree(phdrs);
after_open_error_out:
  filp_close(filp, NULL);
error_out:
  set_fs(oldfs);
  return err;  
}


struct exit_reason_item {
  int code;
  const char *reason;
};

static struct exit_reason_item vmx_exit_reason_table[] = {
  VMX_EXIT_REASONS };

const char *lcd_exit_reason(int exit_code) {
  int i = ARRAY_SIZE(vmx_exit_reason_table);
  struct exit_reason_item *tbl = vmx_exit_reason_table;

  while (i--) {
    if (tbl->code == exit_code)
      return tbl->reason;
    tbl++;
  }
  
  printk(KERN_ERR "unknown VMX exit code:%d\n",
         exit_code);
  return "UNKNOWN";
}
EXPORT_SYMBOL(lcd_exit_reason);
