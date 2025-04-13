# Define source directories
SRC_DIR = src
INC_DIR = include
ASM_DIR = arch

# Define output directory for object files
OBJ_DIR = build

# --- Debug Start ---
$(info --- Variable Values ---)
$(info SRC_DIR: $(SRC_DIR))
$(info INC_DIR: $(INC_DIR))
$(info ASM_DIR: $(ASM_DIR))
$(info OBJ_DIR: $(OBJ_DIR))
# --- Debug End ---


# Find source files automatically
C_SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_SRC_FILES = $(wildcard $(ASM_DIR)/*.s)

# --- Debug Start ---
$(info C_SRC_FILES: $(C_SRC_FILES))
$(info ASM_SRC_FILES: $(ASM_SRC_FILES))
# --- Debug End ---


# Generate object file names based on source files and output directory
C_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRC_FILES))
ASM_OBJ_FILES = $(patsubst $(ASM_DIR)/%.s,$(OBJ_DIR)/%.o,$(ASM_SRC_FILES))
OBJ_FILES = $(C_OBJ_FILES) $(ASM_OBJ_FILES)

# --- Debug Start ---
$(info C_OBJ_FILES: $(C_OBJ_FILES))
$(info ASM_OBJ_FILES: $(ASM_OBJ_FILES))
$(info OBJ_FILES: $(OBJ_FILES))
# --- Debug End ---


# Tools and flags
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I$(INC_DIR) -c -g
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

# Emulator command
EMULATOR = qemu-system-i386

# --- Targets ---

# Default target: Build the bootable ISO
all: light_os.iso

# Rule to compile C files: Places object file in OBJ_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling C: $<"
	$(CC) $(CFLAGS) $< -o $@

# Rule to assemble Assembly files: Places object file in OBJ_DIR
# Uses the new ASM_DIR path for source files
$(OBJ_DIR)/%.o: $(ASM_DIR)/%.s | $(OBJ_DIR)
	@echo "Assembling: $<"
	$(AS) $(ASFLAGS) $< -o $@

# Rule to ensure build directory exists (order-only prerequisite)
$(OBJ_DIR):
	@echo "Creating build directory: $(OBJ_DIR)"
	@mkdir -p $(OBJ_DIR)

# Rule to link the kernel
kernel.elf: $(OBJ_FILES) link.ld
	@echo "Linking kernel: $@"
	@echo "Linker dependencies: $(OBJ_FILES)" # Debug Linker Line
	ld $(LDFLAGS) $(OBJ_FILES) -o $@

# Rule to create the bootable ISO image
light_os.iso: kernel.elf iso/boot/grub/stage2_eltorito iso/boot/grub/menu.lst
	@echo "Creating ISO Image: $@"
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R                              \
							-b boot/grub/stage2_eltorito    \
							-no-emul-boot                   \
							-boot-load-size 4               \
							-A os                           \
							-input-charset utf8             \
							-quiet                          \
							-boot-info-table                \
							-o light_os.iso                 \
							iso

# Rule to run the OS in the emulator
run: all
	@echo "--- Running OS in QEMU ---"
	$(EMULATOR) -cdrom light_os.iso

# Rule to clean up build files (removes entire OBJ_DIR)
clean:
	@echo "--- Cleaning up ---"
	rm -rf $(OBJ_DIR) kernel.elf light_os.iso bochslog.txt serial.out

# Declare targets that don't correspond to files
.PHONY: all run clean