all:
	aarch64-linux-gnu-gcc -c -fno-stack-protector user.c -o user.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector user_sys.S -o user_sys.o
	aarch64-linux-gnu-ld -T test.ld -o test.elf user.o user_sys.o -Map test.map
	aarch64-linux-gnu-objcopy test.elf -O binary test.img
clean:
	rm -rf *.o *.img *.map *.elf
