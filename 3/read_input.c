#include <stdio.h>

unsigned long read_input(FILE *file_pointer)
{
    int result_char;
    int extraout_var;
    unsigned long result;
    unsigned long uVar2;
    unsigned long RDX;
    unsigned long counter;
    unsigned long uVar3;

    counter = 0;
    uVar3 = 0;
    while (1)
    {
        result_char = fgetc(file_pointer);
        result = result_char;
        if (result == 10) // 10 = 0x0a = newline
        {
            break;
        }

        counter = counter + 1;
        if (counter > 0x10) // 0x10 = 16 decimal
        {
            goto LAB_00400722;
        }

        printf("result1: %ld\n", result);
        result = result | 0x20;
        printf("result2: %ld\n", result);
        uVar2 = result - 0x30;
        printf("result3: %ld\n", result);
        if (uVar2 > 9)
        {
            uVar2 = result - 0x57;
            if (result - 0x61 > 5)
            {
                goto LAB_00400722;
            }
        }
        uVar3 = uVar3 << 4 | uVar2;
    }
    if (counter == 0)
    {
    LAB_00400722:
        /* Set RDX to -1 */
        RDX = 0xffffffffffffffff;
        uVar3 = result;
    }
    else
    {
        RDX = 0;
    }
    return result;
}

int main()
{
    printf("? ");
    FILE *file_pointer = fdopen(0, "r");
    unsigned long result = read_input(file_pointer);
}