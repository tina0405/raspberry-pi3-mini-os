ARMGNU ?= aarch64-linux-gnu

COPS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only

all : kernel8.img

objects/%_c.o: %.c
	mkdir -p objects
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

objects/%_s.o: %.S
	$(ARMGNU)-gcc -Iinclude -MMD -c $< -o $@

C_FILES = $(wildcard *.c)
ASM_FILES = $(wildcard *.S)
OBJ_FILES = $(C_FILES:%.c=objects/%_c.o)
OBJ_FILES += $(ASM_FILES:%.S=objects/%_s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

kernel8.img: linker.ld $(OBJ_FILES)
	$(ARMGNU)-ld -T linker.ld -o objects/kernel8.elf  $(OBJ_FILES) -Map System.map
	$(ARMGNU)-objcopy objects/kernel8.elf -O binary kernel8.img 

clean :
	rm -rf objects *.img 

