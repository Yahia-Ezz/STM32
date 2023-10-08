Source_Folder = 00_Sources
Output_Folder = 01_Output
Install_Folder = 02_Install
ApplicationName = STM_App

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
NM = arm-none-eabi-nm
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

SRCS = $(wildcard $(Source_Folder)/*.c $(Source_Folder)/*/*.c $(Source_Folder)/*/*/*.c)
HEADERS = $(wildcard $(Source_Folder)/*.h $(Source_Folder)/*/*.h $(Source_Folder)/*/*/*.h)
HEADERS_DIR := $(foreach d,$(HEADERS),-I$(dir $(d)))

CFLAGS += -Wall 
CFLAGS += -O0 
CFLAGS += -mcpu=cortex-m3 
CFLAGS += -mfloat-abi=soft
CFLAGS += -mthumb
CFLAGS += -ggdb
CFLAGS += -D$(MYARG)

LDFLAGS += -L./StaticLibs
LDFLAGS += -lgcc
LDFLAGS += -lm
LDFLAGS += -lc_nano
LDFLAGS += -lc

.PHONY : all

all: $(SRCS)
	@echo -e "\n------%% Building %%------\n"
	$(foreach src,$(SRCS),$(shell echo -e $(CC) $(CFLAGS) $(HEADERS_DIR) -c $(src) -o $(subst .c,.o,$(src)));)
	@#--------------------------------------------------------------------------------------------
	@echo -e "\n------%% Linking %%------\n"  
	@$(shell if [ ! -d $(Output_Folder) ];then mkdir $(Output_Folder);fi )
	$(LD) -Map App.map  -T LinkerCommands.ld $(subst .c,.o,$(SRCS)) $(LDFLAGS) -o $(Output_Folder)/$(ApplicationName).elf
	@#--------------------------------------------------------------------------------------------
	@echo -e "\n------%% ELF->BIN %%------\n"  
	@$(shell if [ ! -d $(Install_Folder) ];then mkdir $(Install_Folder);fi )
	$(OBJCOPY)  $(Output_Folder)/$(ApplicationName).elf $(Install_Folder)/$(ApplicationName).bin -O binary
	@#--------------------------------------------------------------------------------------------
	@echo -e "\n=========================="
	@echo -e "----- BUILD COMPLETE -----"	
	@echo -e "=========================="	

dump:
	$(foreach src,$(SRCS),$(shell  echo -e $(OBJDUMP) -D -h $(subst .c,.o,$(src)) ) > $(subst .c,.list,$(src));)
	$(NM) --numeric-sort $(Output_Folder)/$(ApplicationName).elf
clean:
	rm -rf $(subst .c,.o,$(SRCS))
	rm -rf $(subst .c,.list,$(SRCS))
	rm -rf $(Output_Folder) $(Install_Folder)
	
