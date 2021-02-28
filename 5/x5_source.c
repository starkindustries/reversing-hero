#include <stdio.h>
#include <assert.h>

typedef unsigned long ulong;

void swap(ulong *x, ulong *y)
{
    printf("swap before: [%lu, %lu]\n", *x, *y);
    *y = *y ^ *x;
    *x = *x ^ *y;
    *y = *y ^ *x;
    printf("swap after:  [%lu, %lu]\n", *x, *y);
    return;
}

void target_func_param_func(long *hex_data, ulong max)
{
    ulong extraout_RDX;
    ulong rcx = 0;
    ulong rdx = 0;

    do
    {
        if ((ulong)hex_data[rdx] < (ulong)hex_data[rcx] || hex_data[rdx] == hex_data[rcx])
        {
            swap((ulong *)(hex_data + rdx), (ulong *)(hex_data + rcx));
            rdx = extraout_RDX;
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

    x = 0xdeadbeef;
    y = 0xc0ffee;
    swap(&x, &y);

    ulong counter = 0x400;
    
}