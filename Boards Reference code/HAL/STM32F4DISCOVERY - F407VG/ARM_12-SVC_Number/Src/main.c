/**
  ******************************************************************************
  * @Project        : ARM_12-SVC_Number
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 03-01-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Example of acquisitions SVC number.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note:
  *		-Generate a SVC number witch trigger the interrupt of SVC_Handler. We
  *		derived that to a naked function with the purpose to get MSP value.
  *		In the entry of this function we dont have push, because is a naked,
  *		then the next function to execute will use the MSP 0x2001ffd0 to point
  *		the start of the stack frame (R0).
  *
  *		-With the MSP R0 value like a argument to SVC function we have:
  *
  *		pBaseOfStackFrame[7] = xPSR
  *		pBaseOfStackFrame[6] = PC
  *		pBaseOfStackFrame[5] = LR
  *		pBaseOfStackFrame[4] = R12
  *		pBaseOfStackFrame[3] = R3
  *		pBaseOfStackFrame[2] = R2
  *		pBaseOfStackFrame[1] = R1
  *		pBaseOfStackFrame[0] = R0 <- MSP 0x2001ffd0
  *
  *		-With PC (08000298) address we have the address for the next main
  *		instruction it was save, then we can use that for reach the last
  *		execute instruction:
  *
  *		------------------------------------------------------------------------
  *		MAIN:
  *
  *		08000296: 0x0000df03   svc     25
  *		36                   	__asm volatile ("MOV %0,R0": "=r"(data) ::);
  *		08000298: 0x00000346   mov     r3, r0
  *
  *		------------------------------------------------------------------------
  *
  *		That means, decrement the address of PC in 2 for reach the svc instruction
  *		and save the svc number via R0, then we can save it in a C variable.
  *
  ******************************************************************************
**/

#include<stdio.h>
#include<stdint.h>

int main(void)
{

	/*Set the SVC interrupt with number 25*/
	__asm volatile ("SVC #25");

	/*After interrupt*/

	/*Store the R0 (SVC number) value in variable c "data".*/
	/*register uint32_t data __asm("r0");*/
	uint32_t data;
	__asm volatile ("MOV %0,R0": "=r"(data) ::);
	printf(" data = %ld\n",data);

	for(;;);
}


__attribute__ ((naked)) void SVC_Handler(void)
{
	/*In the entry of this function we dont have push, because is a naked
	 function for this reason get the MSP value, then the next function to
	 execute will use the MSP 0x2001ffd0 to point the start of the stack
	 frame (R0)*/

	__asm("MRS R0,MSP");
	__asm("B SVC_Handler_c");
}

void SVC_Handler_c(uint32_t *pBaseOfStackFrame)
{
	/* The argument the address of MSP is pass to pBaseOfStackFrame
		pBaseOfStackFrame[7] = xPSR
		pBaseOfStackFrame[6] = PC
		pBaseOfStackFrame[5] = LR
		pBaseOfStackFrame[4] = R12
		pBaseOfStackFrame[3] = R3
		pBaseOfStackFrame[2] = R2
		pBaseOfStackFrame[1] = R1
		pBaseOfStackFrame[0] = R0 <- MSP 0x2001ffd0
	*/

	printf("in SVC handler\n");

	/* pBaseOfStackFrame[6] is address of PC */
	uint8_t *pReturn_addr = (uint8_t*)pBaseOfStackFrame[6];

	/* 2. decrement the LR return address by 2 to point to opcode of the SVC instruction in the program memory */
	/*
	*   See disassembly 0x8000298 (PC value (*pReturn_addr) the next instruction to execute of main)
	*
	*	08000296: 0x0000df03   svc     25
	*	36                   	__asm volatile ("MOV %0,R0": "=r"(data) ::);
	*	08000298: 0x00000346   mov     r3, r0
	*
	**/

	pReturn_addr-=2;

	/* 3. extract the SVC number (LSByte of the opcode) */
    uint8_t svc_number = *pReturn_addr;

    printf("Svc number is : %d\n",svc_number);

    /* Store the value to remember that in [0] with the svc number. it is R0 */
    pBaseOfStackFrame[0] = svc_number;

}
