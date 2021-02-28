/**
  ******************************************************************************
  * @Project        : ARM_07-Stack
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-28-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Configure the Stack RAM in two parts for MSP and PSP with 521
  * 				  bytes each.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note:
  * 	-We use 1kbyte of RAM for produced two stacks working with MSP and the
  * 	 other with PSP, 512bytes each.
  * 	-MSP is initialize in startup_stm32f407vgtx.s like 0x20020000 (SRAM_END)
  * 	 After execute main, the MSP change.
  * 	Reset_Handler:
  *	  	  ldr   r0, =_estack	 (_estack = 0x20020000)
  *	  	  mov   sp, r0           (set stack pointer)
  *
  *
  * 	The RAM information (RM0090,71/1749):
  *
  * 		SRAM_START 			0x20000000U
  * 		SRAM_SIZE 			((112+16) * 1024 - 1)
  * 		SRAM_END 			((SRAM_START) + (SRAM_SIZE))
  *
  *		The stack information and PSP / MSP
  *
  * 		STACK_START 		SRAM_END
  * 		STACK_MSP_START		STACK_START
  * 		STACK_MSP_END 	    (STACK_MSP_START - 512)
  * 		STACK_PSP_START 	STACK_MSP_END
  *
  *						STACK1			 STACK2
  *	0x20000000U		|				|0x2001fe00		|0x20020000
  *	SRAM_START		|				|				|SRAM_END
  *					|STACK_END*		|				|STACK_START
  *					|				|STACK_MSP_END	|STACK_MSP_START
  *					|STACK_PSP_END*	|STACK_PSP_START|
  * -------------------------------------------------
  * 				|		PSP	 	|		MSP		|
  * -------------------------------------------------
  * 				0			 512bytes		   1kb
  *
  ******************************************************************************
**/


#include<stdio.h>

/* Macros just for see calculations in debugger not for program*/
#define SRAM_START 			0x20000000U
#define SRAM_SIZE 			((112+16) * 1024)
#define SRAM_END 			((SRAM_START) + (SRAM_SIZE))

#define STACK_START 		SRAM_END
#define STACK_MSP_START		STACK_START
#define STACK_MSP_END 	    (STACK_MSP_START - 512)
#define STACK_PSP_START 	STACK_MSP_END
/* ---------------------------------------------------- */


int fun_add(int a, int b , int c , int d)
{
	return a+b+c+d;
}

/* this function changes SP to PSP */
__attribute__((naked)) void change_sp_to_psp(void)
{
	/*Initialize the end of the user RAM */
	__asm volatile(".equ SRAM_END, (0x20000000 + ( 128 * 1024))");
	/*Initialize the start of the RAM for PSP*/
	__asm volatile(".equ PSP_START, (SRAM_END-512)");

	/*------------------------------------------------------------*/
	/*Initialize PSP in the start of the first stack of RAM */
	/*------------------------------------------------------------*/
	/*Load address at PSP_START in R0*/
	__asm volatile("LDR R0,=PSP_START");
	/*Load R0 in PSP (PSP init)*/
	__asm volatile("MSR PSP, R0");
	/*Load 0x02 in R0*/
	__asm volatile("MOV R0,#0X02");
	/*Load in CONTROL the R0 value. Change the control of PS to PSP.
	 * CONTROL Bit[1] = 1 (PSP)
	 *CONTROL Bit[1] = 0 (MSP)*/
	__asm volatile("MSR CONTROL,R0");
	/*------------------------------------------------------------*/

	/*__attribute__((naked)) void change_sp_to_psp(void) is a assembly
	 * function, for this reason, we have to jump manually to the next
	 * function, that return address to main is keep in LR.
	 * LR is loaded to PC for jump back to main...*/
	__asm volatile("BX LR");

}

void generate_exception(void)
{
	/* execute SVC instruction to cause SVC exception */
	__asm volatile("SVC #0X2");
}

int main(void)
{
	/*Change control of PS to PSP*/
	change_sp_to_psp();

	/* SP
	 * MSP: 0x2001fff0 (change for call main in startup)
	 * PSP: 0x2001fdfc
	 * */

	/* from here onwards PSP will be used for stack activities */
	int ret;
	ret = fun_add(1, 4, 5, 6);
	printf("result = %d\n",ret);

	/* SP change to MSP*/
	generate_exception();

	/* SP change to PSP*/
	for(;;);
}


void SVC_Handler(void)
{
	printf(" in SVC_Handler\n");
}
