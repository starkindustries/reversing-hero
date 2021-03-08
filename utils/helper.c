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

void print_long_array(long *arr, int length)
{
    printf("{");
    for (int i = 0; i < length - 1; i++)
    {
        printf("0x%02lx, ", arr[i]);
    }
    printf("0x%02lx}\n", arr[length - 1]);
}

void print_char_array(char *arr, int length)
{
    printf("{");
    for (int i = 0; i < length - 1; i++)
    {
        printf("0x%02x, ", (unsigned char)arr[i]);
    }
    printf("0x%02x}\n", (unsigned char)arr[length - 1]);
}

// Returns 0 if equal else -1
int compare_char_array(char *a, char *b, int length)
{
    for (int i = 0; i < length; i++)
    {
        if ((unsigned char)*(a + i) != (unsigned char)*(b + i))
        {
            debug("(compare_char_array) Found non-match: a:{0x%02x}, b:{0x%02x}, index:{%d}\n", (unsigned char)*(a + i), (unsigned char)*(b + i), i);
            return -1;
        }
    }
    return 0;
}