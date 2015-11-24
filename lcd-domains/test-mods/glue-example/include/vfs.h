/**
 * vfs.h - VFS interface.
 */
#ifndef EXAMPLE_VFS_H
#define EXAMPLE_VFS_H

/* DATA ---------------------------------------- */

struct file {
	int id;
	int private;
};

struct fs {
	int id;
	int size;
	struct fs_operations *fs_operations;
};

struct fs_operations {
	int (*new_file)(int id, struct file **file_out);
	void (*rm_file)(struct file *file);
};

/* VFS FUNCTIONS ---------------------------------------- */

int register_fs(struct fs *fs);
void unregister_fs(struct fs *fs);

#endif /* EXAMPLE_VFS_H */
