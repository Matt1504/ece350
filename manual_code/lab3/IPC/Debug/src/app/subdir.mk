################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app/ae.c \
../src/app/ae_mem.c \
../src/app/ae_priv_tasks.c \
../src/app/ae_usr_tasks.c \
../src/app/kcd_task.c 

OBJS += \
./src/app/ae.o \
./src/app/ae_mem.o \
./src/app/ae_priv_tasks.o \
./src/app/ae_usr_tasks.o \
./src/app/kcd_task.o 

C_DEPS += \
./src/app/ae.d \
./src/app/ae_mem.d \
./src/app/ae_priv_tasks.d \
./src/app/ae_usr_tasks.d \
./src/app/kcd_task.d 


# Each subdirectory must supply rules for building sources it contributes
src/app/%.o: ../src/app/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler 5'
	armcc.exe --cpu=Cortex-A9 --apcs=/hardfp --arm --apcs=/interwork -DDEBUG_0 -I"N:\ECE350\ece350\manual_code\lab3\IPC\src\app" -I"N:\ECE350\ece350\manual_code\lab3\IPC\src\board\DE1_SoC_A9" -I"N:\ECE350\ece350\manual_code\lab3\IPC\src\board\VE_A9" -I"N:\ECE350\ece350\manual_code\lab3\IPC\src\INC" -I"N:\ECE350\ece350\manual_code\lab3\IPC\src\kernel" --c99 -O0 -g --md --depend_format=unix_escaped --no_depend_system_headers --depend_dir="src/app" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


