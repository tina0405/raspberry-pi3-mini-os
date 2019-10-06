all:
	aarch64-linux-gnu-gcc -c -fno-stack-protector testfork.c -o testfork.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector user_sys.S -o user_sys.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector sys_need.c -o sys_need.o
	aarch64-linux-gnu-ld -T test.ld -o testfork.elf sys_need.o testfork.o user_sys.o -Map test.map
	aarch64-linux-gnu-objcopy testfork.elf -O binary testfork.img
clean:
	rm -rf *.o *.img *.map *.elf
