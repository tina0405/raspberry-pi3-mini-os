all:
	aarch64-linux-gnu-gcc -c -fno-stack-protector execom.c -o execom.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector user_sys.S -o user_sys.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector sys_need.c -o sys_need.o
	aarch64-linux-gnu-ld -T test.ld -o execom.elf sys_need.o execom.o user_sys.o -Map test.map
	aarch64-linux-gnu-objcopy execom.elf -O binary execom.img
clean:
	rm -rf *.o *.img *.map *.elf
