all:
	aarch64-linux-gnu-gcc -c -fno-stack-protector thread.c -I./include -o thread.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector user_sys.S -o user_sys.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector sys_need.c -o sys_need.o
	aarch64-linux-gnu-ld -T test.ld -o thread.elf sys_need.o thread.o user_sys.o -Map test.map
	aarch64-linux-gnu-objcopy thread.elf -O binary thread.img
clean:
	rm -rf *.o *.img *.map *.elf
