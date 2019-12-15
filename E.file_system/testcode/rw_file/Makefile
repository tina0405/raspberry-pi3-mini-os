all:
	aarch64-linux-gnu-gcc -c -fno-stack-protector readfile.c -o readfile.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector user_sys.S -o user_sys.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector sys_need.c -o sys_need.o
	aarch64-linux-gnu-ld -T test.ld -o readfile.elf sys_need.o readfile.o user_sys.o -Map test.map
	aarch64-linux-gnu-objcopy readfile.elf -O binary readfile.img
clean:
	rm -rf *.o *.img *.map *.elf
