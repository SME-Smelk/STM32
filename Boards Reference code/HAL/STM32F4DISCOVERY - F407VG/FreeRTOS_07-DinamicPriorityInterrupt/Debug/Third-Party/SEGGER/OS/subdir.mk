################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-Party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c 

OBJS += \
./Third-Party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.o 

C_DEPS += \
./Third-Party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.d 


# Each subdirectory must supply rules for building sources it contributes
Third-Party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.o: ../Third-Party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Core/Src" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/SEGGER/Config" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/SEGGER/OS" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/SEGGER/Patch" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/SEGGER/SEGGER" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Core/Inc/Peripherals" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Core/Src/Peripherals" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/FreeRTOS/org/Source/include" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Third-Party/FreeRTOS/org/Source/portable/MemMang" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/CMSIS/Device" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/CMSIS/Include" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/STM32F4xx_HAL_Driver/Src" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Drivers/STM32F4xx_HAL_Driver/Src/Legacy" -I"C:/Users/Ismael/git/STM32/Boards Reference code/HAL/STM32F4DISCOVERY - F407VG/FreeRTOS_07-DinamicPriorityInterrupt/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Third-Party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

