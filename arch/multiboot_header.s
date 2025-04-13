section .multiboot
align 4
dd 0x1BADB002             ; Magic number
dd 0x00                   ; Flags
dd -(0x1BADB002 + 0x00)   ; Checksum

section .text
global loader
