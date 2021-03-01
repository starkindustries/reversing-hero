#include <stdio.h>
#include <assert.h>
#include "helper.h"

typedef unsigned long ulong;

long target_0xff(long all_ones_buffer, ulong max_0x1a, char user_input)
{
    char bVar1;
    long lVar2;
    ulong counter;

    counter = 0;
    do
    {
        bVar1 = *(char *)(all_ones_buffer + counter);
        if ((bVar1 == 0xff) || (user_input == bVar1))
        {
            return all_ones_buffer + counter;
        }
        lVar2 = counter * 2;
        counter = lVar2 + 1;
        if (bVar1 <= user_input)
        {
            counter = lVar2 + 2;
        }
    } while (counter < max_0x1a);
    return 0;
}

long target_return_zero2(long all_ones_buffer, ulong counter, char user_input)
{
    char *result;
    long return_value;

    result = (char *)target_0xff(all_ones_buffer, counter, user_input);
    /* if result is zero or not 0xff */
    if ((result == (char *)0x0) || (*result != 0xff))
    {
        return_value = -1;
    }
    else
    {
        /* Goal to get here */
        *result = user_input;
        return_value = 0;
    }
    return return_value;
}

long verify_only_alphabetical(char *user_input)
{
    char my_char;
    do
    {
        my_char = *user_input;
        debug("verify, my_char{%c}\n", my_char);
        if (my_char == '\0')
        {
            return 0;
        }
        user_input = user_input + 1;
        debug("verify, my_char-0x61:{0x%02x}\n", (unsigned char)(my_char-0x61));
    } while ((unsigned char)(my_char - 0x61) <= (unsigned char)0x19);
    return -1;
}

int main()
{
    assert ((unsigned char)0xff > (unsigned char)0x60);
    debug("HELLO_DEBUGGER!!\n");    

    printf("hello_world\n");

    char buffer[] = "_abcdefghijklmnopqrstuvwxyz";
    long result = verify_only_alphabetical(buffer);
    printf("verify_only_alphabetical: %ld\n", result);
}