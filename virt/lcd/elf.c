#include <linux/elf.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <error.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define roundup(x, y) (                                 \
    {                                                   \
      const typeof(y) __y = y;                          \
      (((x) + (__y - 1)) / __y) * __y;                  \
    }                                                   \
    )

int main(int argc, char* argv[]) {
  struct stat stat;
  char* buf;
  Elf64_Ehdr* ehdr;
  Elf64_Phdr* phdr;
  int fd, i;
  Elf64_Nhdr *nhdr;
  
  fd = open(argv[1], O_RDONLY); // O_RDWR
  if (fd < 0) {
    perror("vmlinux open");
    return 0;
  }

  if (fstat(fd, &stat) != 0) {
    perror("fstat");
    goto after_fd_err_out;
  }

  buf = (char*)mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (buf == MAP_FAILED) {
    perror("mmap");
    goto after_fd_err_out;
  }

  ehdr = (Elf64_Ehdr*)buf;
  if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
    fprintf(stderr, "%s not ELF\n", argv[1]);
    goto after_mmap_err_out;
  }

  printf("ELF class %d\n", ehdr->e_ident[EI_CLASS]);

  phdr = (Elf64_Phdr*)(buf + ehdr->e_phoff);
  for (i = 0; i < ehdr->e_phnum; ++i) {
    if (phdr[i].p_type == PT_LOAD) {
      printf("LOAD ");
    } else if (phdr[i].p_type == PT_NOTE) {
      printf("NOTE ");
    } else
      printf("NOT CARE ");
    printf("HDR: ofs %llx, va %p, pa %p, fsz %llx, msz %llx, al %llx\n",
           phdr[i].p_offset,
           (void*)phdr[i].p_vaddr,
           (void*)phdr[i].p_paddr,
           phdr[i].p_filesz,
           phdr[i].p_memsz,
           phdr[i].p_align);

    if (phdr[i].p_type == PT_NOTE) {
      char* name, *desc;
      __u64 ofs = phdr[i].p_offset;
      while (ofs < phdr[i].p_offset + phdr[i].p_filesz) {
        nhdr = (Elf64_Nhdr*)(buf + ofs);
        printf("NOTE nsz %x, dsz %x, tp %x\n",
               nhdr->n_namesz, nhdr->n_descsz, nhdr->n_type);
        ofs += sizeof(Elf64_Nhdr);
        name = buf + ofs;
        ofs += roundup(nhdr->n_namesz, phdr[i].p_align);
        desc = buf + ofs;
        printf("\tname %s, desc %s\n", name, desc);
        ofs += roundup(nhdr->n_descsz, phdr[i].p_align);
      }
    }
  }  

after_mmap_err_out:
  munmap(buf, stat.st_size);
after_fd_err_out:
  close(fd);
err_out:
  return 0;
}
