#include "../include/shell.h"
#include "../include/fs.h"
#include "../include/fb.h"
#include "../include/keyboard.h"
#include "../include/string.h"
#include "../include/serial.h"

#define MAX_INPUT_LEN 256
#define MAX_ARGS 10
#define MAX_ECHO_LEN 200

void shell()
{
    char input[MAX_INPUT_LEN];
    char temp_char[2];
    char echo_buffer[MAX_ECHO_LEN];

    write("Welcome to Light_OS\n", 20);
    write("File system initialized.\n", 25);
    write("Entering shell...\n", 18);
    newline();

    while (1)
    {
        fs_pwd(fs_current);
        write("> ", 2);
        int i = 0;
        char c;
        temp_char[1] = '\0';

        while (i < MAX_INPUT_LEN - 1)
        {
            c = keyboard_read_char();
            if (c == 0)
                continue;
            if (c == '\n')
            {
                input[i] = '\0';
                newline();
                break;
            }
            else if (c == '\b')
            {
                if (i > 0)
                {
                    i--;
                    if (cursor_position > 0)
                    {
                        move_cursor(cursor_position / FB_WIDTH, (cursor_position % FB_WIDTH) - 1);
                        write(" ", 1);
                        move_cursor(cursor_position / FB_WIDTH, (cursor_position % FB_WIDTH) - 1);
                    }
                }
            }
            else if (c >= ' ')
            {
                input[i++] = c;
                temp_char[0] = c;
                write(temp_char, 1);
            }
        }
        input[i] = '\0';
        if (c != '\n')
            newline();

        if (strlen(input) == 0)
            continue;

        char *args[MAX_ARGS];
        int argc = 0;
        char *current_pos = input;
        char *token_start = NULL;
        while (*current_pos != '\0' && argc < MAX_ARGS)
        {
            while (*current_pos == ' ')
                current_pos++;
            if (*current_pos == '\0')
                break;
            token_start = current_pos;
            while (*current_pos != ' ' && *current_pos != '\0')
                current_pos++;
            if (*current_pos != '\0')
            {
                *current_pos = '\0';
                current_pos++;
            }
            args[argc++] = token_start;
        }
        args[argc] = NULL;

        if (argc == 0)
            continue;

        if (strcmp(args[0], "ls") == 0)
        {
            if (argc == 1)
            {
                fs_ls(fs_current);
            }
            else
            {
                write("ls: Too many arguments (arguments not supported).\n", 50);
            }
        }
        else if (strcmp(args[0], "mkdir") == 0)
        {
            if (argc == 2)
            {
                fs_mkdir(args[1], fs_current);
            }
            else
            {
                write("Usage: mkdir <directory_name>\n", 29);
            }
        }
        else if (strcmp(args[0], "touch") == 0)
        {
            if (argc == 2)
            {
                if (fs_find(args[1], fs_current) == NULL)
                {
                    fs_create(args[1], fs_current);
                }
                else
                {
                    write("File already exists: ", 21);
                    write(args[1], strlen(args[1]));
                    newline();
                }
            }
            else
            {
                write("Usage: touch <file_name>\n", 25);
            }
        }
        else if (strcmp(args[0], "rm") == 0)
        {
            if (argc == 2)
            {
                fs_delete(args[1], fs_current);
            }
            else
            {
                write("Usage: rm <file_or_directory_name>\n", 35);
            }
        }
        else if (strcmp(args[0], "mv") == 0)
        {
            if (argc == 3)
            {
                fs_node_t *src_node = fs_find(args[1], fs_current);
                if (src_node == NULL)
                {
                }
                fs_move(args[1], args[2], fs_current);
            }
            else
            {
                write("Usage: mv <source> <destination>\n", 31);
            }
        }
        else if (strcmp(args[0], "pwd") == 0)
        {
            if (argc == 1)
            {
                fs_pwd(fs_current);
                newline();
            }
            else
            {
                write("pwd: too many arguments\n", 24);
            }
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            if (argc == 2)
            {
                fs_cd(args[1]);
            }
            else if (argc == 1)
            {
                write("cd: missing operand (no home directory concept yet)\n", 52);
            }
            else
            {
                write("cd: too many arguments\n", 23);
            }
        }
        else if (strcmp(args[0], "cat") == 0)
        {
            if (argc == 2)
            {
                fs_node_t *file_node = fs_find(args[1], fs_current);
                if (file_node != NULL && !file_node->is_directory)
                {
                    write("cat: File content reading not implemented for '", 47);
                    write(args[1], strlen(args[1]));
                    write("'.\n", 3);
                }
                else if (file_node != NULL && file_node->is_directory)
                {
                    write("cat: '", 6);
                    write(args[1], strlen(args[1]));
                    write("' is a directory.\n", 19);
                }
                else
                {
                    write("cat: '", 6);
                    write(args[1], strlen(args[1]));
                    write("': No such file or directory.\n", 29);
                }
            }
            else
            {
                write("Usage: cat <filename>\n", 22);
            }
        }
        else if (strcmp(args[0], "echo") == 0)
        {
            int redirect_index = -1;
            char *filename = NULL;

            for (int j = 1; j < argc; j++)
            {
                if (strcmp(args[j], ">") == 0)
                {
                    if (j + 1 < argc)
                    {
                        redirect_index = j;
                        filename = args[j + 1];
                        break;
                    }
                    else
                    {
                        write("echo: Syntax error near unexpected token `newline'\n", 50);
                        filename = (char *)-1;
                        break;
                    }
                }
            }

            if (filename == (char *)-1)
            {
                continue;
            }

            echo_buffer[0] = '\0';
            int current_len = 0;
            int limit = (redirect_index != -1) ? redirect_index : argc;

            for (int j = 1; j < limit; j++)
            {
                int arg_len = strlen(args[j]);
                if (current_len + arg_len + (j > 1 ? 1 : 0) < MAX_ECHO_LEN)
                {
                    if (j > 1)
                    {
                        strcat(echo_buffer, " ");
                        current_len++;
                    }
                    strcat(echo_buffer, args[j]);
                    current_len += arg_len;
                }
                else
                {
                    write("echo: Output string too long.\n", 29);
                    filename = (char *)-1;
                    break;
                }
            }

            if (filename == (char *)-1)
            {
                continue;
            }

            if (filename != NULL)
            {
                fs_node_t *file_node = fs_find(filename, fs_current);
                if (file_node != NULL && file_node->is_directory)
                {
                    write("echo: Cannot write to '", 24);
                    write(filename, strlen(filename));
                    write("': Is a directory\n", 19);
                }
                else
                {
                    if (file_node == NULL)
                    {
                        fs_create(filename, fs_current);
                        file_node = fs_find(filename, fs_current);
                        if (file_node == NULL)
                        {
                            write("echo: Failed to create file '", 29);
                            write(filename, strlen(filename));
                            write("'.\n", 3);
                            continue;
                        }
                    }
                    write("echo: Wrote '", 13);
                    write(echo_buffer, strlen(echo_buffer));
                    write("' to '", 6);
                    write(filename, strlen(filename));
                    write("' (simulation - content not stored).\n", 36);
                }
            }
            else
            {
                write(echo_buffer, strlen(echo_buffer));
                newline();
            }
        }
        else
        {
            write("Unknown command: ", 17);
            write(args[0], strlen(args[0]));
            newline();
        }
    }
}