#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "helper.c"

extern Token nextToken(const char **input, int *line)
{
    // skip it if it's whitespace - this allows arbitrary whitespace
    while (isspace(**input))
        (*input)++;

    // check for newline
    if (**input == '\n')
    {
        (*line)++;
        (*input)++;
        return (Token){TOKEN_NEWLINE, NULL};
    }

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
        (*line)++;
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
            }

            (*input)++;
        }

        // if there was no closing quote
        if (**input == '\0')
        {
            PARSE_ERROR("Unclosed string", *line);
        }

        int len = *input - start;

        char *str = malloc(len + 1);
        if (!str)
        {
            PARSE_ERROR("Error allocating memory\n", -1);
        }
        strncpy(str, start, len);
        str[len] = '\0';

        (*input)++;
        (*line)++;

        return (Token){TOKEN_STRING, str};
    }

    // check for colon token
    if (**input == ':')
    {
        (*input)++;
        return (Token){TOKEN_COLON, NULL};
    }

    // check for addition token
    if (**input == '+')
    {
        (*input)++;
        return (Token){TOKEN_ADDITION, NULL};
    }

    // check for subtraction token
    if (**input == '-')
    {
        (*input)++;
        return (Token){TOKEN_SUBTRACTION, NULL};
    }

    // check for multiplication token
    if (**input == '*')
    {
        (*input)++;
        return (Token){TOKEN_MULTIPLICATION, NULL};
    }

    // check for division token
    if (**input == '/')
    {
        (*input)++;
        return (Token){TOKEN_DIVISION, NULL};
    }

    // check for the int/float literal token
    if (isdigit(**input))
    {
        const char *start = *input;
        int isDouble = 0;

        while (isdigit(**input))
        {
            (*input)++;
        }

        if (**input == '.')
        {
            isDouble = 1;
            (*input)++;

            if (!isdigit(**input))
            {
                PARSE_ERROR("Invalid float format: '.' must be followed by a number", *line);
            }

            while (isdigit(**input))
            {
                (*input)++;
            }
        }
        int len = *input - start;
        char *num = malloc(len + 1);
        if (!num)
        {
            PARSE_ERROR("Error allocating memory\n", -1);
        }
        strncpy(num, start, len);
        num[len] = '\0';
        (*line)++;
        if (isDouble)
        {
            return (Token){TOKEN_FLOAT_LITERAL, num};
        }
        else
        {
            return (Token){TOKEN_INT_LITERAL, num};
        }
    }

    // check for the name of variable (identifier token)
    // also checks for "spittin fax" for debugging
    // check if character is alphanumeric
    if (isalnum(**input))
    {
        const char *start = *input;
        while (isalnum(**input))
        {
            (*input)++;
        }
        int len = *input - start;
        char *name = malloc(len + 1);
        if (!name)
        {
            PARSE_ERROR("Error allocating memory\n", -1);
        }
        strncpy(name, start, len);
        name[len] = '\0';

        // see if it's just type hinting
        if (strncmp(name, "int", 3) == 0)
        {
            return (Token){TOKEN_TYPE, "int"};
        }
        else if (strncmp(name, "float", 5) == 0)
        {
            return (Token){TOKEN_TYPE, "float"};
        }
        // see if it is deugging message
        else if (strncmp(name, "spittin", 7) == 0)
        {
            return (Token){TOKEN_SPITTIN, NULL};
        }
        else if (strncmp(name, "fax", 3) == 0)
        {
            return (Token){TOKEN_FAX, NULL};
        }
        else if (strncmp(name, "cap", 3) == 0)
        {
            return (Token){TOKEN_CAP, NULL};
        }
        // check for is and lowkey tokens
        else if (strncmp(name, "is", 2) == 0)
        {
            return (Token){TOKEN_IS, NULL};
        }
        else if (strncmp(name, "lowkey", 6) == 0)
        {
            return (Token){TOKEN_LOWKEY, NULL};
        }
        else if (strncmp(name, "mod", 3) == 0)
        {
            return (Token){TOKEN_MODULO, NULL};
        }

        return (Token){TOKEN_IDENTIFIER, name};
    }

    // if it didn't pass any of the previous checks, it's an unknown token and we should raise an error
    return (Token){TOKEN_UNKNOWN, NULL};
}