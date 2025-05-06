#include "lexer.c"

void printToken(Token t)
{
    switch (t.type)
    {
    case TOKEN_YAP:
        printf("TOKEN_YAP\n");
        break;
    case TOKEN_ARROW:
        printf("TOKEN_ARROW\n");
        break;
    case TOKEN_STRING:
        printf("TOKEN_STRING: \"%s\"\n", t.value);
        break;
    case TOKEN_COMMENT:
        printf("TOKEN_COMMENT\n");
        break;
    case TOKEN_EOF:
        printf("TOKEN_EOF\n");
        break;
    default:
        printf("UNKNOWN TOKEN\n");
    }
}

int main()
{
    const char *code = "yap -> \"Hello World\"\0";
    const char *ptr = code;

    Token t;
    do
    {
        t = nextToken(&ptr);
        printToken(t);
        if (t.type == TOKEN_STRING)
        {
            free(t.value);
        }
    } while (t.type != TOKEN_EOF);

    return 0;
}