long verify_only_alphabetical(char *user_input)
{
    char my_char;
    do
    {
        my_char = *user_input;
        if (my_char == '\0')
        {
            return 0;
        }
        user_input = user_input + 1;
    } while ((byte)(my_char + 0x9fU) < 0x1a);
    return -1;
}