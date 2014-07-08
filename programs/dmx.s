#-----------ENABLE GPIO 14 TO UART---------
#Set GPIO 14 to transmit UART Tx, alt function 0
ldr r0, =0x20200004
ldr r1, =0x00004000
str r1, [r0]

#-----------CONFIGURE UART------------
#Load UART base register to r1
ldr r1, =0x20201000

#Disable UART manually
ldr r0, =0
str r0, [r1, #0x30]

#Set baud rate divisor
#It is 1 to output 250kbps
ldr r0, =1
str r0, [r1, #0x24]

#Set baud rate fractional divisor
ldr r0, =0
str r0, [r1, #0x28]

#Set transmission settings on Line Control Register
ldr r0, =0x68
str r0, [r1, #0x2c]

#Enable Tx and UART
ldr r0, =0x101
str r0, [r1, #0x30]

#-------------GET DATA-----------
#Show data base register, pre-determined
ldr r2, =0x0000812c

#-----------OUTPUT DATA--------
main:
#Get the number of scenes
ldr r3, [r2]
mov r3, r3, ror #16
ldr r0, =0x0000ffff
and r3, r3, r0
#Set data byte offset
ldr r5, =0

show:
#Get the delay between the scenes
ldr r4, [r2]
ldr r0, =0x0000ffff
and r4, r4, r0

scene:
#Wait until UART is NOT BUSY
wait0:
ldr r0, [r1, #0x18]
and r0, r0, #8
cmp r0, #0
bne wait0

#OUTPUT BREAK
#Slow down UART baud rate
ldr r0, =2
str r0, [r1, #0x24]
ldr r0, =32
str r0, [r1, #0x28]

#Break
ldr r0, =0x0
str r0, [r1]

#Wait until UART is NOT BUSY
wait1:
ldr r0, [r1, #0x18]
and r0, r0, #8
cmp r0, #0
bne wait1

#Speed up UART to standard baud rate
ldr r0, =1
str r0, [r1, #0x24]
ldr r0, =0
str r0, [r1, #0x28]

#Write the START CODE
ldr r0, =0
str r0, [r1]

#Wait until UART is NOT BUSY
wait2:
ldr r0, [r1, #0x18]
and r0, r0, #8
cmp r0, #0
bne wait2

ldr r6, =4
data:
#Output scene data 64 CHANNELS
add r7, r5, r6
ldr r0, [r2, r7]
and r0, r0, #0xff

str r0, [r1]

#Wait until UART is NOT BUSY
wait3:
ldr r0, [r1, #0x18]
and r0, r0, #8
cmp r0, #0
bne wait3

#Increment offset
add r6, r6, #1
cmp r6, #68
bne data

#Decrease delay count
sub r4, r4, #1
cmp r4, #0
bne scene

#If the scene is done move on to the next
add r5, r5, #64
sub r3, r3, #1
cmp r3, #0
bne show

#Infinite loop
b main

andeq r0, r0, r0
