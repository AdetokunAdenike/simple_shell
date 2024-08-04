#include <stdio.h>

/**
 * factorial - Computes the factorial of a given number.
 * @n: The number for which to compute the factorial.
 *
 * Return: The factorial of the number, or -1 if the number is negative.
 */
int factorial(int n)
{
    if (n < 0)
    {
        return -1; /* Error case for negative numbers */
    }
    if (n == 0 || n == 1)
    {
        return 1; /* Base case: factorial of 0 or 1 is 1 */
    }
    return n * factorial(n - 1); /* Recursive case */
}

/**
 * main - Entry point of the program.
 *
 * Return: Always 0.
 */
int main(void)
{
    int number;
    int result;

    printf("Enter a number: ");
    scanf("%d", &number);

    result = factorial(number);

    if (result == -1)
    {
        printf("Factorial is not defined for negative numbers.\n");
    }
    else
    {
        printf("Factorial of %d is %d\n", number, result);
    }

    return 0;
}
