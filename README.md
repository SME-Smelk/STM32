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
    - [x] UART2_01-HelloWord
    - [x] UART2_02-TXRX
    - [x] UART2_03-TXRX_IT
- [x] SystemClock
    - [x] SYSCLK_01-HSI_MCO
    - [x] SYSCLK_02-PLL_HSE_MCO
- [x] GPIO
    - [x] GPIO_01-LedBlink_Block
    - [x] GPIO_02-LedBlink_NoBlock
    - [x] GPIO_03-Button_Led
    - [x] GPIO_04-Button_Led_IT
- [x] TIMER
    - [x] TIMER2_01-IT_100ms
    - [x] TIMER2_02-PWM_1us
    - [x] TIMER2_03-IC
    - [x] TIMER2_04-IC_OC
- [x] ADC
    - [x] ADC_01-singleConv_poll
    - [x] ADC_02-contConv_poll_tempSen
    - [x] ADC_03-contConv_IT
    - [x] ADC_04-DMA
- [x] DMA
    - [x] ADC_01-DMA (previously)
    - [x] DMA_02-M2M_Poll-ToggleLed
    - [x] DMA_03-M2M_ToggleLed_IT
    - [x] DMA_04-P2M_UART2-RX_SRAM
    - [x] DMA_05-M2P_SRAM_UART2-TX
- [x] LowPower Modes
    - [x] LowPower_01-SleepOnExit_Feature
    - [x] LowPower_02-WFI_Instruction
    - [x] LowPower_03-WFE_Instruction
    - [x] LowPower_04-modes
    - [x] LowPower_05-BackupSRAM_Standby
- [x] Internal RTC
    - [x] RTC_01-DateTime
    - [x] RTC_02-DateTime_Standby (NOT TESTED: X3 LSE not provided in board)
    - [x] RTC_03-Alarm_A
    - [x] RTC_04-Alarm_Stanby_WakeUp (NOT TESTED: X3 LSE not provided in board)
    - [x] RTC_05-Demo_APIs
- [x] Cyclic Redundancy Check (CRC)
    - [x] CRC_01-Checksum
- [x] Protection Modes (Read RDP - Write Protection)
    - [x] Protect_01-Level1
- [ ] Communication Protocols - Conectivity
	- [x] UART
	- [x] I2C
		- [x] I2C_01-Master_Slave_TX_RX_AllOne
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
	- [ ] ETH
	- [ ] FSMC
	- [ ] USB_OTG
		- [ ] USB_OTG_FS
		- [ ] USB_OTG_HS
	- [ ] USART
	- [ ] SDIO
- [ ] Multimedia
	- [ ] I2S
	- [ ] DCMI
- [ ] Security
	- [ ] RNG	
- [ ] others Middleware
	- [ ] FATFS
	- [ ] LIBJPEG
	- [ ] MBEDTLS
	- [ ] USB_HOST
- [ ] System Core
	- [ ] IWDG
	- [ ] WWDG
	- [ ] SYS
### RTOS

- [x] RTOS FreeRTOS
    - [x] FreeRTOS_00-SystemView
    - [x] FreeRTOS_00-Tasks
    - [x] FreeRTOS_00-Task-SemiHosting
    - [x] FreeRTOS_01-UART2
    - [x] FreeRTOS_02-SystemView-Cooperative
    - [x] FreeRTOS_03-LedButton
    - [x] FreeRTOS_04-Led-ButtonIT
    - [x] FreeRTOS_05-Task-Notify
    - [x] FreeRTOS_06-TaskDelete
    - [x] FreeRTOS_07-DinamicPriorityInterrupt
    - [x] FreeRTOS_08-TaskDelay
    - [x] FreeRTOS_09-Hook-PowerSaving
    - [x] FreeRTOS_10-Queue-Command
    - [x] FreeRTOS_11-Bin_Sema_Taks
    - [x] FreeRTOS_12-Count-Sema-Taks
    - [x] FreeRTOS_13-Mutex-BinSema
    - [x] FreeRTOS_14-MutexAPI
 
### Bootloaders

- [x] Bootloader
    - [x] Bootloader_00-BootCode
    - [x] Bootloader_01-BootCode (Need a host program. Protocols and others)
    - [x] Bootloader_01-UserCode

### Cortex M4 Programming

- [ ] Cortex-M4
    - [x] ARM_00-Ref
    - [x] ARM_01-HelloWorld-Semihosting
    - [x] ARM_02-OperationMode
    - [ ] ARM_03-Inline
    - [ ] ARM_04-Access_levels
    - [ ] ARM_05-T-bit
    - [ ] ARM_06-Bit_Banding
    - [ ] ARM_07-Stack
    - [ ] ARM_08-Int_Pend_USART3
    - [ ] ARM_09-Interrupt_PendBit_USART3
    - [ ] ARM_10-Interrupt_Priority
    - [ ] ARM_11-Exception_Entry_Exit
    - [ ] ARM_12-Fault_Gen
    - [ ] ARM_13-SVC_Number
    - [ ] ARM_14-SVC_Operation
    - [ ] ARM_15-Task_Scheduler
    - [ ] ARM_16-CMSIS_Task_Scheduler

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
	
