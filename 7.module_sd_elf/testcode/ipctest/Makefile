all:
	aarch64-linux-gnu-gcc -c -fno-stack-protector user.c -o user.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector user_sys.S -o user_sys.o
	aarch64-linux-gnu-gcc -c -fno-stack-protector user_sys_ipc.c -o user_sys_ipc.o
	aarch64-linux-gnu-ld -T test.ld -o test.elf user.o user_sys.o user_sys_ipc.o -Map test.map
	aarch64-linux-gnu-objcopy test.elf -O binary ipctest.img
clean:
	rm -rf *.o *.img *.map *.elf
