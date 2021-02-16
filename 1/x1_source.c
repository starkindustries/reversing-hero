#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *read_file(char *buffer, long max_chars, FILE *file_pointer)
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

int main()
{
    int __status;
    char *return_buffer;

    char question_mark[] = "? ";
    char exclamation_plus[] = "! + ";
    char exclamation_minus[] = "! - \n";
    char r_string[] = "r";
    char buffer[0x1b];
    
    // This is the string that the program compares the user input to
    // Same string w/o double backslash: 
    // d(-_-)b//d(+_+)b\\d(-_-)b
    // Above is easier to copy/paste
    char emoji_string[] = "d(-_-)b//d(+_+)b\\\\d(-_-)b";    

    printf(&question_mark);
    FILE *file_pointer = fdopen(0, &r_string);
    return_buffer = (char *)read_file(&buffer, 0x1b, file_pointer);
    if (return_buffer != (char *)0x0)
    {
        __status = strcmp(&buffer, emoji_string);
        if (__status == 0)
        {
            printf(&exclamation_plus);
            printf("** Target Function **\n");
            // target_func(&global_buffer);
            fclose(file_pointer);
            return 0;
        }
    }
    printf(&exclamation_minus);
    fclose(file_pointer);
    return 0;
}