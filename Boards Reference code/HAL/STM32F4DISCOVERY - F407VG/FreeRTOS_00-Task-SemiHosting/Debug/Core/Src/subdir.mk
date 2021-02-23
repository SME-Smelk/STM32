################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/main.o: ../Core/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Core/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/portable/MemMang" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/CMSIS/Device" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/CMSIS/Include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Src/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32f4xx_hal_msp.o: ../Core/Src/stm32f4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Core/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/portable/MemMang" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/CMSIS/Device" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/CMSIS/Include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Src/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32f4xx_it.o: ../Core/Src/stm32f4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Core/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/portable/MemMang" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/CMSIS/Device" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/CMSIS/Include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Src/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/system_stm32f4xx.o: ../Core/Src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Core/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Third-Party/FreeRTOS/org/Source/portable/MemMang" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/CMSIS/Device" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/CMSIS/Include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Drivers/STM32F4xx_HAL_Driver/Src/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/Programaciones/02-CreateTask-FreeRTOS-SemiHosting/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/system_stm32f4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
