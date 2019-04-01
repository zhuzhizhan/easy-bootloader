.equ PH_CFG1, 0x01c20900
.equ PH_CFG2, 0x01c20904
.equ PH_PULL0, 0x01c20918
.equ PH_PULL1, 0x01c2091c
.equ PH_DAT, 0x01c2090c

.global main

main:
	ldr r0, =PH_CFG1
	ldr r1, =0x10000000
	str r1, [r0]

	ldr r0, =PH_CFG2
	ldr r1, =0x00000001
	str r1, [r0]

