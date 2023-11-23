// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include "copyright.h"
#include "syscall.h"
#include "system.h"
#include <cstring>

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

#define MAX_READ_STRING_LENGTH 255

void AdvancePC() {
    machine->registers[PrevPCReg] = machine->registers[PCReg];
    machine->registers[PCReg] = machine->registers[NextPCReg];
    machine->registers[NextPCReg] += 4;
}

// Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
// Output: Bo nho dem Buffer(char*)
// Chuc nang: Sao chep vung nho User sang vung nho System
char *User2System(int virtAddr, int limit) {
    int i; // chi so index
    int oneChar;
    char *kernelBuf = NULL;
    kernelBuf = new char[limit + 1]; // can cho chuoi terminal
    if (kernelBuf == NULL)
        return kernelBuf;

    memset(kernelBuf, 0, limit + 1);

    for (i = 0; i < limit; i++) {
        machine->ReadMem(virtAddr + i, 1, &oneChar);
        kernelBuf[i] = (char)oneChar;
        if (oneChar == 0)
            break;
    }
    return kernelBuf;
}

// Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem
// buffer(char*) Output: So byte da sao chep(int) Chuc nang: Sao chep vung nho
// System sang vung nho User
int System2User(int virtAddr, int len, char *buffer) {
    if (len < 0)
        return -1;
    if (len == 0)
        return len;
    int i = 0;
    int oneChar = 0;
    do {
        oneChar = (int)buffer[i];
        machine->WriteMem(virtAddr + i, 1, oneChar);
        i++;
    } while (i < len && oneChar != 0);
    return i;
}

void reverse(char src[], int start, int end) {
    while (start < end) {
        char temp =
            src[start]; // a l  => temp = a,   a = l -> l, l = temp = a -> a
        src[start] = src[end];
        src[end] = temp;
        start++;
        end--;
    }
}

void ExceptionHandler(ExceptionType which) {
    int type = machine->ReadRegister(2);
    switch (which) {
    case NoException: // return control to kernel
        return;
    case PageFaultException:
    case ReadOnlyException:
    case BusErrorException:
    case AddressErrorException:
    case OverflowException:
    case IllegalInstrException:
    case NumExceptionTypes:
        // DEBUG('a', which, "\n");
        interrupt->Halt();

    case SyscallException:
        switch (type) {
        case SC_Halt: {
            // DEBUG('a', "Shutdown, initiated by user program.\n");
            // printf("Unexpected user mode exception %d %d\n", which, type);
            interrupt->Halt();
        }
        case SC_ReadInt: {
            int maxBuf = 255;
            char *buf = new char[maxBuf + 1];
            bool isNegative = false;
            int start = 0, size, ans = 0;

            size = gSynchConsole->Read(buf, maxBuf);

            // empty input
            if (size == 0) {
                DEBUG('a', "\n Invalid number.");
                printf("\n\n Invalid number.");

                machine->WriteRegister(2, 0);
                AdvancePC();
                delete buf;
                return;
            }

            while (buf[start] == ' ') {
                start++;
            }

            // number is negative
            if (buf[start] == '-') {
                isNegative = true;
                start++;
            }

            // handle: 000005
            while (buf[start] == '0') {
                start++;
            }

            for (int i = start; i < size; ++i) {

                if (buf[i] == '.') { // valid: 1.0000
                    for (int j = i + 1; j < size; ++j) {
                        if (buf[j] != '0') { // Invalid: 1.0002
                            DEBUG('a', "\n Invalid number.");
                            printf("\n\n Invalid number.");

                            machine->WriteRegister(2, 0);
                            AdvancePC();
                            delete buf;
                            return;
                        }
                    }
                    break;
                } else if (buf[i] < '0' || buf[i] > '9') { // Invalid: 12a1
                    DEBUG('a', "\n Invalid number.");
                    printf("\n\n Invalid number.");

                    machine->WriteRegister(2, 0);
                    AdvancePC();
                    delete buf;
                } else {
                    ans = ans * 10 + (buf[i] - '0');

                    if ((!isNegative && (i - start + 1) > 10) ||
                        (isNegative && (i - start + 1) > 11)) {
                        ans = isNegative ? -__INT_MAX__ : __INT_MAX__;
                        machine->WriteRegister(2, ans);
                        AdvancePC();
                        delete buf;
                        return;
                    }
                }
            }

            if (isNegative)
                ans *= -1;

            machine->WriteRegister(2, ans);
            AdvancePC();
            delete buf;

        } break;

        case SC_PrintInt: {
            int number = machine->ReadRegister(4);

            if (number == 0) {
                gSynchConsole->Write("0", 1);
                AdvancePC();
                break;
            }

            int maxBuf = 255;
            char *buf = new char[maxBuf + 1];
            bool isNegative = false;
            int start = 0;

            if (number < 0) {
                isNegative = true;
                number *= -1;
                buf[start++] = '-'; // buf[0] = '-'
            }

            while (number != 0) {
                buf[start++] = (number % 10) + '0';
                number /= 10;
            }

            buf[start] = '\0';

            reverse(buf, (isNegative ? 1 : 0), start - 1);

            gSynchConsole->Write(buf, start);
            AdvancePC();
        } break;

        case SC_ReadChar: {
            char ch;
            gSynchConsole->Read(&ch, 1);
            machine->WriteRegister(2, ch);
            AdvancePC();
            break;
        }

        case SC_PrintChar: {
            char ch = static_cast<char>(machine->ReadRegister(4));
            gSynchConsole->Write(&ch, 1);
            AdvancePC();
            break;
        }

        case SC_ReadString: {
            int bufAddrUser = machine->ReadRegister(4);
            int maxLength = machine->ReadRegister(5);

            char *buf = new char[maxLength + 1];
            if (buf == NULL) {
                printf("Memory allocation failed for string buffer.\n");
                machine->WriteRegister(2, -1);
                AdvancePC();
                break;
            }

            int sz = 0;
            char ch;
            while (sz < maxLength) {

                gSynchConsole->Read(&ch, 1);
                if (ch == '\0') {
                    break;
                }
                buf[sz++] = ch;
            }

            buf[sz] = '\0';
            System2User(bufAddrUser, sz + 1, buf);

            delete[] buf;
            AdvancePC();
        } break;
        case SC_PrintString: {
            int userAddr = machine->ReadRegister(4);

            char ch;
            while (true) {
                if (!machine->ReadMem(userAddr, 1, (int *)&ch)) {
                    printf("Error reading character from user space.\n");
                    machine->WriteRegister(2, -1);
                    return;
                }

                if (ch == '\0') {
                    break;
                }

                gSynchConsole->Write(&ch, 1);

                userAddr++;
            }

            machine->WriteRegister(2, 0);
            AdvancePC();
        } break;

        default:
            // cerr << "Unexpected system call " << type << "\n";
            break;
        }
        break;
    default:
        // cerr << "Unexpected user mode exception" << (int)which << "\n";
        break;
    }
}
