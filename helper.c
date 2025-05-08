#include <stdio.h>
#include <stdarg.h>

extern void PARSE_ERROR(const char *msg, int currentLine, ...)
{
    va_list args;
    va_start(args, currentLine);

    fprintf(stderr, "Error on line %d:\n\t", currentLine);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    exit(1);
}