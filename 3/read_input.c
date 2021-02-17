#include <stdio.h>

// To compile:
// gcc read_input.c -o read_input.o
unsigned long read_input(FILE *file_pointer)
{
    int result_char;
    int extraout_var;
    unsigned long char_input;
    unsigned long uVar2;
    unsigned long RDX;
    unsigned long counter;
    unsigned long uVar3;

    counter = 0;
    uVar3 = 0;
    while (1)
    {
        result_char = fgetc(file_pointer);
        char_input = result_char;
        if (char_input == 10) // 10 = 0x0a = newline
        {
            break;
        }

        counter = counter + 1;
        if (counter > 0x10) // 0x10 = 16 decimal
        {
            goto LAB_00400722;
        }

        printf("result1: %ld\n", char_input);
        char_input = char_input | 0x20; // 0x20 = 32 decimal
        printf("result2: %ld\n", char_input);
        uVar2 = char_input - 0x30;  // 0x30 = "0" ASCII
        printf("uVar2: %ld\n", uVar2);
        if (uVar2 > 9)
        {
            uVar2 = char_input - 0x57;
            printf("uVar2 part2: %ld\n", uVar2);
            // For: result - 0x61 > 5
            // 0x61   = "a" ASCII
            // 0x61+5 = 0x66 = "f" ASCII
            // possibly enter values in hex?
            if (char_input - 0x61 > 5)
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
        uVar3 = char_input;
    }
    else
    {
        RDX = 0;
    }
    return uVar3;
}

int main()
{
    printf("? ");
    FILE *file_pointer = fdopen(0, "r");
    unsigned long result = read_input(file_pointer);
    printf("read_input: %ld\n", result);
}