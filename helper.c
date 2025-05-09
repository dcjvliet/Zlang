#include <stdio.h>
#include <stdarg.h>

extern void PARSE_ERROR(const char *msg, int currentLine, ...)
{
    va_list args;
    va_start(args, currentLine);

    // case for if we don't want a line number in the error
    if (currentLine < 0)
    {
        fprintf(stderr, msg, args);
        exit(1);
    }

    fprintf(stderr, "Error on line %d:\n\t", currentLine);
    vfprintf(stderr, msg, args);
    exit(1);
}

extern float modulo(float a, float b)
{
    int quotient = (int)(a / b);
    return a - (b * quotient);
}