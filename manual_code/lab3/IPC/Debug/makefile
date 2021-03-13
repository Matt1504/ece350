################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := clean

# All of the sources participating in the build are defined here
-include sources.mk
-include src/kernel/subdir.mk
-include src/board/DE1_SoC_A9/subdir.mk
-include src/app/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: IPC.axf

# Tool invocations
IPC.axf: $(OBJS) N:\ECE350\ece350_4\ece350\manual_code\lab3\IPC\scatter_DE1_SoC.sct $(USER_OBJS) $(LIBS)
	@echo 'Building target: $@'
	@echo 'Invoking: Arm Linker 5'
	armlink.exe --cpu=Cortex-A9.no_neon.no_vfp --entry=__Vectors --scatter="N:\ECE350\ece350_4\ece350\manual_code\lab3\IPC\scatter_DE1_SoC.sct" --info=sizes -o "IPC.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SX_DEPS) $(ASM_UPPER_DEPS) $(EXECUTABLES) $(OBJS) $(SX_UPPER_DEPS) $(ASM_DEPS) $(S_DEPS) $(S_UPPER_DEPS) $(C_DEPS) IPC.axf 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
