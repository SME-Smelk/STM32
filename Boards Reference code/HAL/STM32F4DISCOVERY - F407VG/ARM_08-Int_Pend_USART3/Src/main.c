/**
  ******************************************************************************
  * @Project        : ARM_08-Int_Pend_USART3
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-28-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Set the pending bit manually for UART3.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note:
  * 	-For calculation ISPRx and bit to modified:
  * 		USART3 | IRQ39
  * 		ISPR0-7, 32bits for register.
  * 		ISPR0 for 0xE000E200
  *			ISPR1 for 0xE000E204
  *
  * 		ISPRx = NUMBER_NVIC/32 = 39/32 = 1 (ISPR1)
  * 		ISPR1[y] bit to modified = NUMBER_NVIC%32 = 39%32 = 7 (bit 7)
  *
  ******************************************************************************
**/


#include<stdint.h>
#include<stdio.h>

#define USART3_IRQNO 39

int main(void)
{
	/*1. Manually pend the pending bit for the USART3 IRQ number in NVIC*/
	uint32_t *pISPR1 = (uint32_t*)0XE000E204;
	*pISPR1 |= ( 1 << (USART3_IRQNO % 32));

	/*2. Enable the USART3 IRQ number in NVIC (Immediately the interrupt trigger)*/
	uint32_t *pISER1 = (uint32_t*)0xE000E104;
	*pISER1 |= ( 1 << (USART3_IRQNO % 32));

	for(;;);
}

//USART3 ISR
void USART3_IRQHandler(void)
{
	printf("in USART3 isr\n");
}
