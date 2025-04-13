#include "../include/fs.h"
#include "../include/string.h"
#include "../include/stdlib.h"
#include "../include/serial.h"
#include "../include/fb.h"

fs_node_t *fs_root = NULL;
fs_node_t *fs_current = NULL;

void fs_init()
{
    fs_root = (fs_node_t *)malloc(sizeof(fs_node_t));
    if (fs_root != NULL)
    {
        strcpy(fs_root->name, "/");
        fs_root->is_directory = 1;
        fs_root->parent = NULL;
        fs_root->num_children = 0;
        fs_current = fs_root;
    }
    else
    {
    }
}

fs_node_t *fs_find(const char *name, fs_node_t *parent)
{
    if (parent == NULL)
        return NULL;
    for (unsigned int i = 0; i < parent->num_children; i++)
    {
        if (parent->children[i] != NULL && strcmp(parent->children[i]->name, name) == 0)
        {
            return parent->children[i];
        }
    }
    return NULL;
}

void fs_create(const char *name, fs_node_t *parent)
{
    if (parent == NULL || parent->num_children >= FS_MAX_FILES)
    {
        write("Cannot create file: Invalid parent or directory full.\n", 55);
        return;
    }
    if (fs_find(name, parent) != NULL)
    {
        write("Cannot create file: '", 22);
        write(name, strlen(name));
        write("' already exists.\n", 18);
        return;
    }

    fs_node_t *new_node = (fs_node_t *)malloc(sizeof(fs_node_t));
    if (new_node != NULL)
    {
        if (strlen(name) >= FS_MAX_FILENAME_LENGTH)
        {
            write("Cannot create file: Filename too long.\n", 39);
            free(new_node);
        }
        strcpy(new_node->name, name);
        new_node->is_directory = 0;
        new_node->parent = parent;
        new_node->num_children = 0;
        parent->children[parent->num_children++] = new_node;
        write("File created: ", 14);
        write(name, strlen(name));
        newline();
    }
    else
    {
        write("Cannot create file: Memory allocation failed.\n", 45);
    }
}

void fs_delete(const char *name, fs_node_t *parent)
{
    if (parent == NULL)
        return;
    fs_node_t *node_to_delete = NULL;
    int index_to_delete = -1;

    for (unsigned int i = 0; i < parent->num_children; i++)
    {
        if (parent->children[i] != NULL && strcmp(parent->children[i]->name, name) == 0)
        {
            node_to_delete = parent->children[i];
            index_to_delete = i;
            break;
        }
    }

    if (node_to_delete == NULL)
    {
        write("rm: Cannot remove '", 19);
        write(name, strlen(name));
        write("': No such file or directory\n", 29);
        return;
    }

    if (node_to_delete->is_directory && node_to_delete->num_children > 0)
    {
        write("rm: Cannot remove '", 19);
        write(name, strlen(name));
        write("': Directory not empty\n", 24);
        return;
    }

    free(node_to_delete);

    for (unsigned int j = index_to_delete; j < parent->num_children - 1; j++)
    {
        parent->children[j] = parent->children[j + 1];
    }
    parent->children[parent->num_children - 1] = NULL;
    parent->num_children--;

    write("Removed: ", 9);
    write(name, strlen(name));
    newline();
}

void fs_move(const char *src, const char *dest, fs_node_t *parent)
{
    if (parent == NULL)
        return;

    fs_node_t *node_to_move = fs_find(src, parent);
    if (node_to_move == NULL)
    {
        write("mv: Cannot stat '", 18);
        write(src, strlen(src));
        write("': No such file or directory\n", 29);
        return;
    }

    if (strcmp(src, dest) == 0)
    {
        write("mv: '", 5);
        write(src, strlen(src));
        write("' and '", 7);
        write(dest, strlen(dest));
        write("' are the same file\n", 20);
        return;
    }

    fs_node_t *dest_dir_node = NULL;
    int is_rename = 0;

    if (strcmp(dest, "..") == 0)
    {
        dest_dir_node = parent->parent;
        if (dest_dir_node == NULL)
        {
            write("mv: Cannot move to parent from root directory.\n", 46);
            return;
        }
        if (fs_find(src, dest_dir_node) != NULL)
        {
            write("mv: Cannot move '", 17);
            write(src, strlen(src));
            write("': File exists in parent directory.\n", 36);
            return;
        }
    }
    else
    {
        fs_node_t *potential_dest_dir = fs_find(dest, parent);
        if (potential_dest_dir != NULL && potential_dest_dir->is_directory)
        {
            dest_dir_node = potential_dest_dir;
            if (node_to_move == dest_dir_node)
            {
                write("mv: Cannot move a directory into itself.\n", 39);
                return;
            }
            if (fs_find(src, dest_dir_node) != NULL)
            {
                write("mv: Cannot move '", 17);
                write(src, strlen(src));
                write("': File exists in destination directory '", 39);
                write(dest, strlen(dest));
                write("'.\n", 3);
                return;
            }
        }
        else
        {
            if (potential_dest_dir != NULL)
            {
                write("mv: Cannot rename '", 19);
                write(src, strlen(src));
                write("' to '", 6);
                write(dest, strlen(dest));
                write("': Target exists and is not a directory.\n", 39);
                return;
            }
            if (strlen(dest) >= FS_MAX_FILENAME_LENGTH)
            {
                write("mv: Cannot rename '", 19);
                write(src, strlen(src));
                write("': New filename '", 17);
                write(dest, strlen(dest));
                write("' too long.\n", 12);
                return;
            }
            is_rename = 1;
            dest_dir_node = parent;
        }
    }

    // 3. Perform the operation

    if (is_rename)
    {
        strcpy(node_to_move->name, dest);
        write("Renamed '", 9);
        write(src, strlen(src));
        write("' to '", 6);
        write(dest, strlen(dest));
        write("'.\n", 3);
    }
    else
    {

        if (dest_dir_node->num_children >= FS_MAX_FILES)
        {
            write("mv: Destination directory is full.\n", 34);
            return;
        }

        int src_index = -1;
        for (unsigned int i = 0; i < parent->num_children; i++)
        {
            if (parent->children[i] == node_to_move)
            {
                src_index = i;
                break;
            }
        }

        if (src_index == -1)
        {
            write("mv: Internal error - source node not found in parent list.\n", 59);
            return;
        }

        for (unsigned int j = src_index; j < parent->num_children - 1; j++)
        {
            parent->children[j] = parent->children[j + 1];
        }
        parent->children[parent->num_children - 1] = NULL;
        parent->num_children--;

        dest_dir_node->children[dest_dir_node->num_children++] = node_to_move;
        node_to_move->parent = dest_dir_node;

        write("Moved '", 7);
        write(src, strlen(src));
        write("' to '", 6);
        if (strcmp(dest, "..") == 0)
        {
            write("..", 2);
        }
        else
        {
            write(dest, strlen(dest));
        }
        write("'.\n", 3);
    }
}

void fs_mkdir(const char *name, fs_node_t *parent)
{
    if (parent == NULL || parent->num_children >= FS_MAX_FILES)
    {
        write("Cannot create directory: Invalid parent or directory full.\n", 59);
        return;
    }
    if (fs_find(name, parent) != NULL)
    {
        write("Cannot create directory '", 25);
        write(name, strlen(name));
        write("': Already exists.\n", 18);
        return;
    }
    if (strlen(name) >= FS_MAX_FILENAME_LENGTH)
    {
        write("Cannot create directory: Name too long.\n", 39);
        return;
    }

    fs_node_t *new_node = (fs_node_t *)malloc(sizeof(fs_node_t));
    if (new_node != NULL)
    {
        strcpy(new_node->name, name);
        new_node->is_directory = 1;
        new_node->parent = parent;
        new_node->num_children = 0;
        parent->children[parent->num_children++] = new_node;
        write("Directory created: ", 19);
        write(name, strlen(name));
        newline();
    }
    else
    {
        write("Cannot create directory: Memory allocation failed.\n", 50);
    }
}

void fs_ls(fs_node_t *parent)
{
    if (parent == NULL)
        return;
    if (parent->num_children == 0)
    {
        return;
    }
    for (unsigned int i = 0; i < parent->num_children; i++)
    {
        if (parent->children[i] != NULL)
        {
            char *name = parent->children[i]->name;
            size_t name_len = strlen(name);
            write(name, name_len);
            if (parent->children[i]->is_directory)
            {
                write("/", 1);
            }
            write("\n", 1);
        }
    }
}

void fs_pwd_recursive(fs_node_t *node)
{
    if (node == NULL || node->parent == NULL)
    {
        return;
    }
    fs_pwd_recursive(node->parent);
    if (node->parent != fs_root)
    {
        write("/", 1);
    }
    write(node->name, strlen(node->name));
}

void fs_pwd(fs_node_t *node)
{
    if (node == NULL)
        return;
    write("/", 1);
    if (node != fs_root)
    {
        fs_pwd_recursive(node);
    }
}

void fs_cd(const char *name)
{
    if (fs_current == NULL)
        return;
    if (strcmp(name, "..") == 0)
    {
        if (fs_current->parent != NULL)
        {
            fs_current = fs_current->parent;
        }
        else
        {
            write("cd: Already at root directory.\n", 29);
        }
    }
    else
    {
        fs_node_t *new_dir = fs_find(name, fs_current);
        if (new_dir != NULL && new_dir->is_directory)
        {
            fs_current = new_dir;
        }
        else if (new_dir != NULL && !new_dir->is_directory)
        {
            write("cd: '", 5);
            write(name, strlen(name));
            write("' is not a directory.\n", 22);
        }
        else
        {
            write("cd: No such directory: ", 23);
            write(name, strlen(name));
            write("\n", 1);
        }
    }
}
