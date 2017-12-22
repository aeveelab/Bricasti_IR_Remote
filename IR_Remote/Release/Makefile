################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../Includes \
../Source


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Source/ATmega48.c \
../Source/Timer.c \
../Source/Sleep_State.c \
../Source/IR_Transmit.c \
../Source/Switch_Scan.c \
../Source/Main.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Source/ATmega48.o \
Source/Timer.o \
Source/Sleep_State.o \
Source/IR_Transmit.o \
Source/Switch_Scan.o \
Source/Main.o


OBJS_AS_ARGS +=  \
Source/ATmega48.o \
Source/Timer.o \
Source/Sleep_State.o \
Source/IR_Transmit.o \
Source/Switch_Scan.o \
Source/Main.o


C_DEPS +=  \
Source/ATmega48.d \
Source/Timer.d \
Source/Sleep_State.d \
Source/IR_Transmit.d \
Source/Switch_Scan.d \
Source/Main.d


C_DEPS_AS_ARGS +=  \
Source/ATmega48.d \
Source/Timer.d \
Source/Sleep_State.d \
Source/IR_Transmit.d \
Source/Switch_Scan.d \
Source/Main.d


OUTPUT_FILE_PATH +=IR_Remote.elf

OUTPUT_FILE_PATH_AS_ARGS +=IR_Remote.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

# AVR32/GNU C Compiler













Source/%.o: ../Source/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -I"../Includes"  -Os -fpack-struct -fshort-enums -Wall -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  -mmcu=atmega48pa  -o"$@" "$<" 
	@echo Finished building: $<



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU C Linker
	$(QUOTE)C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="IR_Remote.map" -Wl,-lm   -mmcu=atmega48pa  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "IR_Remote.elf" "IR_Remote.hex"
	"C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "IR_Remote.elf" "IR_Remote.eep" || exit 0
	"C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-objdump.exe" -h -S "IR_Remote.elf" > "IR_Remote.lss"
	"C:\Program Files (x86)\Atmel\AVR Studio 5.1\extensions\Atmel\AVRGCC\3.3.1.27\AVRToolchain\bin\avr-size.exe" -C --mcu=atmega48pa  "IR_Remote.elf"
	




# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS)$(C_DEPS_AS_ARGS) $(EXECUTABLES) 
	rm -rf "IR_Remote.hex" "IR_Remote.lss" "IR_Remote.eep" "IR_Remote.map"
	