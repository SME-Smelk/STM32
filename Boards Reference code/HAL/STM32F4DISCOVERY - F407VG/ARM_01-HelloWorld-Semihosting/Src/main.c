/**
  ******************************************************************************
  * @Project        : ARM_01-HelloWorld-Semihosting
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-27-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Example of program with debugs message via semihosting.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*none
  *
  ******************************************************************************
**/

#include<stdio.h>

extern void initialise_monitor_handles(void);

int main(void)
{
	/* This message is going to be displayed via OpenOCD semihosting.
	 * Make sure the message terminates with '\n' character.
	 * set the linker arguments
	 * 		-specs=rdimon.specs -lc -lrdimon
	 * Add semihosting enable command of the openOCD
	 * 		monitor arm semihosting enable
	 * Add the below function call and prototype to main.c
	 * 		extern void initialise_monitor_handles(void);
	 * 		initialise_monitor_handles();
	 * remove syscalls.c from the project
	 */

	initialise_monitor_handles();

	printf("Hello world\n");

	for(;;);
}
