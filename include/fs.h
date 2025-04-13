#ifndef INCLUDE_FS_H
#define INCLUDE_FS_H

#define FS_MAX_FILES 1024
#define FS_MAX_FILENAME_LENGTH 255

typedef struct fs_node {
    char name[FS_MAX_FILENAME_LENGTH];
    unsigned int is_directory;
    struct fs_node* parent;
    struct fs_node* children[FS_MAX_FILES];
    unsigned int num_children;
} fs_node_t;

extern fs_node_t *fs_root;
extern fs_node_t *fs_current;

void fs_init();
void fs_create(const char *name, fs_node_t *parent);
void fs_delete(const char *name, fs_node_t *parent);
void fs_move(const char *src, const char *dest, fs_node_t *parent);
void fs_mkdir(const char *name, fs_node_t *parent);
void fs_ls(fs_node_t *parent);
void fs_pwd(fs_node_t *node);
void fs_cd(const char *name);

fs_node_t* fs_find(const char *name, fs_node_t *parent);

#endif /* INCLUDE_FS_H */
