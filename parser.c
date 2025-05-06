#include "lexer.c"

typedef struct
{
    enum
    {
        NODE_YAP
    } type;
    char *value;
} ASTNode;

ASTNode *parsePrint(const char **input)
{
    // skip past any comments
    Token tok = nextToken(input);
    while (tok.type == TOKEN_COMMENT)
    {
        tok = nextToken(input);
    }

    // make sure we have a print
    if (tok.type != TOKEN_YAP)
    {
        fprintf(stderr, "Expected 'yap' at start of line\n");
        exit(1);
    }

    tok = nextToken(input);
    while (tok.type == TOKEN_COMMENT)
    {
        tok = nextToken(input);
    }
    // make sure we have an arrow
    if (tok.type != TOKEN_ARROW)
    {
        fprintf(stderr, "Expected '->' after 'print'\n");
        exit(1);
    }

    tok = nextToken(input);
    while (tok.type == TOKEN_COMMENT)
    {
        tok = nextToken(input);
    }
    // make sure we have something to print
    if (tok.type != TOKEN_STRING)
    {
        fprintf(stderr, "Expected string after '->'\n");
        exit(1);
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_YAP;
    node->value = tok.value;
    return node;
}

ASTNode **parseProgram(const char **input, int *outCount)
{
    int capacity = 8;
    ASTNode **nodes = malloc(sizeof(ASTNode *) * capacity);
    int count = 0;

    while (1)
    {
        Token tok = nextToken(input);
        if (tok.type == TOKEN_EOF)
        {
            break;
        }
        if (tok.type == TOKEN_COMMENT)
        {
            continue;
        }

        // move back 3 because it was a yap token
        *input -= 3;
        ASTNode *stmt = parsePrint(input);

        if (count >= capacity)
        {
            capacity *= 2;
            nodes = realloc(nodes, sizeof(ASTNode *) * capacity);
        }

        nodes[count++] = stmt;
    }

    *outCount = count;
    return nodes;
}