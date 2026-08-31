#ifndef VFS_H
#define VFS_H

struct file {
    char name[32];
    unsigned char* data;
    unsigned int size;
};

struct vfs_root {
    struct file files[16];
    unsigned int file_count;
};

struct vfs_root root;

__attribute__((aligned(4096))) unsigned char ramdisk_zone[16 * 4096];

static inline void smem(unsigned char* d, const char* s, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        d[i] = (unsigned char)s[i];
    }
}

int storage_write(const char* filename, const char* content, unsigned int len) {
    if (root.file_count >= 16 || len > 4096) return -1;
    unsigned int offset = root.file_count * 4096;
    unsigned char* target_zone = &ramdisk_zone[offset];
    unsigned int name_len = 0;
    while (filename[name_len] && name_len < 31) {
        root.files[root.file_count].name[name_len] = filename[name_len];
        name_len++;
    }
    root.files[root.file_count].name[name_len] = '\0';
    smem(target_zone, content, len);
    root.files[root.file_count].data = target_zone;
    root.files[root.file_count].size = len;
    root.file_count++;
    return 0;
}

#endif