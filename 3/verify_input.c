#include <stdio.h>


// To compile
// gcc verify_input.c -o verify_input.o
long verify_input(int a, int b, int c)
{
    long result;

    // Equation 1
    result = (a + c) * 2 + b * 3 - 1;    

    // ********************
    // Comma Operator in C
    // ********************
    // The expression: (expression1,  expression2)
    // First expression1 is evaluated, then expression2 is evaluated,
    // and the value of expression2 is returned for the whole expression.
    if (result == 0)
    {
        // Equation 2
        result = ((b + a * 2) * 2 + c * 3) - 1;
        if (result == 0)
        {
            // Equation 3
            result = (a * 9 + c * 7 + b * 6) - 1;
            printf("INPUT VERIFIED!\n");
        }
    }

    // The return value needs to equal zero as well
    return result;
}

int main()
{
    long result = verify_input(-8, -3, 13);
    printf("result: %ld\n", result);
}