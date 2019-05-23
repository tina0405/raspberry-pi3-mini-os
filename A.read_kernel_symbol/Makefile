ARMGNU ?= aarch64-linux-gnu

COPS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only -mtune=cortex-a53 -march=armv8-a
ASMOPS = -Iinclude -mtune=cortex-a53 -march=armv8-a

BUILD_DIR = build
SRC_DIR = src
FS_DIR = src/file_system
APP_DIR = src/app
DEVICE_DIR = src/device
KERNEL_DIR = src/kernel
MM_DIR = src/memory_manager
PM_DIR = src/proc_manager
THREAD_DIR = src/proc_manager/thread
USER_DIR = src/testcode
SHELL_DIR = src/user_shell

all : kernel8.img

clean :
	rm -rf $(BUILD_DIR) *.img *.map symbol.txt


$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)	
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@



C_FILES = $(wildcard $(FS_DIR)/*.c $(USER_DIR)/*.c $(PM_DIR)/*.c $(MM_DIR)/*.c $(THREAD_DIR)/*.c $(KERNEL_DIR)/*.c $(DEVICE_DIR)/*.c $(APP_DIR)/*.c $(SHELL_DIR)/*.c)
ASM_FILES = $(wildcard $(FS_DIR)/*.S $(USER_DIR)/*.S $(PM_DIR)/*.S $(MM_DIR)/*.S $(KERNEL_DIR)/*.S $(DEVICE_DIR)/*.S $(APP_DIR)/*.S $(SHELL_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)


DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

kernel8.img: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	$(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf  $(OBJ_FILES) -Map System.map
	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary kernel8.img
	./symbol.sh
