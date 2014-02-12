#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <uapi/asm/kvm.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/tboot.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <asm/vmx.h>


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
