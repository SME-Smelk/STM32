/**
  ******************************************************************************
  * @Project        : ARM_09-Interrupt_Priority
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-28-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Example of interrupt priority with TIMER2 and I2C IRQ handler
  * 				  software interrupt.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note:
  *
  * 	-For calculation IPRx (Interrupt Priority)
  * 		IPRx is a 32 bits register divided in 4 parts (32/4 = 8 bytes)
  * 		then IPR0 have 4 parts to IRQ0-3
  *
  * 			IRQ TIMER2 28
  * 			IPRx = 28/4= 7 (IPR7)
  * 			IPRx bit = x%4 * 8 for 1 IPR:
  * 				(8 bits of offset 0*8 = PRI_0)
  * 				(8 bits of offset 1*8 = PRI_1)
  * 				(8 bits of offset 1*8 = PRI_3)
  * 				(8 bits of offset 1*8 = PRI_4)
  *
  * 			IPR7 bit = 31%4 * 8 = 24 (bit 24-31)
  *
  ******************************************************************************
**/

#define IRQNO_TIMER2  28
#define IRQNO_I2C1    31

#include<stdint.h>
#include<stdio.h>

/* NVIC register addresses. Refer to the processor generic guide */
uint32_t *pNVIC_IPRBase =  (uint32_t*)0xE000E400;
uint32_t *pNVIC_ISERBase = (uint32_t*)0xE000E100;
uint32_t *pNVIC_ISPRBase = (uint32_t*)0XE000E200;


void configure_priority_for_irqs(uint8_t irq_no, uint8_t priority_value)
{
	/*1. find out IPRx */
	uint8_t iprx = irq_no / 4;
	uint32_t *ipr =  pNVIC_IPRBase+iprx;

	/*2. position in iprx*/
	uint8_t pos = (irq_no % 4) * 8;

	/*3. configure the priority*/
	/*Clear priority*/
	*ipr &= ~(0xFF << pos);
	/*Set priority value*/
	*ipr |=  (priority_value << pos);

}

int main(void)
{
	/*1. Lets configure the priority for the peripherals*/
	 configure_priority_for_irqs(IRQNO_TIMER2,0x80);
	 configure_priority_for_irqs(IRQNO_I2C1,0x80);

	/*2. Set the interrupt pending bit in the NVIC PR*/
	 *pNVIC_ISPRBase |= ( 1 << IRQNO_TIMER2);

	/*3. Enable the IRQs in NVIC ISER. It will trigger immediately the
	     interrupt for priority*/
	 *pNVIC_ISERBase |= ( 1 << IRQNO_I2C1);
	 *pNVIC_ISERBase |= ( 1 << IRQNO_TIMER2);


}

//isrs

void TIM2_IRQHandler(void)
{
  printf("[TIM2_IRQHandler]\n");
  /*Here, we are pending I2C interrupt request manually */
  *pNVIC_ISPRBase |= ( 1 << IRQNO_I2C1);
  while(1);

}


void I2C1_EV_IRQHandler(void)
{
  printf("[I2C1_EV_IRQHandler]\n");
}
