
#include "syscall.h"

int main() {
    PrintChar('\n');
    PrintString("Enter a number: ");
    PrintInt(ReadInt());
    PrintChar('\n');
    Halt();
}
