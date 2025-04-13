section .text
    global gdt_flush
gdt_flush:
    lgdt [esp + 4]      ; Load the GDT pointer (address passed via stack)
    mov ax, 0x10        ; Load the data segment selector
    mov ds, ax          ; Update the data segment registers
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ; Far jump to reload the code segment
.flush:
    ret                 ; Return from the function
