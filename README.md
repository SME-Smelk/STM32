# STMicroelectronics Microcontroller Repository
### Features

In the process of learning and generating a personal repository as well as friendly to the community. This section will host codes on the use of STM32 microcontrollers, but focused on STM32F407VG, for now. 

- In process of code programming
- Basic code support and exaples of STM32 microcontrollers and AC, DMA, UART, SPI, I2C, CAN, low power, interrupts, etc.
- Focus in DISCOVERY-DISC1 F407VG Board
- Support  STM32 and Cortex Mx architecture
- Focus in FreeRTOS
- Memory Management with processor Cortex-M4 and assembly.
- Multiples projects code to use in basic electronic, advance, robotics and rtos projects.
- Own Drivers SME for pherypherals

#




![](https://i.postimg.cc/SsDPs73D/Sin-t-tulo-2.png)




#




# Board Reference Code: Basics

## DISCOVERY-DISC1 STM32F407VG

The DISCOVERY-DISC1 F407VG board has the following code repository the following plan, at the moment. This list could change**

- [x] UART2
    - [x] 01-UART2_HelloWord
    - [x] 02-UART2_TXRX
    - [x] 03-UART2_TXRX_IT
- [x] SystemClock
    - [x] 04-SYSCLK_HSI_MCO
    - [x] 05-SYSCLK_PLL_HSE_MCO
- [x] GPIO
    - [x] 06-GPIO_LedBlink_Block
    - [x] 07-GPIO_LedBlink_NoBlock
    - [x] 08-GPIO_Button_Led
    - [x] 09-GPIO_Button_Led_IT
- [x] TIMER
    - [x] 10-TIMER2_IT_100ms
    - [x] 11-TIMER2_PWM_1us
    - [x] 12-TIMER2_IC
    - [x] 13-TIMER2_IC_OC
- [x] ADC
    - [x] 14-ADC_singleConv_poll
    - [x] 15-ADC_contConv_poll_tempSen
    - [x] 16-ADC_contConv_IT
    - [x] 17-ADC_DMA
- [x] DMA
    - [x] 17-ADC_DMA (previously)
    - [x] 18-DMA_M2M_Poll-ToggleLed
    - [x] 19-DMA_M2M_ToggleLed_IT
    - [x] 20-DMA_P2M_UART2-RX_SRAM
    - [x] 21-DMA_M2P_SRAM_UART2-TX
- [x] LowPower Modes
    - [x] 22-LowPower_SleepOnExit_Feature
    - [x] 23-LowPower_WFI_Instruction
    - [x] 24-LowPower_WFE_Instruction
    - [x] 25-LowPower_modes
    - [x] 26-LowPower_BackupSRAM_Standby
- [x] Internal RTC
    - [x] 27-RTC_DateTime
    - [x] 28-RTC_DateTime_Standby (NOT TESTED: X3 LSE not provided in board)
    - [x] 29-RTC_Alarm_A
    - [x] 30-RTC_Alarm_Stanby_WakeUp (NOT TESTED: X3 LSE not provided in board)
    - [x] 31-RTC_Demo_APIs
- [ ] Communication Protocols
	- [x] UART
	- [x] I2C
		- [x] 32-I2C_Master_Slave_TX_RX_AllOne
			- [x] Master Write - Slave Read
			- [x] Master Read - Slave Write
	- [ ] SPI
		- [ ] Master_TX
		- [ ] Master_RX
		- [ ] Master_RX_TI
		- [ ] Slave_TX
	- [ ] CAN
		- [ ] LoopBack
		- [ ] LoopBack_IT
		- [ ] Normal_mode_twonodes

### Bootloaders

- [ ] Bootloader
    - [ ] Bootloader Base Code
    - [ ] Test-ToggleLed-STBootloader
    - [ ] Bootloader-UART
    - [ ] UserCode
    - [ ] Bootloader-Code

### RTOS

- [ ] RTOS FreeRTOS
    - [ ] FreeRTOS Code Base
    - [ ] FreeRTOS SystemView SEGGER Code Base
    - [ ] CreateTask 4 leds
    - [ ] UART2
    - [ ] LedButton
    - [ ] LedButtonIT
    - [ ] Task-Notify
    - [ ] TaskDelete
    - [ ] DinamicPriorityInterrupt
    - [ ] TaskDelay
    - [ ] Hook-PowerSaving
    - [ ] Queue-Command
    - [ ] Bin_sema_taks
    - [ ] Count-Sema-Taks
    - [ ] Mutex-BinSema
    - [ ] MutexAPI

### Cortex M4 Programming

- [ ] Cortex-M4
    - [ ] --

### Other pherypherals and sensors
- [ ] SD 
    - [ ] write_sd
    - [ ] read_sd
- [ ] RTC
    - [ ] DS1307
    - [ ] DS3231
- [ ] Ultrasonics
	- [ ] HC-SR04
- [ ] IMU
	- [ ] MPU6050

### Aplications

- [ ] With Interfaces
    - [ ] GPS Communication
	- [ ] IMU/AHRS MPU6050
- [ ] Signal Procesing
	- [ ] ADC-FFT
	- [ ] DSP
	- [ ] Filters
	
