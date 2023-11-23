/* sort.c
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"
// #include <limits.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>

#define MAX_SIZE 100
int A[MAX_SIZE]; /* size of physical memory; with code, we'll run out of
                    space!*/

int main() {
    int n, i, j, tmp;
    // srand(time(NULL));
    // PrintString("Enter size of array: ");
    // n = ReadInt();
    // n = n > 100 ? 100 : n;
    // PrintString("Enter size of array: ");
    PrintString("Enter n: ");
    n = ReadInt();
    PrintString("You have enter: ");
    PrintInt(n);
    PrintString("\n");

    /* first initialize the array, in reverse sorted order */
    for (i = 0; i < n; i++) {
        PrintString("A[");
        PrintInt(i);
        PrintString("] = ");

        A[i] = ReadInt();
        // A[i] = A[i] = rand() % (INT_MAX - INT_MIN + 1) + INT_MIN;
    }

    PrintString("\nYou have enter:\n");
    for (i = 0; i < n; i++) {
        PrintInt(A[i]);
        PrintChar('\t');
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < (n - i - 1); j++)
            if (A[j] > A[j + 1]) {
                tmp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = tmp;
            }
    }

    PrintString("\nAfter sort:\n");
    for (i = 0; i < n; i++) {
        PrintInt(A[i]);
        PrintChar('\t');
    }
    PrintChar('\n');

    Halt();
}
