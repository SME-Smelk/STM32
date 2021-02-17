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
    - [x] 16-ADC-contConv_IT
    - [x] 17-ADC-DMA
- [ ] DMA
    - [ ] DMA_ADC-SRAM
    - [ ] DMA_Interrupt-ToggleLed
    - [ ] DMA_UART_SRAM
    - [ ] DMA_SRAM-UART2-TX
- [ ] LowPower Modes
    - [ ] WFE Button
    - [ ] WFI
    - [ ] Run Mode
    - [ ] Sleep Mode
    - [ ] Standby Mode
    - [ ] Stop Mode
    - [ ] Sleep On Exit
- [ ] Internal RTC
    - [ ] Alarm_Stanby_WakeUp
    - [ ] Alarm_A
    - [ ] Alarm_A_12
    - [ ] Date_time
    - [ ] Demo_app
    - [ ] Print_TimeInfo
    - [ ] TS_TIMER
- [ ] Communication Protocols
	- [x] UART
	- [ ] I2C
		- [ ] Master_TX
		- [ ] Master_RX
		- [ ] Master_RX_TI
		- [ ] Slave_TX
	- [ ] SPI
		- [ ] Master_TX
		- [ ] Master_RX
		- [ ] Master_RX_TI
		- [ ] Slave_TX
	- [ ] CAN
		- [ ] LoopBack
		- [ ] LoopBack_IT
		- [ ] Normal_mode_twonodes

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

### Bootloaders

- [ ] Bootloader
    - [ ] Bootloader Base Code
    - [ ] Test-ToggleLed-STBootloader
    - [ ] Bootloader-UART
    - [ ] UserCode
    - [ ] Bootloader-Code

### Cortex M4 Programming

- [ ] Cortex-M4
    - [ ] --

