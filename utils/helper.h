#ifndef HELPER_H
#define HELPER_H

#define DEBUG 1

void debug(const char *format, ...);

void print_long_array(long *arr, int length);

void print_char_array(char *arr, int length);

int compare_char_array(char *a, char *b, int length);

#endif // HELPER_H