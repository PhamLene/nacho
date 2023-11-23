/* sort.c*/

#include "syscall.h"

int main() {
    PrintString("Our team:\n");
    PrintString("21120434 - Bui Tien Dung\n");
    PrintString("21120443 - Pham The Duyet\n");
    PrintString("21120481 - Chau Hoang Khoa\n");
    PrintString("21120494 - Ho Trong Le\n");
    PrintString("21120495 - Pham Thi My Le\n\n");
    PrintString("ASCII: at directory code, \"run ./userprog/nachos -rs 1023 -x "
                "test/ascii\" to "
                "print the ASCII table\n");
    PrintString("Sort:  at directory code, run \"./userprog/nachos -rs 1023 -x "
                "test/sort\" to start the sort program\n");
    PrintString("\t- Enter n (the length of the array, 0 <= n <= 100)\n");
    PrintString("\t- Enter n elements of the array\n");
    PrintString(
        "\t- The program will print out the sorted array and then exit\n");

    Halt();
}
