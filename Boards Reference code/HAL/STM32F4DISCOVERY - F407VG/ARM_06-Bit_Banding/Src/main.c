/**
  ******************************************************************************
  * @Project        : ARM_06-Bit_Banding
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-28-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Allocate a alias address to the bit 8 of pointer. Modified
  * 				  the alias_address modified only the bit 8 of the pointer.
  * @Lib			: -
  * @System Clock
  * 	SYSSource:		HSI
  * 	SYSCLK: 		16MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf: none
  * @note:
  * 	-We can use a bit like a flag in a register or variable we use clear or set
  *		 via bitwise bit, but we can use bit banding form to manipulate the bit
  *		 assigning it as address it called like a alias_address.
  *
  * 	-Bit Banding complete word of memory onto a single bit in the bit-band region.
  * 	 Writing to one of the alias words sets or clears the corresponding bit in the
  * 	 bit-band region. This enables every individual bit in the bit-banding region
  * 	 to be directly accessible from a word-aligned address using a single LDR
  * 	 instruction.
  *
  * 	-We use the formula:
  * 	 Address to use for represent a bit: 0x20000200 (create a pointer) bit_band_memory_addr
  * 	 From STM32F407VG (ref: 2.2 Cortex M4 Device Generic User Guide page 25/277):
  * 	 	alias_base = 0x22000000U
  * 	 	bit_band_base = 0x20000000U
  *
  * 	 alias_address = alias_base + (32* (bit_band_memory_addr - bit_band_base)) + bit * 4
  *		 alias_addres = 0x22000000 + (32 * ( 0x20000200 - 0x20000000) ) + 7 * 4
  *		 alias_addres = 0x22000000 + 0x20 * (0x200) + 0x1C
  *		 alias_addres = 0x22000000 + 0x401C
  *		 alias_addres = 0x2200401c
  ******************************************************************************
**/


#include <stdint.h>

/*From Cortex-M4 Device Generic User Guide*/
#define ALIAS_BASE   0x22000000U
#define BITBAND_BASE 0x20000000U

int main(void)
{


	/*Pointer address, and address by user*/
	uint8_t *ptr = (uint8_t*)0x20000200;

	/*Put a value in the pointer address*/
	*ptr = 0xff;

#if 0
	/*normal method*/
	/*clearing 7th bit position*/
	*ptr &= ~( 1 << 7);

	/*reset to 0xff*/
	*ptr = 0xff;
#endif

	/*bit band method*/
	uint8_t *alias_addr = (uint8_t*) (ALIAS_BASE+(32 * (0x20000200-BITBAND_BASE))+ 7 * 4);

	/*Only clearing 7th bit with address 0x20000200 of *ptr*/
	*alias_addr = 0;

	/* Set bit 8 */
	*alias_addr = 1;

	for(;;);
}
