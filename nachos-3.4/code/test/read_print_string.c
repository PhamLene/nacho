
#include "syscall.h"
#define MAX_STRING_LEN 255

int main() {
    char buffer[MAX_STRING_LEN + 1];
    PrintString("Enter a string (length <= ");
    PrintInt(MAX_STRING_LEN);
    PrintString(" ):");
    ReadString(buffer, MAX_STRING_LEN);
    PrintString(buffer);
    PrintChar('\n');
    Halt();
}
