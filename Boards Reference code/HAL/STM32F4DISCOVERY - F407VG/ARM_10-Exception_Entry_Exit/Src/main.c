/**
  ******************************************************************************
  * @Project        : ARM_10-Exception_Entry_Exit
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-28-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Example to analysis of frame stack with a interrupt.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note:
  *
  *   						Stack Frame
  * 			+++.*************************
  * 	MSP(PUSH)-->|	xPSR				|
  * 				|	PC (Return address)	|
  * 				|	LR					|
  * 				|	R12					|
  * 				|	R3					|
  * 				|	R2					|
  * 				|	R1					|
  * 	MSP(POP)-->	|	R0 					|
  *				---.*************************
  *
  *  * 	-LR have EXC_RETURN[31-0]
  * 		-0xfffffff9 for return from handler mode before configured like a MSP
  * 		-0xfffffffd for return to thread mode before configured like a PSP
  *
  * 	-With PS MSP without PSP initialized
  * 		Before the interrupt PS=MSP Get MSP address. We can see in that address
  * 		the stack frame with the memory browser, when the interrupt trigger, then
  *	    	the stack frame will be MSP - 4, because the decrement.
  * 		In entry interrupt the CPU PUSH, decrementing the MSP from xPSR to R0.
  * 			the stack is save in the CPU stack frame with PUSH. LR is load with
  * 			EXC_RETURN.
  * 		In entry interrupt the CPU POP, incrementing the MSP from R0 to xPSR.
  *				the frame is return to the CPU with POP. LR return the address of
  *				before the entry and put it in PC.
  *
  *				0x2001ffe8 - 4 MSP before interrupt
  *				0x2001ffe4 (memory browser)
  *
  *				00000001	xprs
  *				0C020008	pc
  *				2B020008	lr
  *				00000000	r12
  *				00EF00E0	r3
  *				03000000	r2
  *				48020020	r1
  *				0A000000	r0
  *
  *				Value of LR in debug is 0xFFFFFFF9 EXC_RETURN. Come back and exit
  *				with thread mode and MSP.
  *
  *			-With PS MSP and PSP initialized
  *				0x20007ff0 - 4 MSP before interrupt
  *				0x20007fec (memory browser)
  *
  *				00000001	xprs
  *				0C020008	pc
  *				41020008	lr
  *				00000000	r12
  *				00EF00E0	r3
  *				03000000	r2
  *				48020020	r1
  *				0A000000	r0
  *
  *				Value of LR in debug is 0xFFFFFFFD EXC_RETURN. Come back and exit
  *				with thread mode and PSP.
  *
  ******************************************************************************
**/

#include<stdio.h>
#include<stdint.h>

/* This function executes in THREAD MODE of the processor */
void generate_interrupt()
{
	//Declaro direcciones de los registros
	uint32_t *pSTIR  = (uint32_t*)0xE000EF00;
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	//enable IRQ3 interrupt
	*pISER0 |= ( 1 << 3);

	//generate an interrupt from software for IRQ3
	/*Break point here
	  Get MSP address. We can see in that address the stack frame
	  with the memory browser, when the interrupt trigger, then
	  the stack frame will be MSP - 4, because the decrement*/
	*pSTIR = (3 & 0x1FF);

}

/* This function executes in THREAD MODE of the processor */
int main(void)
{


#if 1
	/*Changing SP to PSP*/
	int control_reg = 0x2;
	uint32_t psp_value = 0x20008000;

   /* this code modifies the CONTROL register for SP selection */
	__asm volatile ("msr PSP,%0"::"r"(psp_value));
	__asm volatile ("msr CONTROL,%0"::"r"(control_reg));
#endif

	printf("In thread mode : before interrupt\n");

	generate_interrupt();

	printf("In thread mode : after interrupt\n");

	for(;;);
}

/* This function(ISR) executes in HANDLER MODE of the processor */
void RTC_WKUP_IRQHandler(void)
{
	printf("In handler mode : ISR\n");
}


