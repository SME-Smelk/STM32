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
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/ARM Cortex M Microcontroller DMA Programming Demystified/SME/Programaciones/00-Ref-F407VG/Core/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/ARM Cortex M Microcontroller DMA Programming Demystified/SME/Programaciones/00-Ref-F407VG/Drivers/CMSIS/Device" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/ARM Cortex M Microcontroller DMA Programming Demystified/SME/Programaciones/00-Ref-F407VG/Drivers/CMSIS/Include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/ARM Cortex M Microcontroller DMA Programming Demystified/SME/Programaciones/00-Ref-F407VG/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/ARM Cortex M Microcontroller DMA Programming Demystified/SME/Programaciones/00-Ref-F407VG/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/ARM Cortex M Microcontroller DMA Programming Demystified/SME/Programaciones/00-Ref-F407VG/Drivers/STM32F4xx_HAL_Driver/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/ARM Cortex M Microcontroller DMA Programming Demystified/SME/Programaciones/00-Ref-F407VG/Drivers/STM32F4xx_HAL_Driver/Src/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/ARM Cortex M Microcontroller DMA Programming Demystified/SME/Programaciones/00-Ref-F407VG/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/STM32F4xx_HAL_Driver/Src/Legacy/stm32f4xx_hal_can.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

