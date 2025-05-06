#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"

extern Token nextToken(const char **input)
{
    // skip it if it's whitespace - this allows arbitrary whitespace
    while (isspace(**input))
        (*input)++;

    // check for EOF token
    if (**input == '\0')
    {
        return (Token){TOKEN_EOF, NULL};
    }

    // check for yap token
    // must have space before the arrow
    if (strncmp(*input, "yap", 3) == 0 && isspace((*input)[3]))
    {
        *input += 3;
        return (Token){TOKEN_YAP, NULL};
    }

    // check for arrow token
    if (strncmp(*input, "->", 2) == 0 && isspace((*input)[2]))
    {
        *input += 2;
        return (Token){TOKEN_ARROW, NULL};
    }

    // check for a comment token
    if (strncmp(*input, ":skull:", 7) == 0)
    {
        *input += 7;
        while (**input != '\n' && **input != '\0')
            (*input)++;
        return (Token){TOKEN_COMMENT, NULL};
    }

    // check if it is a string token
    if (**input == '"')
    {
        // skip the quotation mark
        (*input)++;
        // find length of the string
        const char *start = *input;
        while (**input != '\0' && **input != '"')
        {
            // if we go to a newline before closing the string or ending the file
            if (**input == '\n')
            {
                fprintf(stderr, "Unclosed string");
                exit(1);
            }

            (*input)++;
        }

        // if there was no closing quote
        if (**input == '\0')
        {
            fprintf(stderr, "Unclosed string");
            exit(1);
        }

        int len = *input - start;

        char *str = malloc(len + 1);
        strncpy(str, start, len);
        str[len] = '\0';

        (*input)++;

        return (Token){TOKEN_STRING, str};
    }

    return (Token){TOKEN_UNKNOWN, NULL};
}