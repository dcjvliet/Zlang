#include "lexer.c"

typedef enum
{
    NODE_YAP,
    NODE_VAR_DECL,
    NODE_DEBUGGING_SHOW,
    NODE_DEBUGGING_HIDE,
    NODE_ADDITION,
    NODE_SUBTRACTION,
    NODE_MULTIPLICATION,
    NODE_DIVISION,
    NODE_MODULO,
    NODE_INT_LITERAL,
    NODE_FLOAT_LITERAL,
    NODE_IDENTIFIER
} ASTNodeType;

typedef struct ASTNode
{
    ASTNodeType type;

    // for variable declarations
    char *name;
    char *varType;
    char *value;

    // for binary operations
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

Token skipComments(const char **input)
{
    Token tok = nextToken(input);
    while (tok.type == TOKEN_COMMENT)
    {
        tok = nextToken(input);
    }

    return tok;
}

ASTNode *parseYap(const char **input)
{
    // skip past any comments
    Token tok = skipComments(input);

    // make sure we have a print
    if (tok.type != TOKEN_YAP)
    {
        fprintf(stderr, "Expected 'yap' at start of line\n");
        exit(1);
    }

    tok = skipComments(input);

    // make sure we have an arrow
    if (tok.type != TOKEN_ARROW)
    {
        fprintf(stderr, "Expected '->' after 'yap'\n");
        exit(1);
    }

    tok = skipComments(input);

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
    Token tok = skipComments(input);

    // make sure we have a name
    if (tok.type != TOKEN_IDENTIFIER)
    {
        fprintf(stderr, "Expected a variable name\n");
        exit(1);
    }
    char *varName = tok.value;

    tok = skipComments(input);

    // make sure we have a colon
    if (tok.type != TOKEN_COLON)
    {
        fprintf(stderr, "Expected a colon after the variable name\n");
        exit(1);
    }

    tok = skipComments(input);

    // make sure we have a variable type
    if (tok.type != TOKEN_TYPE)
    {
        fprintf(stderr, "Expected a variable type after ':'\n");
        exit(1);
    }
    char *varType = tok.value;

    tok = skipComments(input);

    // make sure we have an 'is'
    if (tok.type != TOKEN_IS)
    {
        fprintf(stderr, "Expected an 'is' after variable type\n");
        exit(1);
    }

    tok = skipComments(input);

    // make sure we have a 'lowkey'
    if (tok.type != TOKEN_LOWKEY)
    {
        fprintf(stderr, "Expected a 'lowkey' after 'is'\n");
        exit(1);
    }

    tok = skipComments(input);

    // make sure we have a value for variable
    if ((tok.type != TOKEN_INT_LITERAL && strncmp(varType, "int", 3) == 0) || (tok.type != TOKEN_FLOAT_LITERAL && strncmp(varType, "float", 5) == 0 && tok.type != TOKEN_INT_LITERAL))
    {
        fprintf(stderr, "Expected variable literal after 'is lowkey'\n");
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

ASTNode *parseDebuggingShow(const char **input)
{
    Token tok = skipComments(input);

    if (tok.type != TOKEN_SPITTIN)
    {
        fprintf(stderr, "Expected 'spittin'\n");
        exit(1);
    }

    tok = skipComments(input);

    if (tok.type != TOKEN_FAX)
    {
        fprintf(stderr, "Expected 'fax' after 'spittin'\n");
        exit(1);
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_DEBUGGING_SHOW;
    return node;
}

ASTNode *parseDebuggingHide(const char **input)
{
    Token tok = skipComments(input);

    if (tok.type != TOKEN_SPITTIN)
    {
        fprintf(stderr, "Expected 'spittin'\n");
        exit(1);
    }

    tok = skipComments(input);

    if (tok.type != TOKEN_CAP)
    {
        fprintf(stderr, "Expected 'cap' after 'spittin'\n");
        exit(1);
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_DEBUGGING_HIDE;
    return node;
}

ASTNode *parseAddition(const char **input)
{
    Token tok = skipComments(input);

    if (tok.type != TOKEN_IDENTIFIER && tok.type != TOKEN_INT_LITERAL && tok.type != TOKEN_FLOAT_LITERAL)
    {
        fprintf(stderr, "Expected a variable identifier or literal at the beginning of a binary expression\n");
        exit(1);
    }
    char *leftValue = tok.value;
    TokenType leftType = tok.type;

    tok = skipComments(input);

    if (tok.type != TOKEN_ADDITION)
    {
        fprintf(stderr, "Expected '+' after a variable identifier or literal\n");
        exit(1);
    }

    tok = skipComments(input);

    if (tok.type != TOKEN_IDENTIFIER && tok.type != TOKEN_INT_LITERAL && tok.type != TOKEN_FLOAT_LITERAL)
    {
        fprintf(stderr, "Expected a variable identifier or literal after '+'\n");
        exit(1);
    }
    char *rightValue = tok.value;
    TokenType rightType = tok.type;

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_ADDITION;
    ASTNode *left = malloc(sizeof(ASTNode));
    if (leftType == TOKEN_IDENTIFIER)
    {
        left->type = NODE_IDENTIFIER;
    }
    else if (leftType == TOKEN_INT_LITERAL)
    {
        left->type = NODE_INT_LITERAL;
    }
    else if (leftType == TOKEN_FLOAT_LITERAL)
    {
        left->type = NODE_FLOAT_LITERAL;
    }
    left->value = leftValue;

    ASTNode *right = malloc(sizeof(ASTNode));
    if (rightType == TOKEN_IDENTIFIER)
    {
        right->type = NODE_IDENTIFIER;
    }
    else if (rightType == TOKEN_INT_LITERAL)
    {
        right->type = NODE_INT_LITERAL;
    }
    else if (rightType == TOKEN_FLOAT_LITERAL)
    {
        right->type = NODE_FLOAT_LITERAL;
    }
    right->value = rightValue;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *parseSubtraction(const char **input)
{
    Token tok = skipComments(input);

    if (tok.type != TOKEN_IDENTIFIER && tok.type != TOKEN_INT_LITERAL && tok.type != TOKEN_FLOAT_LITERAL)
    {
        fprintf(stderr, "Expected a variable identifier or literal at the beginning of a binary expression\n");
        exit(1);
    }
    char *leftValue = tok.value;
    TokenType leftType = tok.type;

    tok = skipComments(input);

    if (tok.type != TOKEN_SUBTRACTION)
    {
        fprintf(stderr, "Expected '-' after a variable identifier or literal\n");
        exit(1);
    }

    tok = skipComments(input);

    if (tok.type != TOKEN_IDENTIFIER && tok.type != TOKEN_INT_LITERAL && tok.type != TOKEN_FLOAT_LITERAL)
    {
        fprintf(stderr, "Expected a variable identifier or literal after '+'\n");
        exit(1);
    }
    char *rightValue = tok.value;
    TokenType rightType = tok.type;

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_SUBTRACTION;
    ASTNode *left = malloc(sizeof(ASTNode));
    if (leftType == TOKEN_IDENTIFIER)
    {
        left->type = NODE_IDENTIFIER;
    }
    else if (leftType == TOKEN_INT_LITERAL)
    {
        left->type = NODE_INT_LITERAL;
    }
    else if (leftType == TOKEN_FLOAT_LITERAL)
    {
        left->type = NODE_FLOAT_LITERAL;
    }
    left->value = leftValue;

    ASTNode *right = malloc(sizeof(ASTNode));
    if (rightType == TOKEN_IDENTIFIER)
    {
        right->type = NODE_IDENTIFIER;
    }
    else if (rightType == TOKEN_INT_LITERAL)
    {
        right->type = NODE_INT_LITERAL;
    }
    else if (rightType == TOKEN_FLOAT_LITERAL)
    {
        right->type = NODE_FLOAT_LITERAL;
    }
    right->value = rightValue;
    node->left = left;
    node->right = right;
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
            else if (next.type == TOKEN_ADDITION)
            {
                *input = start;
                stmt = parseAddition(input);
            }
            else if (next.type == TOKEN_SUBTRACTION)
            {
                *input = start;
                stmt = parseSubtraction(input);
            }
            else
            {
                fprintf(stderr, "Unexpected identifier without type declaration\n");
                exit(1);
            }
        }
        else if (tok.type == TOKEN_SPITTIN)
        {
            Token next = nextToken(input);
            if (next.type == TOKEN_FAX)
            {
                *input = start;
                stmt = parseDebuggingShow(input);
            }
            else if (next.type == TOKEN_CAP)
            {
                *input = start;
                stmt = parseDebuggingHide(input);
            }
            else
            {
                fprintf(stderr, "Unexpected 'spittin' without following 'fax'\n");
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