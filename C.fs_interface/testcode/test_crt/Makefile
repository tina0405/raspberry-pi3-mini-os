all:
	aarch64-linux-gnu-gcc -c -fstack-protector crt_test.c -o crt_test.o
	aarch64-linux-gnu-gcc -c -fstack-protector user_sys.S -o user_sys.o
	aarch64-linux-gnu-ld -o crt_test.elf crt_test.o user_sys.o -Map crt_test.map
	aarch64-linux-gnu-objcopy crt_test.elf -O binary crt_test.img
clean:
	rm -rf *.o *.img *.map *.elf
