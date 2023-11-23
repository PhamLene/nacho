
#include "syscall.h"

int main() {
    PrintString("Enter a character: ");
    PrintChar(ReadChar());
    PrintChar('\n');
    Halt();
}
