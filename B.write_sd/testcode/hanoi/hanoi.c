#include <stddef.h>

int application(void) 
{	
    call_sys_write("1024 level to do hanoi\n\r");
    hanoi(1024, 'A', 'B', 'C');
    
    return 0;
	
}

void hanoi(int n, char A, char B, char C) {
    
    static int num;
    if(n == 1) {
        call_sys_write("Move sheet from %c to %c\n\r", A, C);
    }
    else {
        hanoi(n-1, A, C, B);     
	hanoi(1, A, B, C);     
	hanoi(n-1, B, A, C);
    }
}

