/**
  ******************************************************************************
  * @Project        : ARM_02-OperationMode
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-28-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Example to see the Operation modes Thread and Handler.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note:
  * 	-Ref to Cortex M4 Devices Generic User Guide
  * 	-To check the operation modes thread or handler it can be appreciated
  * 	 In General Registers with debug mode.
  * 	 In the xpsr (Program Status Register). APSR, IPSR, EPSR.
  * 	 In  IPSR (Interrupt program register) contain the ISR_NUMBER[8]
  *		 of the actual interrupt. (Ref. Page: 17/277)
  *
  *		 IRQ_number for IRQ0 is 16. (Ref. Page: 19/277)
  *		 For RTC_WKUP IRQ_number is 19 (decimal).
  *		 If ISR_NUMBER[8-0] = 0x00 Thread mode
  *		 If ISR_NUMBER[8-0] != 0x00 Handler mode
  *
  ******************************************************************************
**/

#include<stdio.h>
#include<stdint.h>

/* This function executes in THREAD MODE of the processor */
void generate_interrupt()
{
	/* I keep the address of ISER register. (ISER enables the interrupts) */
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	/* I keep the address of STIR register. (STIR sets the software interrupts) */
	uint32_t *pSTIR  = (uint32_t*)0xE000EF00;

	/* Enable the interrupt IRQ3 of  RTC_WKUP*/
	*pISER0 |= ( 1 << 3);

	/* Set the software interrupt for IRQ3 */
	*pSTIR = (3 & 0x1FF);
}

/* This function executes in THREAD MODE of the processor */
int main(void)
{
	/* Start Thread mode */


	printf("In thread mode : before interrupt\n");

	/* Configure and trigger software interrupt via IRQ for Handler mode. */
	/* Before interrupt. Value xpsr[8-0] = 0x1000000 (Thread mode, bit xpsr[8-0] = 0x00)
	 * ISR_NUMBER[8-0] = 0, Thread mode.
	 * */
	generate_interrupt();

	/*after of the interrupt it return to the main in mode thread*/
	printf("In thread mode : after interrupt\n");

	for(;;);
}

/* This function(ISR) executes in HANDLER MODE of the processor */
void RTC_WKUP_IRQHandler(void)
{
	/* In interrupt. Value xpsr[8-0] = 0x1000013 (Handler mode, bit xpsr[8-0] = 0x13)*/
	printf("In handler mode : ISR\n");
}

