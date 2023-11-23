//  ascii.c

#include "syscall.h"

int main() {
    int i;
    PrintString("ASCII Table:\n");
    PrintString("+--------+----------+\n");
    PrintString("| Decimal| Character|\n");
    PrintString("+--------+----------+\n");
    PrintChar('\n');
   
    for ( i = 32; i <= 126; i++) {
        PrintString("   ");
        PrintInt(i);
        PrintString("\t\t");
        PrintChar((char)i);
        PrintChar('\n');
    }

    PrintString("+--------+----------+\n");

    Halt();
}
