################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/board/DE1_SoC_A9/startup_a9.s 

C_SRCS += \
../src/board/DE1_SoC_A9/Serial.c \
../src/board/DE1_SoC_A9/interrupt.c \
../src/board/DE1_SoC_A9/printf.c \
../src/board/DE1_SoC_A9/system_a9.c 

OBJS += \
./src/board/DE1_SoC_A9/Serial.o \
./src/board/DE1_SoC_A9/interrupt.o \
./src/board/DE1_SoC_A9/printf.o \
./src/board/DE1_SoC_A9/startup_a9.o \
./src/board/DE1_SoC_A9/system_a9.o 

S_DEPS += \
./src/board/DE1_SoC_A9/startup_a9.d 

C_DEPS += \
./src/board/DE1_SoC_A9/Serial.d \
./src/board/DE1_SoC_A9/interrupt.d \
./src/board/DE1_SoC_A9/printf.d \
./src/board/DE1_SoC_A9/system_a9.d 


# Each subdirectory must supply rules for building sources it contributes
src/board/DE1_SoC_A9/%.o: ../src/board/DE1_SoC_A9/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler 5'
	armcc.exe --cpu=Cortex-A9.no_neon.no_vfp --arm --apcs=/interwork -I"N:\ECE350\ece350_4\ece350\manual_code\lab3\IPC\src\app" -I"N:\ECE350\ece350_4\ece350\manual_code\lab3\IPC\src\board\DE1_SoC_A9" -I"N:\ECE350\ece350_4\ece350\manual_code\lab3\IPC\src\board\VE_A9" -I"N:\ECE350\ece350_4\ece350\manual_code\lab3\IPC\src\INC" -I"N:\ECE350\ece350_4\ece350\manual_code\lab3\IPC\src\kernel" --c99 -O0 -g --md --depend_format=unix_escaped --no_depend_system_headers --depend_dir="src/board/DE1_SoC_A9" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/board/DE1_SoC_A9/startup_a9.o: ../src/board/DE1_SoC_A9/startup_a9.s
	@echo 'Building file: $<'
	@echo 'Invoking: Arm Assembler 5'
	armasm.exe --cpu=Cortex-A9.no_neon.no_vfp --arm --apcs=/interwork -g --md --depend_format=unix_escaped --depend="src/board/DE1_SoC_A9/startup_a9.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


