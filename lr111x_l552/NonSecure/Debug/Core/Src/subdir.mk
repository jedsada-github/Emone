################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/crc.c \
../Core/Src/delay.c \
../Core/Src/dma.c \
../Core/Src/fifo.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/lpm-board.c \
../Core/Src/main.c \
../Core/Src/rng.c \
../Core/Src/rtc.c \
../Core/Src/spi.c \
../Core/Src/stm32l5xx_hal_msp.c \
../Core/Src/stm32l5xx_hal_timebase_tim.c \
../Core/Src/stm32l5xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l5xx_ns.c \
../Core/Src/systime.c \
../Core/Src/tim.c \
../Core/Src/timer.c \
../Core/Src/usart.c \
../Core/Src/vcom.c 

OBJS += \
./Core/Src/adc.o \
./Core/Src/crc.o \
./Core/Src/delay.o \
./Core/Src/dma.o \
./Core/Src/fifo.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/lpm-board.o \
./Core/Src/main.o \
./Core/Src/rng.o \
./Core/Src/rtc.o \
./Core/Src/spi.o \
./Core/Src/stm32l5xx_hal_msp.o \
./Core/Src/stm32l5xx_hal_timebase_tim.o \
./Core/Src/stm32l5xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l5xx_ns.o \
./Core/Src/systime.o \
./Core/Src/tim.o \
./Core/Src/timer.o \
./Core/Src/usart.o \
./Core/Src/vcom.o 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/crc.d \
./Core/Src/delay.d \
./Core/Src/dma.d \
./Core/Src/fifo.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/lpm-board.d \
./Core/Src/main.d \
./Core/Src/rng.d \
./Core/Src/rtc.d \
./Core/Src/spi.d \
./Core/Src/stm32l5xx_hal_msp.d \
./Core/Src/stm32l5xx_hal_timebase_tim.d \
./Core/Src/stm32l5xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l5xx_ns.d \
./Core/Src/systime.d \
./Core/Src/tim.d \
./Core/Src/timer.d \
./Core/Src/usart.d \
./Core/Src/vcom.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32L552xx -DSTM32L5 -DREGION_AS923 -DACTIVE_REGION=LORAMAC_REGION_AS923 -c -I../Core/Inc -I../../Secure_nsclib -I../../Drivers/STM32L5xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/BSP/STM32L552E-EV -I../../Drivers/BSP/Components/Common -I../../Drivers/BSP/Components/icg20330 -I../../Drivers/BSP/Components/lr1110 -I../../Drivers/BSP/lr1110_driver/src -I../../Drivers/BSP/EEPROM_Emul/Core -I../../Drivers/BSP/EEPROM_Emul/Porting/STM32L5 -I../../Middlewares/Third_Party/LoRaWAN/Mac -I../../Middlewares/Third_Party/LoRaWAN/Mac/region -I../../Middlewares/Third_Party/LoRaWAN/Patterns -I../../Middlewares/Third_Party/LoRaWAN/Patterns/Advanced -I../../Middlewares/Third_Party/LoRaWAN/Patterns/Advanced/LmHandler -I../../Middlewares/Third_Party/LoRaWAN/Patterns/Advanced/LmHandler/packages -I../../Middlewares/Third_Party/LoRaWAN/Patterns/Basic -I../../Middlewares/Third_Party/LoRaWAN/Phy -I../../Middlewares/Third_Party/LoRaWAN/Utilities -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

