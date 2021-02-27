#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h> /* For SYS_xxx definitions */

typedef unsigned long ulong;
typedef unsigned int uint;

int DEBUG = 1;

void clear_buffer(char *buffer)
{
    ulong counter = 0x20; // 0x20 = 32 decimal
    while (counter != 0)
    {
        counter = counter + -1;
        *buffer = 0;
        buffer = buffer + 1;
    }
    return;
}

// **********************************
// read_char() function
// Reads just 1 character at a time
// 0x400409 <_start+513>:	mov    rdi,r12
// 0x40040c <_start+516>:	call   0x40033e <_start+310>
// **********************************
ulong read_char(ulong file_descriptor, char *buffer)
{
    ulong rax = syscall(SYS_read, file_descriptor, buffer, 0x1);
    return rax;
}

void scramble(char *buffer, ulong char1)
{
    ulong offset1 = *buffer;
    ulong offset2 = buffer[1];
    ulong offset3 = buffer[2];
    ulong offset4 = buffer[3];
    ulong offset5;
    ulong counter = 0x100;

    char1 = char1 & 0xff;

    do
    {
        char1 = (offset1 ^ 0xb010c150e26d857b) + char1;
        offset1 = (char1 >> 0x38 | (char1 & 0xff000000000000) >> 0x28 | (char1 & 0xff0000000000) >> 0x18 | (char1 & 0xff00000000) >> 8 | (char1 & 0xff000000) << 8 | (char1 & 0xff0000) << 0x18 | (char1 & 0xff00) << 0x28 | char1 << 0x38) << 7 | (char1 << 0x38) >> 0x39;
        offset5 = (offset2 ^ 0x4526e76d6d799032) + offset1;
        offset2 = offset5 >> 0x38 | (offset5 & 0xff000000000000) >> 0x28 | (offset5 & 0xff0000000000) >> 0x18 | (offset5 & 0xff00000000) >> 8 | (offset5 & 0xff000000) << 8 | (offset5 & 0xff0000) << 0x18;
        offset2 = offset2 << 0x17 | (offset2 | (offset5 & 0xff00) << 0x28 | offset5 << 0x38) >> 0x29;
        offset5 = (offset3 ^ 0xa27c2069adb0e0c3) + offset2;
        offset3 = offset5 >> 0x38 | (offset5 & 0xff000000000000) >> 0x28 | (offset5 & 0xff0000000000) >> 0x18;
        offset3 = offset3 << 0x29 | (offset3 | (offset5 & 0xff00000000) >> 8 | (offset5 & 0xff000000) << 8 | (offset5 & 0xff0000) << 0x18 | (offset5 & 0xff00) << 0x28 | offset5 << 0x38) >> 0x17;
        offset5 = (offset4 ^ 0xf0d6ea65fdb172b6) + offset3;
        offset4 = offset5 >> 0x38 | (offset5 & 0xff000000000000) >> 0x28 | (offset5 & 0xff0000000000) >> 0x18;
        char1 = offset4 << 0x2f | (offset4 | (offset5 & 0xff00000000) >> 8 | (offset5 & 0xff000000) << 8 | (offset5 & 0xff0000) << 0x18 | (offset5 & 0xff00) << 0x28 | offset5 << 0x38) >> 0x11;
        counter = counter + -1;
        offset4 = char1;
    } while (counter != 0);

    *buffer = *buffer + offset1;
    buffer[1] = buffer[1] + offset2;
    buffer[2] = buffer[2] + offset3;
    buffer[3] = buffer[3] + char1;
    return;
}

ulong unknown_func(ulong *deadbeef, ulong char1, int *counter)
{
    char bVar1;
    char bVar3;
    int iVar4;

    if (char1 == 0x30 && *counter != 0)
    {

        bVar3 = (char)*counter;
        bVar1 = bVar3 & 0x3f;

        *deadbeef = (*deadbeef << bVar1 | *deadbeef >> 0x40 - bVar1);   // 0x400383 <_start+379>   rol    rax,cl
        
        // Shift right then left by 1 just clears the last bit
        // `rax & 0xfffffffffffffffe` does the same thing
        *deadbeef = *deadbeef & 0xfffffffffffffffe;                     // 0x400386 <_start+382>   shr    rax,1
                                                                        // 0x400389 <_start+385>   shl    rax,1
        bVar3 = bVar3 & 0x3f;
        *deadbeef = *deadbeef >> bVar3 | *deadbeef << 0x40 - bVar3;     // 0x40038c <_start+388>   ror    rax,cl
        *counter -= 1;                                                  // 0x400392 <_start+394>   dec    DWORD PTR [rdi+0x8]
        return 0;
    }
    else if (char1 == 0x31 && *counter < 0x40)
    {
        *counter += 1;
        bVar3 = (char)*counter;
        bVar1 = bVar3 & 0x3f;
        
        // Rotate left
        *deadbeef = (*deadbeef << bVar1 | *deadbeef >> 0x40 - bVar1);

        // Clear last bit 
        *deadbeef = *deadbeef & 0xfffffffffffffffe;
        
        bVar3 = bVar3 & 0x3f;

        // Rotate right
        *deadbeef = *deadbeef >> bVar3 | *deadbeef << 0x40 - bVar3;
        return 0;
    }
    else if (char1 == 0x32 && *counter < 0x40)
    {
        *counter += 1;
        bVar3 = (char)*counter;
        bVar1 = bVar3 & 0x3f;
        *deadbeef = (*deadbeef << bVar1 | *deadbeef >> 0x40 - bVar1);
        
        // Clears the last bit then sets it to 1
        *deadbeef = *deadbeef & 0xfffffffffffffffe | 1;
        
        bVar3 = bVar3 & 0x3f;

        // Rotate back left
        *deadbeef = *deadbeef >> bVar3 | *deadbeef << 0x40 - bVar3;
        return 0;
    }

    return 0xffffffffffffffff; // -1
}

ulong verify(ulong file_descriptor, char *buffer)
{
    ulong char1;
    ulong result;
    ulong rax;

    ulong counter = 0x80;
    int counter2 = 0x40;
    ulong deadbeef = 0xdeadfacedeadbeef;

    do
    {
        rax = read_char(file_descriptor, buffer);
        if (rax == 0xffffffffffffffff)
        {
            printf("rax is -1\n");
            return -1;
        }
        char1 = (ulong)buffer[0];
        *buffer = 0;
        printf("char read: {0x%02x}\n", (char)char1);

        // Scramble
        printf("buffer before scramble: {%s}\n", buffer);
        scramble(buffer, char1);
        printf("buffer after scramble:  {%s}\n", buffer);

        result = unknown_func(&deadbeef, char1, &counter2);
        if (result != 0)
        {
            printf("result is not zero: {%ld}\n", result);
            return result;
        }
        if (deadbeef == 0x123456701234567)
        {
            printf("VICTORY! deadbeef equals 0x123456701234567!\n");
            return 0;
        }
        counter -= 1;
    } while (counter != 0);
    return -1;
}

void __start()
{
    ulong rax;
    ulong r13;

    // ************************
    // Syscall to open a file
    // umode_t mode = 0;        // umode not necessary?
    // ************************
    char filename[] = "d.d";
    int flags = 0;
    ulong file_descriptor = syscall(SYS_open, filename, flags);

    // Clear buffer
    char buffer[32];
    clear_buffer(buffer);

    // Verify
    rax = verify(file_descriptor, buffer);

    r13 = 2;
    if (rax == 0)
    {
        r13 = 0;
        // print ReversingHero string w/ syscall();
        printf("! + \n! -\n! --\nReversingHero\n");
        // target_func((ulong)&G_BUFFER32, 0x20);
        // syscall();
    }
    // syscall();
    if (r13 == 1)
    {
        // syscall();
    }
    if (r13 == 2)
    {
        // syscall();
    }
    // syscall();
    // syscall();
    return;
}

int main()
{
    // gdb breakpoints:
    // breakpoint *0x400208     <_start>        _start()
    // breakpoint *0x4003e4     <_start+476>    verify()
    // breakpoint *0x400368     <_start+352>    unknown_func()
    // breakpoint *0x400433     <_start+555>    instruction right after unknown_func() completes
    ulong deadbeef = 0xdeadfacedeadbeef;
    ulong char1;
    int counter = 0x40;

    for (int i = 0; i < 0x41; i++)
    {
        char1 = 0x30;
        printf("deadbeef: {0x%02lx}, char: {0x%02lx}, counter: {%d}\n", deadbeef, char1, counter);
        ulong result = unknown_func(&deadbeef, char1, &counter);
        printf("deadbeef: {0x%02lx}, char: {0x%02lx}, counter: {%d}, result: {%ld}\n\n", deadbeef, char1, counter, result);
    }

    deadbeef = 0xdeadfacedeadbeef;
    counter = 0x0;
    for (int i = 0; i < 0x41; i++)
    {
        char1 = 0x32;
        printf("deadbeef: {0x%02lx}, char: {0x%02lx}, counter: {%d}\n", deadbeef, char1, counter);
        ulong result = unknown_func(&deadbeef, char1, &counter);
        printf("deadbeef: {0x%02lx}, char: {0x%02lx}, counter: {%d}, result: {%ld}\n\n", deadbeef, char1, counter, result);
    }

    return 0;

    printf("Starting program..\n");
    __start();
}