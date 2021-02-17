#include <stdio.h>
#include <string.h>
// #include <stdlib.h>

// The read_user_input function looks identical to level 1
char *read_user_input(char *buffer, long max_chars, FILE *file_pointer)
{
    char char_in_buffer_at_strcspn_result;
    char *return_buffer;
    size_t strcspn_result;
    char *default_return_var;

    char newline_null[] = {0x0a, 0x00};

    if (max_chars != 0)
    {
        return_buffer = fgets(buffer, (int)max_chars, file_pointer);
        if (return_buffer != (char *)0x0)
        {
            strcspn_result = strcspn(buffer, &newline_null);
            char_in_buffer_at_strcspn_result = buffer[strcspn_result];
            buffer[strcspn_result] = '\0';
            if (char_in_buffer_at_strcspn_result == '\n')
            {
                return buffer;
            }
        }
        default_return_var = 0;
    }
    return (char *)default_return_var;
}

int verify_input(char *user_input, void *global_const, char *buffer)
{
    // scramble(user_input, &buffer);
    /* memcmp() compares first n bytes of memory areas s1 and s2 */
    return memcmp(global_const, &buffer, 0x20);
}

int main()
{
    int __status;

    // In Ghidra, the buffer starts/ends at these addresses:
    // 006010d8        ??         ??        ; start
    // 006010f9        ??         ??        ; end
    // Use these addresses to calculate the appropriate buffer size:
    int buffer_size = 0x006010f9 - 0x006010d8 + 1;
    printf("Buffer size: %d\n", buffer_size);
    char buffer[buffer_size];

    // void *memset(void *s, int c, size_t n);
    // memset() fills the first n bytes of the memory area pointed to by s with the constant byte c.
    // This is just clearing the buffer
    printf("Buffer before: {%s}\n", buffer);
    memset(&buffer, 0, 0x21);
    printf("Buffer after: {%s}\n", buffer);

    // char question_mark[] = "? ";
    printf("? ");

    char r_string[] = "r";
    FILE *file_pointer = fdopen(0, &r_string);

    char *result;
    result = read_user_input(&buffer, 0x22, file_pointer);
    printf("Buffer: {%s}\n", buffer);
    if (result != 0)
    {
        printf("Checkpoint: in `if` statement..\n");
        char code[] = "89349536319392163324855876422573";
        char buffer2[0x0060111b - 0x006010fa + 1];
        int verified = verify_input(&buffer, code, &buffer2);
        if (result == 0)
        {
            printf("! + ");
            // target_func(&buffer);
            // __status = 0;
            // goto LAB_004005c8;
        }
    }
    //   printf(&exclamation_minus);
    //   __status = 1;
    // LAB_004005c8:
    //   fclose(DAT_006010d0);
    //                     /* WARNING: Subroutine does not return */
    //   exit(__status);
}