################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2/cmsis_os2.c 

OBJS += \
./Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2/cmsis_os2.o 

C_DEPS += \
./Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2/cmsis_os2.d 


# Each subdirectory must supply rules for building sources it contributes
Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2/cmsis_os2.o: ../Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2/cmsis_os2.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Core/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Third-Party/FreeRTOS/org/Source/include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Third-Party/FreeRTOS/org/Source/portable/MemMang" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Drivers/CMSIS/Device" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Drivers/CMSIS/Include" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Drivers/STM32F4xx_HAL_Driver/Src" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Drivers/STM32F4xx_HAL_Driver/Src/Legacy" -I"C:/Users/Ismael/Dropbox/Smelk/Universidad y Conocimiento/Cursos/Electronica/Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)/SME/01-CreateTask-FreeRTOS/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2/cmsis_os2.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

