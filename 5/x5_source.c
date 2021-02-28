#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#define DEBUG

typedef unsigned long ulong;

// ********************
// Helper Functions
// ********************

void debug(const char *format, ...)
{
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#endif
}

void print_array(long *arr, int length)
{
    printf("{");
    for (int i = 0; i < length - 1; i++)
    {
        printf("0x%02lx, ", arr[i]);
    }
    printf("0x%02lx}\n", arr[length - 1]);
}

// ********************
// x5 Functions
// ********************

void swap(ulong *x, ulong *y)
{
    // Note if you pass in the same pointer
    // Then that pointer will XOR to zero
    debug("swap before: [0x%lx, 0x%lx]\n", *x, *y);
    *y ^= *x;
    debug("swap step 1: [0x%lx, 0x%lx]\n", *x, *y);
    *x ^= *y;
    debug("swap step 2: [0x%lx, 0x%lx]\n", *x, *y);
    *y ^= *x;
    debug("swap after:  [0x%lx, 0x%lx]\n", *x, *y);
    return;
}

void sort(ulong *hex_data, ulong max)
{
    ulong extraout_RDX;
    ulong rcx = 0;
    ulong rdx = 0;

    do
    {
        if (hex_data[rdx] < hex_data[rcx])
        // if (hex_data[rdx] < hex_data[rcx] || hex_data[rdx] == hex_data[rcx])
        {
            debug("rdx[%lu], rcx[%lu], hex_data[%p]\n", rdx, rcx, hex_data);
            debug("About to swap: [0x%lx, 0x%lx]\n", *(hex_data + rdx), *(hex_data + rcx));
            swap((hex_data + rdx), (hex_data + rcx));
            // rdx = extraout_RDX;
        }
        rcx = rcx + 1;
    } while ((rcx < max) || (rcx = rdx + 1, rdx = rcx, rcx < max));
    return;
}

ulong calc_sum(ulong *hex_data, long counter)
{
    ulong sum = 0;
    do
    {
        sum += *hex_data;
        // printf("hex data: {%ld}\n", *hex_data);
        counter -= 1;
        hex_data += 1;
    } while (counter != 0);
    return sum;
}

// ********************
// Main
// ********************
int main()
{
    // Test calc_sum function
    ulong data[] = {0x1, 0x2, 0x3, 0x4, 0x5};
    ulong result = calc_sum(data, sizeof(data) / sizeof(data[0]));
    assert(result == 15);

    ulong data2[] = {0x10, 0x7, 0x9, 0x12};
    result = calc_sum(data2, 4);
    assert(result == 50);
    // printf("Result: %ld\n", result);

    // Test swap
    ulong x = 0x1;
    ulong y = 0x2;
    swap(&x, &y);
    assert(x == 0x2 && y == 0x1);

    x = 0x25;
    y = 0x25;
    swap(&x, &y);
    assert(x == 0x25 && y == 0x25);

    x = 0xdeadbeef;
    y = 0xc0ffee;
    swap(&x, &y);
    assert(x == 0xc0ffee && y == 0xdeadbeef);

    // Test sort
    ulong data3[] = {0x3, 0x2, 0x4, 0x1};
    printf("Before [%p]: ", data3);
    print_array(data3, 4);
    sort(data3, 4);
    printf("After [%p]: ", data3);
    print_array(data3, 4);

    ulong counter = 0x400;
}