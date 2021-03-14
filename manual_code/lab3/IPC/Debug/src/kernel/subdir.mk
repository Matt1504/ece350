################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/kernel/HAL_CA.c \
../src/kernel/k_mem.c \
../src/kernel/k_msg.c \
../src/kernel/k_rtx_init.c \
../src/kernel/k_task.c \
../src/kernel/main_svc_cw.c 

OBJS += \
./src/kernel/HAL_CA.o \
./src/kernel/k_mem.o \
./src/kernel/k_msg.o \
./src/kernel/k_rtx_init.o \
./src/kernel/k_task.o \
./src/kernel/main_svc_cw.o 

C_DEPS += \
./src/kernel/HAL_CA.d \
./src/kernel/k_mem.d \
./src/kernel/k_msg.d \
./src/kernel/k_rtx_init.d \
./src/kernel/k_task.d \
./src/kernel/main_svc_cw.d 


# Each subdirectory must supply rules for building sources it contributes
src/kernel/%.o: ../src/kernel/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler 5'
	armcc.exe --cpu=Cortex-A9 --apcs=/hardfp --arm --apcs=/interwork -DDEBUG_0 -I"N:\ECE350\ece350\manual_code\lab3\IPC\src\app" -I"N:\ECE350\ece350\manual_code\lab3\IPC\src\board\DE1_SoC_A9" -I"N:\ECE350\ece350\manual_code\lab3\IPC\src\board\VE_A9" -I"N:\ECE350\ece350\manual_code\lab3\IPC\src\INC" -I"N:\ECE350\ece350\manual_code\lab3\IPC\src\kernel" --c99 -O0 -g --md --depend_format=unix_escaped --no_depend_system_headers --depend_dir="src/kernel" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


