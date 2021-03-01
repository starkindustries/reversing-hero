#include <stdio.h>
#include <stdarg.h>
#include "helper.h"

typedef unsigned long ulong;

void debug(const char *format, ...)
{
#if DEBUG == 1
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