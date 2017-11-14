################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ..\makefile.init

RM := del 

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include libraries\Wire\src\utility\subdir.mk
-include libraries\Wire\src\subdir.mk
-include libraries\SPI\src\subdir.mk
-include libraries\Grove_Temper_Humidity_TH02-master\subdir.mk
-include libraries\Grove_SHT31\example\subdir.mk
-include libraries\Grove_SHT31\subdir.mk
-include libraries\Adafruit_NeoPixel\subdir.mk
-include core\core\subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ..\makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
C:\Eclipse_Arduino\workspace\MySensors_Veilleuse_Perrine\Release\arduino.ar \

ELF += \
MySensors_Veilleuse_Perrine.elf \

HEX += \
MySensors_Veilleuse_Perrine.hex \


# All Target
all: MySensors_Veilleuse_Perrine

# Tool invocations
MySensors_Veilleuse_Perrine: $(HEX)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"C:\Eclipse_Arduino\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-size" -A "C:/Eclipse_Arduino/workspace/MySensors_Veilleuse_Perrine/Release/MySensors_Veilleuse_Perrine.elf"
	@echo 'Finished building target: $@'
	@echo ' '

C:\Eclipse_Arduino\workspace\MySensors_Veilleuse_Perrine\Release\arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"C:\Eclipse_Arduino\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-gcc-ar" rcs    C:/Eclipse_Arduino/workspace/MySensors_Veilleuse_Perrine/Release/arduino.ar $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

MySensors_Veilleuse_Perrine.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"C:\Eclipse_Arduino\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-gcc" -Wall -Wextra -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p -o "C:\Eclipse_Arduino\workspace\MySensors_Veilleuse_Perrine/Release/MySensors_Veilleuse_Perrine.elf"   $(LINK_OBJ) $(AR)   C:/Eclipse_Arduino/workspace/MySensors_Veilleuse_Perrine/Release/arduino.ar  "-LC:\Eclipse_Arduino\workspace\MySensors_Veilleuse_Perrine/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

MySensors_Veilleuse_Perrine.hex: $(ELF)
	@echo 'Create eeprom image'
	${A.RECIPE.OBJCOPY.EEP.PATTERN}
	${A.RECIPE.OBJCOPY.HEX.PATTERN}
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(HEX)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ..\makefile.targets
