#set GPIO 16 as output
ldr r0, =0x20200004
ldr r2, =0x00040000
str r2, [r0]

#load adresses
ldr r0, =0x20200028
ldr r1, =0x2020001C
ldr r2, =0x00010000

#clear LED
str r2, [r0]

#infinite loop
loop:

#set LED ON
str r2, [r1]

#wait
ldr r3, =0x1000000
loop2:
sub r3, r3, #1
cmp r3, #0
bne loop2

#clear LED / set LED OFF
str r2, [r0]

#wait
ldr r3, =0x1000000
loop3:
sub r3, r3, #1
cmp r3, #0
bne loop3

b loop

#stop
andeq r0, r0, r0
