/**
  ******************************************************************************
  * @Project        : ARM_05-T-bit
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-28-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : See the effect of T-Bit with a variable pointer function
  * 				  writing in PC a even and odd T-Bit value.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note:
  * 	-T-Bit:The T bit reflects the instruction set state: when the T bit is set,
  * 	 the processor executes in Thumb state. when the T bit is clear, the processor
  * 	 executes in ARM state. In ARM Cortex M, we don have a ARM state, put a clear
  * 	 bit result a fault.
  * 	-When the next instruction should be executed, this direction is store
  * 	 in the Program Counter (PC). It value should be odd, otherwise it will
  * 	 result in a hardfault.
  *
  * 	-Reading from PC shouldn't return an odd address. However when you write to PC,
  * 	 LSB of value is loaded into the EPSR T-bit.
  * 	 The value to read of R15 PC is always is even, for automatic function of the CPU.
  * 	 The following can clear the T bit to 0:
  * 	   *instructions BLX, BX and POP{PC}
  * 	   *restoration from the stacked xPSR value on an
  * 	   *exception return bit[0] of the vector value on an exception entry or reset.
  *
  * 	-The value to load in PC will be always odd.
  *
  ******************************************************************************
**/

#include<stdio.h>
#include<stdint.h>

/* This function executes in THREAD MODE of the processor */
void generate_interrupt()
{
	/*Store the register address*/
	uint32_t *pSTIR  = (uint32_t*)0xE000EF00;
	uint32_t *pISER0 = (uint32_t*)0xE000E100;

	/*enable IRQ3 interrupt*/
	*pISER0 |= ( 1 << 3);

	/*Set a software interrupt for IRQ3*/
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

	printf("In thread mode : before interrupt\n");


	/*Test a valid T-Bit, store a address in a pointer function and jump.*/

	/*Declare a function pointer variable*/
	void (*fun_ptr)(void);

	/*Store the address function in function pointer variable*/
	//en el puntero de funcion (fun_ptr)
	fun_ptr = change_access_level_unpriv;

	/*Execute the function in the address of fun_ptr. To execute the address.
	  Will store in PC (Program Counter) the next address to trigger.*/
	fun_ptr();


	/*Test store in PC a odd address to probe the T-Bit
	  Test store in PC a even address to probe the T-Bit.*/
    /*The value of R15 PC is always is even, for automatic function of the CPU*/

	/*even address*/
	/*fun_ptr = (void*)0x080001e9;*/
	/*The value to load in PC will be always odd, but we can read even via debugger
	   automatic EPSR of CPU, it return 0 even value.*/

	/*odd address: Hard Fault is produced*/
	fun_ptr = (void*)0x080001e8;

  /* Here the address 0x080001e8 gets copied into PC 
   * 0th bit of the address will be copied into T bit.
   *Since the 0th bit is 0, T bit becomes 0 which raises processor fault */
	fun_ptr();

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
