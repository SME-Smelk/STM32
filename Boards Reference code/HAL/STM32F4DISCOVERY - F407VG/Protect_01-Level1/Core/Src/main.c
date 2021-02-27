/**
  ******************************************************************************
  * @Project        : Protect_01-Level1
  * @Autor          : Ismael Poblete
  * @Company		: -
  * @Date         	: 02-27-2021
  * @Target			: DISCOVERY-DISC1 STM32F407VG
  * @brief          : Change the flash memory protection to level 1. Once time
  * 				  it program is execute, read the memory and in the Sector 2
  * 				  and try to write will produced a error. After that the
  * 				  debugger and any program will dont could read the MCU.
  * 				  Change a Level 0 can use Stm32CubeProgrammer or ST-Utility.
  * 				  Once time the level 1 is set, debugger must be disconnected
  * 				  (Put off jumpers ST-Link of Discovery board)
  *
  * @Lib			: CMSIS, HAL.
  * @System Clock
  * 	SYSSource:		PLL(HSE)
  * 	SYSCLK: 		84MHz
  * 	RTCSource:		None
  * 	RTCCLK: 		None
  * @Perf
  * 	*UART2
  * 		PA2			<-----> USART_TX
  * 		PA3			<-----> USART_RX
  * @note
  *		-We can modified the write protection for sections, but read protection (RBD)
  *		 Only affect all sections.
  * 	-See 3.3-"Flash module organization" to observe Sector of the memory
  * 	-See 3.7-Option Bytes for section of configurations bytes. STM32 devices
  * 	include a set of user configurable option bytes that can control features
  * 	such as HW / SW watchdog, read protection, and write protection.
  *		-RDP Read protection.
  * 	-RPD Protections levels are 0, 1 and 2. It protect against read or write memory.
  * 		-0: No protection
  * 		-1: Reversible Protection, it can return back to level 0, but all memory
  * 		    protected will be erased. Debug limited
  * 		-2: Permanent Protection. Use the level 2 means that the system cannot
  * 		    be used to read or write again and there is no going back.
  * 		    Debug and boot RAM is disable
  * 	-Sectors
  * 		Name 			Block base addresses 			Size
  * 	    Sector 0  		0x0800 0000 - 0x0800 3FFF 		16 Kbytes
  *			Sector 1  		0x0800 4000 - 0x0800 7FFF 		16 Kbytes
  *			Sector 2  		0x0800 8000 - 0x0800 BFFF 		16 Kbytes
  *			Sector 3  		0x0800 C000 - 0x0800 FFFF 		16 Kbytes
  *			Sector 4  		0x0801 0000 - 0x0801 FFFF 		64 Kbytes
  *			Sector 5  		0x0802 0000 - 0x0803 FFFF 		128 Kbytes
  *			Sector 6  		0x0804 0000 - 0x0805 FFFF 		128 Kbytes
  *			Sector 7  		0x0806 0000 - 0x0807 FFFF 		128 Kbytes
  *			Sector 8  		0x0808 0000 - 0x0809 FFFF 		128 Kbytes
  *			Sector 9  		0x080A 0000 - 0x080B FFFF 		128 Kbytes
  *			Sector 10 		0x080C 0000 - 0x080D FFFF 		128 Kbytes
  *			Sector 11		0x080E 0000 - 0x080F FFFF 		128 Kbytes
  *
  * 	-Write in OPTCR 16-27 it enable the write protection and activate the
  * 	 Proprietary code read-out protection (PCROP) putting the system in
  * 	 level 1, we cannot read.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void USART2_Init(void);

void printmsg(char *format,...);

uint8_t get_flash_rdp_level(void);
uint8_t configure_flash_sector_w_protection(uint8_t sector_details, uint8_t protection_mode);
uint8_t configure_all_sector_w_protection(uint8_t disable);
void configure_read_levelone_protection();
/* Private user code ---------------------------------------------------------*/
UART_HandleTypeDef huart2;


#define BL_RX_LEN  200
uint8_t bl_rx_buffer[BL_RX_LEN];

uint8_t rdp_level = 0x00;


/* Sectors number 0-11 STM32F407 */
#define SECTOR_NUMBER 2

/* Protections Levels 0, 1, 2.
 * 2: for danger has been comment. not available.
 * */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
    USART2_Init();

	/* User Code */
    printmsg("Protections levels:\n");
    printmsg("   Level 0: 0xaa   (Hex)\n");
    printmsg("   Level 1: Others (Hex)\n");
    printmsg("   Level 2: 0xcc   (Hex)\n");

    /* Read protection level */
    rdp_level = get_flash_rdp_level();
    printmsg("Read Protection: ");
    printmsg("Level %#x\n",rdp_level);

    /* Change the write protection level */
    printmsg("\nChanging to write protection Sector 2.\n");
    configure_flash_sector_w_protection(SECTOR_NUMBER, (uint8_t) 1);

    //printmsg("\nChanging to write protection all Sectors.\n");
    //configure_all_sector_w_protection(ENABLE);

    printmsg("\nChanging to read protection to Level 1.\n");
    configure_read_levelone_protection();

    // Read protection level
    rdp_level = get_flash_rdp_level();
    printmsg("Read Protection: ");
    printmsg("Level %#x\n",rdp_level);

	while (1);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void USART2_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

/* prints formatted string to console over UART */
 void printmsg(char *format,...)
{
	char str[80];

	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, format);
	vsprintf(str, format,args);
	HAL_UART_Transmit(&huart2,(uint8_t *)str, strlen(str),HAL_MAX_DELAY);
	va_end(args);
 }

 /*This function reads the RDP ( Read protection option byte) value
  *For more info refer "Table 9. Description of the option bytes" in stm32f446xx RM
  */
 uint8_t get_flash_rdp_level(void)
 {

 	uint8_t rdp_status=0;
 #if 0
 	FLASH_OBProgramInitTypeDef  ob_handle;
 	HAL_FLASHEx_OBGetConfig(&ob_handle);
 	rdp_status = (uint8_t)ob_handle.RDPLevel;
 #else

 	 volatile uint32_t *pOB_addr = (uint32_t*) 0x1FFFC000;
 	 rdp_status =  (uint8_t)(*pOB_addr >> 8) ;
 #endif

 	return rdp_status;

 }

 /*
 Modifying user option bytes
 To modify the user option value, follow the sequence below:
 1. Check that no Flash memory operation is ongoing by checking the BSY bit in the
 FLASH_SR register
 2. Write the desired option value in the FLASH_OPTCR register.
 3. Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
 4. Wait for the BSY bit to be cleared.
 */
 uint8_t configure_flash_sector_w_protection(uint8_t sector_details, uint8_t protection_mode)
 {

 //Flash option control register (OPTCR)
 volatile uint32_t *pOPTCR = (uint32_t*) 0x40023C14;

 	 if(protection_mode == (uint8_t) 0)
	{

		//disable all r/w protection on sectors

		//Option byte configuration unlock
		HAL_FLASH_OB_Unlock();

		//wait till no active operation on flash
		while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

		/*clear the 31st bit (default state) STM32F411RE
		please refer : Flash option control register (FLASH_OPTCR) in RM
		*pOPTCR &= ~(1 << 31);*/

		//clear the protection : make all bits belonging to sectors as 1
		*pOPTCR |= ((uint32_t)sector_details << 16);

		//Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
		*pOPTCR |= ( 1 << 1);

		//wait till no active operation on flash
		while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

		HAL_FLASH_OB_Lock();


	} else{

	   if(protection_mode == (uint8_t) 1)
	   {
			//we are putting write protection on the sectors encoded in sector_details argument

			//Option byte configuration unlock
			HAL_FLASH_OB_Unlock();

			//wait till no active operation on flash
			while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

			/*clear the 31st bit (default state) STM32F411RE
			please refer : Flash option control register (FLASH_OPTCR) in RM
			*pOPTCR &= ~(1 << 31);*/

			//put write protection on sectors
			*pOPTCR &= ~ ((uint32_t)sector_details << 16);

			//Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
			*pOPTCR |= ( 1 << 1);

			//wait till no active operation on flash
			while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

			HAL_FLASH_OB_Lock();
		}
		/*
			else if (protection_mode == (uint8_t) 2)
		 {

			//Option byte configuration unlock
				HAL_FLASH_OB_Unlock();

				//wait till no active operation on flash
				while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

				//here wer are setting read and write protection for the sectors
				//set the 31st bit
				//please refer : Flash option control register (FLASH_OPTCR) in RM
				*pOPTCR |= (1 << 31);

				//put read and write protection on sectors
				 *pOPTCR &= ~(0xff << 16);
				*pOPTCR |= (sector_details << 16);

				//Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
				*pOPTCR |= ( 1 << 1);

				//wait till no active operation on flash
				while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

				HAL_FLASH_OB_Lock();

		 }
		*/
	}
 		return 0;
}

 void configure_read_levelone_protection(){
	 //Flash option control register (OPTCR)
	 volatile uint32_t *pOPTCR = (uint32_t*) 0x40023C14;

	//Option byte configuration unlock
	HAL_FLASH_OB_Unlock();

	//wait till no active operation on flash
	while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

	//clear the protection : make all bits belonging to sectors as 1
	*pOPTCR |= (0xbb << 8);

	//Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
	*pOPTCR |= ( 1 << 1);

	//wait till no active operation on flash
	while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

	HAL_FLASH_OB_Lock();

 }

uint8_t configure_all_sector_w_protection(uint8_t status)
{

	//Flash option control register (OPTCR)
	volatile uint32_t *pOPTCR = (uint32_t*) 0x40023C14;

	 if(status == DISABLE)
	{

		//disable all r/w protection on sectors

		//Option byte configuration unlock
		HAL_FLASH_OB_Unlock();

		//wait till no active operation on flash
		while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

		/*clear the 31st bit (default state) STM32F411RE
		please refer : Flash option control register (FLASH_OPTCR) in RM
		*pOPTCR &= ~(1 << 31);*/

		//clear the protection : make all bits belonging to sectors as 1
		*pOPTCR |= (0xfff << 16);

		//Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
		*pOPTCR |= ( 1 << 1);

		//wait till no active operation on flash
		while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

		HAL_FLASH_OB_Lock();

	} else{

		//we are putting write protection on the sectors encoded in sector_details argument

		//Option byte configuration unlock
		HAL_FLASH_OB_Unlock();

		//wait till no active operation on flash
		while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

		//put write protection on sectors
		*pOPTCR &= ~ (0xfff << 16);

		//Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
		*pOPTCR |= ( 1 << 1);

		//wait till no active operation on flash
		while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

		HAL_FLASH_OB_Lock();

	}
	return 0;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	while(1);
}

