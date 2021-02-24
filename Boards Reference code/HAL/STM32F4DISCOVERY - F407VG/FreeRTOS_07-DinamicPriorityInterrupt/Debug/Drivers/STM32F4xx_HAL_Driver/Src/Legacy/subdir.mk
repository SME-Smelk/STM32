################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32F4xx_HAL_Driver/Src/Legacy/stm32f4xx_hal_can.c 

OBJS += \
./Drivers/STM32F4xx_HAL_Driver/Src/Legacy/stm32f4xx_hal_can.o 

C_DEPS += \
./Drivers/STM32F4xx_HAL_Driver/Src/Legacy/stm32f4xx_hal_can.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F4xx_HAL_Driver/Src/Legacy/stm32f4xx_hal_can.o: ../Drivers/STM32F4xx_HAL_Driver/Src/Legacy/stm32f4xx_hal_can.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Core/Src" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/SEGGER/Config" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/SEGGER/OS" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/SEGGER/Patch" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/SEGGER/SEGGER" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Core/Inc/Peripherals" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Core/Src/Peripherals" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/FreeRTOS/org/Source/include" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/FreeRTOS/org/Source/portable/MemMang" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/CMSIS/Device" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/CMSIS/Include" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/STM32F4xx_HAL_Driver/Src" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/STM32F4xx_HAL_Driver/Src/Legacy" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/Legacy/stm32f4xx_hal_can.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

