/**
  ******************************************************************************
  * @Project        : ARM_11-Fault_Gen
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-28-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Example to analysis of frame stack with UsageFaults.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note:
  * 	-Capture the stack frame for analysis of faults.
  * 	-Use the UFSR register with Usage faults
  * 	 ARM Cortex M4 device generic guide: page 244/277.
  * 		DIVBYZERO
  * 		UNALIGNED
  *			NOCP
  *			INVPC
  *			INVSTATE
  *			UNDEFINSTR
  *
  ******************************************************************************
**/

#include<stdint.h>
#include<stdio.h>

void UsageFault_Handler_c(uint32_t *pMSP);

int fun_divide(int x , int y)
{

	return x/y;
}

int main(void)
{
	/*1. Enable all configurable exceptions : usage fault, mem manage fault and bus fault*/
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;

	/*Enable mem manage*/
	*pSHCSR |= ( 1 << 16);
	/*Enable bus fault*/
	*pSHCSR |= ( 1 << 17);
	/*Enable usage fault*/
	*pSHCSR |= ( 1 << 18);

#if 1
	/*1. Lets force the processor to execute some undefined instruction*/
		uint32_t *pSRAM = (uint32_t*)0x20010001;

		/*This is an undefined instruction value */
		*pSRAM = 0xFFFFFFFF;

		/* change PC to jump to location 0x20010001 with invalid value */
		void (*some_address) (void);
		some_address = (void*)0x20010001;
		some_address();
#endif

#if 0
    /*2- Initialize  the function pointer variable to some address with invalid T-Bit*/
		/* change PC to jump to location 0x20010001 */
		void (*some_address) (void);
		some_address = (void*)0x20010000;
		some_address();
#endif

#if 0
	/*3- enable divide by zero trap*/
		uint32_t *pCCR = (uint32_t*)0xE000ED14;
		*pCCR |= ( 1 << 4);
		fun_divide(10,0);
#endif
	/*4. analyze the fault*/

	for(;;);
}

//2. implement the fault handlers
void HardFault_Handler(void)
{
	printf("Exception : Hardfault\n");
	while(1);
}


void MemManage_Handler(void)
{
	printf("Exception : MemManage\n");
	while(1);
}

void BusFault_Handler(void)
{
	printf("Exception : BusFault\n");
	while(1);
}

#if 0
/*basic usage fault*/
void UsageFault_Handler(){
	uint32_t *pUFSR = (uint32_t*)0xE000ED2A;
	printf("Exception : UsageFault\n");
	printf("UFRS = %lx\n", (*pUFSR) & 0xFFFF);
	while(1);
}

#else

__attribute__ ((naked)) void UsageFault_Handler(void)
{
	//here we extracted the value of MSP which happens to be the
	//base address of the stack frame(thread mode) which got saved during the exception entry
	//from thread mode to handler mode
	__asm ("MRS r0,MSP");
	__asm ("B UsageFault_Handler_c");
}

void UsageFault_Handler_c(uint32_t *pBaseStackFrame)
{
	uint32_t *pUFSR = (uint32_t*)0xE000ED2A;
	printf("Exception : UsageFault\n");
	printf("UFSR = %lx\n",(*pUFSR) & 0xFFFF);
	printf("pBaseStackFrame = %p\n",pBaseStackFrame);
	printf("Value of R0 = %lx\n", pBaseStackFrame[0]);
	printf("Value of R1 = %lx\n", pBaseStackFrame[1]);
	printf("Value of R2 = %lx\n", pBaseStackFrame[2]);
	printf("Value of R3 = %lx\n", pBaseStackFrame[3]);
	printf("Value of R12 = %lx\n", pBaseStackFrame[4]);
	printf("Value of LR = %lx\n", pBaseStackFrame[5]);
	printf("Value of PC = %lx\n", pBaseStackFrame[6]);
	printf("Value of XPSR = %lx\n", pBaseStackFrame[7]);
	while(1);
}
#endif
