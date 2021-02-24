################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
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
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

BUILD_ARTIFACT_NAME := FreeRTOS_00-Tasks
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX := 
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME).$(BUILD_ARTIFACT_EXTENSION)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
FreeRTOS_00-Tasks.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
FreeRTOS_00-Tasks.list \

OBJCOPY_BIN += \
FreeRTOS_00-Tasks.bin \


# All Target
all: main-build

# Main-build Target
main-build: FreeRTOS_00-Tasks.elf secondary-outputs

# Tool invocations
FreeRTOS_00-Tasks.elf: $(OBJS) $(USER_OBJS) C:\Users\Ismael\git\STM32\Boards\ Reference\ code\HAL\STM32F4DISCOVERY\ -\ F407VG\FreeRTOS_00-Tasks\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "FreeRTOS_00-Tasks.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Ismael\git\STM32\Boards Reference code\HAL\STM32F4DISCOVERY - F407VG\FreeRTOS_00-Tasks\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="FreeRTOS_00-Tasks.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

FreeRTOS_00-Tasks.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "FreeRTOS_00-Tasks.list"
	@echo 'Finished building: $@'
	@echo ' '

FreeRTOS_00-Tasks.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "FreeRTOS_00-Tasks.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified
.SECONDARY:

-include ../makefile.targets
