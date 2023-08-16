Source_Folder = 00_Sources
Output_Folder = 01_Output
Install_Folder = 02_Install
ApplicationName = STM_App
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
NM = arm-none-eabi-nm
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

VPATH += $(Source_Folder)/Startup_code  $(Source_Folder)/Interrupts

SRCS = $(wildcard */*.c */*/*.c */*/*/*.c)

CFLAGS += -Wall 
CFLAGS += -O0 
CFLAGS += -mcpu=cortex-m3 
CFLAGS += -mthumb
CFLAGS += -ggdb
CFLAGS += -I$(Source_Folder)/ENC28J60
CFLAGS += -I$(Source_Folder)/btld_fw/nvic
CFLAGS += -I$(Source_Folder)/btld_fw/spi
CFLAGS += -I$(Source_Folder)/Startup_code
CFLAGS += -I$(Source_Folder)/btld_fw/gpio
CFLAGS += -I$(Source_Folder)/btld_fw/rcc
CFLAGS += -I$(Source_Folder)/btld_fw/uart
CFLAGS += -I$(Source_Folder)/btld_fw/flash_wrapper
CFLAGS += -I$(Source_Folder)/btld_fw/dma
CFLAGS += -I$(Source_Folder)/btld_fw/scb
CFLAGS += -I$(Source_Folder)/btld_fw/i2c
CFLAGS += -I$(Source_Folder)/X_Modem
CFLAGS += -I$(Source_Folder)/btld
CFLAGS += -I$(Source_Folder)/Scheduler
CFLAGS += -I$(Source_Folder)/Serial_Print
CFLAGS += -I$(Source_Folder)/Can
CFLAGS += -D$(MYARG)

all: $(SRCS)
	@echo -e "\n------%% Building %%------\n"  
	$(foreach src,$(SRCS),$(shell  echo -e $(CC) $(CFLAGS) -c $(src) -o $(subst .c,.o,$(src)));)
	@#--------------------------------------------------------------------------------------------
	@echo -e "\n------%% Linking %%------\n"  
	@$(shell if [ ! -d $(Output_Folder) ];then mkdir $(Output_Folder);fi )
	$(LD) -Map App.map  -T LinkerCommands.ld $(subst .c,.o,$(SRCS)) -o $(Output_Folder)/$(ApplicationName).elf
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
	
