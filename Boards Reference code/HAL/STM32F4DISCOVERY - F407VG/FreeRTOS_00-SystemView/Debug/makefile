################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Third-Party/SEGGER/SEGGER/subdir.mk
-include Third-Party/SEGGER/OS/subdir.mk
-include Third-Party/SEGGER/Config/subdir.mk
-include Third-Party/FreeRTOS/org/Source/portable/MemMang/subdir.mk
-include Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Third-Party/FreeRTOS/org/Source/Debug/Third-Party/FreeRTOS/org/Source/portable/MemMang/subdir.mk
-include Third-Party/FreeRTOS/org/Source/Debug/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Third-Party/FreeRTOS/org/Source/Debug/Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2/subdir.mk
-include Third-Party/FreeRTOS/org/Source/Debug/Third-Party/FreeRTOS/org/Source/subdir.mk
-include Third-Party/FreeRTOS/org/Source/Debug/Startup/subdir.mk
-include Third-Party/FreeRTOS/org/Source/Debug/Drivers/STM32F4xx_HAL_Driver/Src/Legacy/subdir.mk
-include Third-Party/FreeRTOS/org/Source/Debug/Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Third-Party/FreeRTOS/org/Source/Debug/Core/Src/subdir.mk
-include Third-Party/FreeRTOS/org/Source/Debug/subdir.mk
-include Third-Party/FreeRTOS/org/Source/CMSIS_RTOS_V2/subdir.mk
-include Third-Party/FreeRTOS/org/Source/subdir.mk
-include Startup/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/Legacy/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Src/Peripherals/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
00-Ref-FreeRTOS-SystemView.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
00-Ref-FreeRTOS-SystemView.list \

OBJCOPY_BIN += \
00-Ref-FreeRTOS-SystemView.bin \


# All Target
all: 00-Ref-FreeRTOS-SystemView.elf secondary-outputs

# Tool invocations
00-Ref-FreeRTOS-SystemView.elf: $(OBJS) $(USER_OBJS) C:\Users\Ismael\Dropbox\Smelk\Universidad\ y\ Conocimiento\Cursos\Electronica\Mastering\ RTOS\ Hands-on\ FreeRTOS\ and\ STM32Fx\ with\ Debugging(RTOS)\SME\Programaciones\00-Ref-FreeRTOS-SystemView\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "00-Ref-FreeRTOS-SystemView.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Ismael\Dropbox\Smelk\Universidad y Conocimiento\Cursos\Electronica\Mastering RTOS Hands-on FreeRTOS and STM32Fx with Debugging(RTOS)\SME\Programaciones\00-Ref-FreeRTOS-SystemView\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="00-Ref-FreeRTOS-SystemView.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

00-Ref-FreeRTOS-SystemView.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "00-Ref-FreeRTOS-SystemView.list"
	@echo 'Finished building: $@'
	@echo ' '

00-Ref-FreeRTOS-SystemView.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "00-Ref-FreeRTOS-SystemView.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
