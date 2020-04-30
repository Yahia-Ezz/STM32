Output_Folder = 02_Output
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
NM = arm-none-eabi-nm
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

VPATH +=startup_code

SRCS = $(wildcard */*.c */*/*.c)

CFLAGS += -Wall 
CFLAGS += -O0 
CFLAGS += -mcpu=cortex-m3 
CFLAGS += -mthumb
CFLAGS += -Iled
CFLAGS += -Istartup_code

all: $(SRCS)
	@echo -e "\n------%% Building %%------\n"  
	$(foreach src,$(SRCS),$(shell  echo -e $(CC) $(CFLAGS) -c $(src) -o $(subst .c,.o,$(src)));)
	@#--------------------------------------------------------------------------------------------
	@echo -e "\n------%% Linking %%------\n"  
	@$(shell if [ ! -d $(Output_Folder) ];then mkdir $(Output_Folder);fi )
	$(LD)  -T LinkerCommands.ld $(subst .c,.o,$(SRCS)) -o $(Output_Folder)/App.elf
	@#--------------------------------------------------------------------------------------------
	@echo -e "\n------%% ELF->BIN %%------\n"  
	$(OBJCOPY)  $(Output_Folder)/App.elf $(Output_Folder)/App.bin -O binary
	@#--------------------------------------------------------------------------------------------
	@echo -e "\n=========================="
	@echo -e "----- BUILD COMPLETE -----"	
	@echo -e "=========================="

DUMP:
	$(OBJDUMP) -D -h startup_code/startup.o > prog/startup.list
	$(OBJDUMP) -D -h main.o > prog/main.list
	$(OBJDUMP) -D -h prog/prog.elf > prog/prog.list
	$(NM) --numeric-sort prog/prog.elf

clean:
	rm -rf $(subst .c,.o,$(SRCS))