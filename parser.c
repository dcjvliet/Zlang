#include "lexer.c"

typedef enum
{
    NODE_YAP,
    NODE_VAR_DECL
} ASTNodeType;

typedef struct
{
    ASTNodeType type;
    char *name;
    char *varType;
    char *value;
} ASTNode;

ASTNode *parseYap(const char **input)
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

ASTNode *parseVarDecl(const char **input)
{
    Token tok = nextToken(input);
    while (tok.type == TOKEN_COMMENT)
    {
        tok = nextToken(input);
    }

    // make sure we have a name
    if (tok.type != TOKEN_IDENTIFIER)
    {
        fprintf(stderr, "Expected a variable name\n");
        exit(1);
    }
    char *varName = tok.value;

    tok = nextToken(input);
    while (tok.type == TOKEN_COMMENT)
    {
        tok = nextToken(input);
    }

    // make sure we have a colon
    if (tok.type != TOKEN_COLON)
    {
        fprintf(stderr, "Expected a colon after the variable name\n");
        exit(1);
    }

    tok = nextToken(input);
    while (tok.type == TOKEN_COMMENT)
    {
        tok = nextToken(input);
    }

    // make sure we have a variable type
    if (tok.type != TOKEN_TYPE)
    {
        fprintf(stderr, "Expected a variable type after ':'\n");
        exit(1);
    }
    char *varType = tok.value;

    tok = nextToken(input);
    while (tok.type == TOKEN_COMMENT)
    {
        tok = nextToken(input);
    }

    // make sure we have an equals sign
    if (tok.type != TOKEN_EQUAL)
    {
        fprintf(stderr, "Expected an '=' after variable type\n");
        exit(1);
    }

    tok = nextToken(input);
    while (tok.type == TOKEN_COMMENT)
    {
        tok = nextToken(input);
    }

    // make sure we have a value for the int
    if (tok.type != TOKEN_INT_LITERAL)
    {
        fprintf(stderr, "Expected integer literal after '='\n");
        exit(1);
    }
    char *value = tok.value;

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_VAR_DECL;
    node->name = varName;
    node->varType = varType;
    node->value = value;
    return node;
}

ASTNode **parseProgram(const char **input, int *outCount)
{
    int capacity = 8;
    ASTNode **nodes = malloc(sizeof(ASTNode *) * capacity);
    int count = 0;

    while (1)
    {
        const char *start = *input;
        Token tok = nextToken(input);

        if (tok.type == TOKEN_EOF)
        {
            break;
        }
        if (tok.type == TOKEN_COMMENT)
        {
            continue;
        }

        ASTNode *stmt = NULL;
        // check for yap token
        if (tok.type == TOKEN_YAP)
        {
            *input = start;
            stmt = parseYap(input);
        }
        else if (tok.type == TOKEN_IDENTIFIER)
        {
            Token next = nextToken(input);
            if (next.type == TOKEN_COLON)
            {
                *input = start;
                stmt = parseVarDecl(input);
            }
            else
            {
                fprintf(stderr, "Unexpected identifier without type declaration\n");
                exit(1);
            }
        }
        else
        {
            fprintf(stderr, "Unexpected token at top level\n");
            exit(1);
        }

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