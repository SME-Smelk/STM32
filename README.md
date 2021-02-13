# STMicroelectronics microcontroller repository
### Features

In the process of learning and generating a personal repository as well as friendly to the community, this section will host codes on the use of STM32 microcontrollers, but focused on STM32F407VG, for now. 

- In process of code programming
- Basic code support and exaples of STM32 microcontrollers and AC, DMA, UART, SPI, I2C, CAN, low power, interrupts, etc.
- Focus in DISCOVERY-DISC1 F407VG Board
- Support  STM32 and Cortex Mx architecture
- Focus in FreeRTOS
- Memory Management with processor Cortex-M4 and assembly.
- Multiples projects code to use in basic electronic, avance, robotics and rtos projects.
---

![](https://i.ibb.co/SmH8YWk/SME-trans.png)

---

###DISCOVERY-DISC1 STM32F407VG

The DISCOVERY-DISC1 F407VG board has the following code repository the following plan, at the moment. This list could change**

#### Basics

- [ ] UART2
    - [ ] 01-TX
    - [ ] 02-TX_RX
    - [ ] 03-TX_RX_IT
- [ ] SystemClock
    - [ ] 04-SYSCLK_HSI_MCO
    - [ ] 05-SYSCLK_PLL_HSI_MCO
- [ ] GPIO
    - [ ] 06-GPIO_LedBlink_Block
    - [ ] 07-GPIO_LedBlink_NoBlock
    - [ ] 07-GPIO_LedBlink_NoBlock_X
    - [ ] 08-GPIO_Button_Led
    - [ ] 09-GPIO_Button_Led_IT
- [ ] DMA - ADC
    - [ ] ADC_Poll
    - [ ] DMA_Interrupt-ToggleLed
    - [ ] DMA-UART_SRAM
    - [ ] DMA-ADC-SRAM
    - [ ] DMA-SRAM-UART2-TX
- [ ] TIMER
    - [ ] 13-TIMER2_IT_100ms
    - [ ] 14-TIMER2_PWM_1us
    - [ ] 15-TIMER2_IC
    - [ ] 16-TIMER2_IC_OC
- [ ] Communication Protocols
    - [ ] UART
    - [ ] I2C
    - [ ] SPI
    - [ ] CAN
    - [ ] I2S

#### Sensors

- [ ] RTC
    - [ ] DS1307
    - [ ] DS3231
- [ ] Ultrasonics
	- [ ] HC-SR04
- [ ] IMU
	- [ ] MPU6050

#### Aplications

- [ ] With Interfaces
    - [ ] ADC-FFT
    - [ ] GPS Communication
	- [ ] IMU/AHRS MPU6050
- [ ] Signal Procesing
	- [ ] DSP
	- [ ] Filters
	
#### RTOS

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

#### Bootloaders

- [ ] Bootloader
    - [ ] Bootloader Base Code
    - [ ] Test-ToggleLed-STBootloader
    - [ ] Bootloader-UART
    - [ ] UserCode
    - [ ] Bootloader-Code

#### Cortex M4 Programming

- [ ] Cortex-M4
    - [ ] --

