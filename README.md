# Light OS

A lightweight operating system kernel project with basic shell functionality.

## Overview

Light OS is a minimalist operating system kernel that implements:
- Framebuffer for screen output
- Serial port communication
- Basic keyboard input
- Simple file system (in-memory)
- Shell interface with essential commands
- Memory management basics
- Global Descriptor Table (GDT) 
- Interrupt Descriptor Table (IDT)

## Directory Structure

```
└── light_os/
    ├── README.md
    ├── link.ld                 # Linker script
    ├── Makefile                # Build system
    ├── stage2_eltorito         # GRUB bootloader file
    ├── arch/                   # Architecture-specific assembly code
    │   ├── gdt_flush.s         # GDT functions
    │   ├── idt_load.s          # IDT functions
    │   ├── io.s                # I/O port access
    │   ├── loader.s            # Boot loader entry point
    │   └── multiboot_header.s  # Multiboot header
    ├── include/                # Header files
    │   ├── fb.h                # Framebuffer
    │   ├── fs.h                # File system
    │   ├── gdt.h               # Global Descriptor Table
    │   ├── idt.h               # Interrupt Descriptor Table
    │   ├── io.h                # I/O functions
    │   ├── keyboard.h          # Keyboard input
    │   ├── paging.h            # Memory paging
    │   ├── serial.h            # Serial communications
    │   ├── shell.h             # Shell interface
    │   ├── stddef.h            # Standard definitions
    │   ├── stdlib.h            # Standard library
    │   └── string.h            # String manipulation
    ├── iso/                    # ISO image construction
    │   └── boot/
    │       └── grub/           # GRUB bootloader
    │           ├── menu.lst    # GRUB menu configuration
    │           └── stage2_eltorito  # GRUB stage2 bootloader
    └── src/                    # Source code
        ├── fb.c                # Framebuffer implementation
        ├── fs.c                # File system implementation
        ├── gdt.c               # GDT implementation
        ├── idt.c               # IDT implementation
        ├── keyboard.c          # Keyboard implementation
        ├── kmain.c             # Kernel main function
        ├── paging.c            # Memory paging implementation
        ├── serial.c            # Serial port implementation
        ├── shell.c             # Shell implementation
        ├── stdlib.c            # Standard library implementation
        └── string.c            # String functions implementation
```

## Setting Up WSL on Windows 11

### WSL Installation

1. Open PowerShell as Administrator and run:

```powershell
wsl --install
```

This installs WSL 2 with Ubuntu by default.

2. Restart your computer when prompted.

3. After restart, a terminal window will open. Create a username and password for your Ubuntu installation.

### Installing Required Packages

1. Update package listings and upgrade existing packages:

```bash
sudo apt update && sudo apt upgrade -y
```

2. Install required development tools:

```bash
sudo apt install build-essential nasm genisoimage xorriso qemu-system-x86 -y
```

- `build-essential`: Includes GCC, Make, and other essential tools
- `nasm`: Assembler for x86 architecture
- `genisoimage`: Creates ISO images (needed for bootable OS)
- `xorriso`: Advanced ISO manipulation tool
- `qemu-system-x86`: Emulator to test your OS

## Building Light OS

1. Clone or download this repository:

```bash
git clone <repository-url> 
cd <repository-directory>
```

2. Create the build directory:

```bash
mkdir -p build
```

3. Build the OS:

```bash
make
```

This will:
- Compile all C source files
- Assemble all assembly files
- Link them into a kernel ELF executable
- Create a bootable ISO image

## Running Light OS

To run the OS in the QEMU emulator:

```bash
make run
```

This will start QEMU with the Light OS ISO, booting directly into the kernel.

## Available Shell Commands

The Light OS shell supports these commands:

| Command | Description | Usage |
|---------|-------------|-------|
| `ls` | List files and directories in current directory | `ls` |
| `mkdir` | Create a new directory | `mkdir <directory_name>` |
| `touch` | Create a new empty file | `touch <file_name>` |
| `rm` | Remove a file or empty directory | `rm <file_or_directory_name>` |
| `mv` | Move or rename a file or directory | `mv <source> <destination>` |
| `pwd` | Show current directory path | `pwd` |
| `cd` | Change current directory | `cd <directory_name>` or `cd ..` |
| `cat` | Show contents of a file (simulation only) | `cat <file_name>` |
| `echo` | Display a line of text | `echo <text>` |
| `echo > file` | Write text to a file (simulation only) | `echo <text> > <file_name>` |

## Limitations

- File system is in-memory only (no persistence between reboots)
- No multi-user capabilities
- Limited driver support
- No networking capabilities
- Cat command doesn't actually show file contents (simulation only)
- Echo redirection only simulates writing to files

## Troubleshooting

### Common Build Issues

1. **Missing Packages**: Ensure all required packages are installed
2. **Permission Denied**: Use `chmod +x <file>` to make scripts executable if needed
3. **Command Not Found**: Ensure your PATH includes the necessary directories
4. **Build Directory Missing**: Manually create the `build` directory if needed

### Common Run Issues

1. **QEMU Not Starting**: Check QEMU installation, try running with `sudo`
2. **Boot Failure**: Ensure all boot files are in the correct locations
3. **Keyboard Not Working**: Ensure keyboard input isn't captured by Windows

## Contributing

Contributions are welcome! Here are some areas for improvement:

- Add file content storage functionality
- Expand shell command capabilities
- Implement a proper VFS (Virtual File System)
- Add device drivers
- Implement multitasking

## License

MIT License
