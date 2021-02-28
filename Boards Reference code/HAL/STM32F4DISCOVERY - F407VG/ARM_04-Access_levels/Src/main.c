/**
  ******************************************************************************
  * @Project        : ARM_04-Access_levels
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-28-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : See the access modes: privileged and unprivileged modes.
  * 				  We cannot call or modified a privileged register in
  * 				  unprivileged mode it will produced a hardfault. See register
  * 				  CONTROl bit 0 in debugger.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note: none
  *
  ******************************************************************************
**/


#include<stdio.h>
#include<stdint.h>

/* This function executes in THREAD MODE of the processor */
void generate_interrupt()
{
	/*Store the address of ISER register. Enable the interrupt in that IRQ*/
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	/*Store the address of STIR register. It set the interrupt in that IRQ*/
	uint32_t *pSTIR  = (uint32_t*)0xE000EF00;

	/*Enable RTC_WKUP IRQ3 interrupt */
	*pISER0 |= ( 1 << 3);

	/*Set the IRQ3 software interrupt*/
	*pSTIR = (3 & 0x1FF);
}


void change_access_level_unpriv(void)
{
	/*See ref: Cortex-M4 Devices Generic Guide 22/277*/

	/*Store CONTROL to R0*/
	__asm volatile ("MRS R0,CONTROL");
	/*Set bit[0] of R0 (CONTROL register value) */
	__asm volatile ("ORR R0,R0,#0x01");
	/* Load R0 to CONTROL. We set the unprivileged mode*/
	__asm volatile ("MSR CONTROL,R0");

}

/* This function executes in THREAD MODE+ PRIV ACCESS LEVEL of the processor */
int main(void)
{

	/*This moment: Thread with privileged mode*/
	printf("In thread mode (privileged): before interrupt\n");

#if 0
	/*Change privileged*/
	change_access_level_unpriv();

	/*This moment: Thread with unprivileged mode. We cannot access to register
	 	 	 	   privileged like a software interrupts registers
	  	  	  	   in unprivileged mode, it produced a hardfault*/

	printf("In thread mode (unprivileged): before interrupt\n");

	/*Setup and trigger the interrupt, it will produce a Hardfault*/
#endif
	generate_interrupt();

	printf("In thread mode : after interrupt\n");

	for(;;);
}

/* This function(ISR) executes in HANDLER MODE of the processor */
void RTC_WKUP_IRQHandler(void)
{
	printf("In handler mode : ISR\n");
}

void HardFault_Handler(void)
{
	printf("Hard fault detected\n");
	while(1);
}
