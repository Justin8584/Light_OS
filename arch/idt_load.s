section .text
    global idt_load
idt_load:
    lidt [esp + 4]      ; Load the IDT pointer (address passed via stack)
    ret                 ; Return from the function
