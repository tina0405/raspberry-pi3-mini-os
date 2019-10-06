all:
	aarch64-linux-gnu-gcc -c hanoi.c -o hanoi.o
	aarch64-linux-gnu-gcc -c user_sys.S -o user_sys.o
	aarch64-linux-gnu-gcc -c sys_need.c -o sys_need.o
	aarch64-linux-gnu-ld -T test.ld -o hanoi.elf sys_need.o hanoi.o user_sys.o -Map hanoi.map
	aarch64-linux-gnu-objcopy hanoi.elf -O binary hanoi.img
clean:
	rm -rf *.o *.img *.map *.elf
