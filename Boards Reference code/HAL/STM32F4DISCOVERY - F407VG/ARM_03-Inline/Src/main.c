/**
  ******************************************************************************
  * @Project        : ARM_03-Inline
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-28-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Use assembly and inline for use memory between C and assembly.
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



int main(void)
{



#if 1
/*Example 1. Use assembly inline
* Load two values in the memory, add them and save in memory with inline assembly
*/
	/*Initialize address in R1*/
	__asm volatile("LDR R1,=#0x20001000");
	/*Initialize address in R2*/
	__asm volatile("LDR R2,=#0x20001004");

	/*Put a value in the address of R1 and R2 for the sum.*/

	/*Keep the R1 value in R0*/
	__asm volatile("LDR R0,[R1]");
	/*Keep the R2 value in R1*/
	__asm volatile("LDR R1,[R2]");
	/*Add R0 with R1, the result is save in R0*/
	__asm volatile("ADD R0,R0,R1");
	/*Keep the value memory of R0 to R2 address. Only the R2 value was unchanged
	  it keeps the address value.*/
	__asm volatile("STR R0,[R2]");

#endif

/*Example 2. Use of assembly with C expressions and constraint.
* Read the especial register CONTROL, write variable C in R0, pointer read.
*/

	/*Load c variable to assembly (inline input form "instruct:output:input")*/
	/*Write the variable c "val" in R0 register*/
	int val=50;
	__asm volatile("MOV R0,%0": :"r"(val));

	/*Store assembly register in c variable (inline output form ":")*/
	/*Read the processor register value of "CONTROL" and write it in variable c "control_reg".*/
	int control_reg;
	__asm volatile("MRS %0,CONTROL": "=r"(control_reg) );

	/*Store and Load (inline output and input form "::")*/
    /*Read the actual value store in p2 address (put a value manually) and then it will put that value in p1*/
	int p1, *p2;
	/* Initialize a address for *p2*/
	p2 = (int*)0x20000008;
	__asm volatile("LDR %0,[%1]": "=r"(p1): "r"(p2));

	for(;;);
}
