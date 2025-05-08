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
    int line;

    // for variable declarations
    char *name;
    char *varType;
    char *value;

    // for binary operations
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

int currentLine = 1;

Token skipComments(const char **input)
{
    Token tok = nextToken(input, &currentLine);
    while (tok.type == TOKEN_COMMENT)
    {
        tok = nextToken(input, &currentLine);
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
        PARSE_ERROR("Expected 'yap' at start of line\n", currentLine);
    }

    tok = skipComments(input);

    // make sure we have an arrow
    if (tok.type != TOKEN_ARROW)
    {
        PARSE_ERROR("Expected '->' after 'yap'\n", currentLine);
    }

    tok = skipComments(input);

    // make sure we have something to print
    if (tok.type != TOKEN_STRING)
    {
        PARSE_ERROR("Expected string after '->'\n", currentLine);
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_YAP;
    node->value = tok.value;
    node->line = currentLine;
    return node;
}

ASTNode *parseVarDecl(const char **input)
{
    Token tok = skipComments(input);

    // make sure we have a name
    if (tok.type != TOKEN_IDENTIFIER)
    {
        PARSE_ERROR("Expected a variable name at the begging of a variable declaration\n", currentLine);
    }
    char *varName = tok.value;

    tok = skipComments(input);

    // make sure we have a colon
    if (tok.type != TOKEN_COLON)
    {
        PARSE_ERROR("Expected a colon after a variable identifier\n", currentLine);
    }

    tok = skipComments(input);

    // make sure we have a variable type
    if (tok.type != TOKEN_TYPE)
    {
        PARSE_ERROR("Expected a variable type after ':'\n", currentLine);
    }
    char *varType = tok.value;

    tok = skipComments(input);

    // make sure we have an 'is'
    if (tok.type != TOKEN_IS)
    {
        PARSE_ERROR("Expected an 'is' after variable type\n", currentLine);
    }

    tok = skipComments(input);

    // make sure we have a 'lowkey'
    if (tok.type != TOKEN_LOWKEY)
    {
        PARSE_ERROR("Expected a 'lowkey' after 'is'\n", currentLine);
    }

    tok = skipComments(input);

    // make sure we have a value for variable
    if ((tok.type != TOKEN_INT_LITERAL && strncmp(varType, "int", 3) == 0) || (tok.type != TOKEN_FLOAT_LITERAL && strncmp(varType, "float", 5) == 0 && tok.type != TOKEN_INT_LITERAL))
    {
        PARSE_ERROR("Expected variable literal after 'is lowkey'\n", currentLine);
    }
    char *value = tok.value;

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_VAR_DECL;
    node->name = varName;
    node->varType = varType;
    node->value = value;
    node->line = currentLine;
    return node;
}

ASTNode *parseDebuggingShow(const char **input)
{
    Token tok = skipComments(input);

    if (tok.type != TOKEN_SPITTIN)
    {
        PARSE_ERROR("Expected 'spittin' at the begginning of a debug statement\n", currentLine);
    }

    tok = skipComments(input);

    if (tok.type != TOKEN_FAX)
    {
        PARSE_ERROR("Expected 'fax' after 'spittin'\n", currentLine);
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_DEBUGGING_SHOW;
    node->line = currentLine;
    return node;
}

ASTNode *parseDebuggingHide(const char **input)
{
    Token tok = skipComments(input);

    if (tok.type != TOKEN_SPITTIN)
    {
        PARSE_ERROR("Expected 'spittin' at the begginning of a debug statement\n", currentLine);
    }

    tok = skipComments(input);

    if (tok.type != TOKEN_CAP)
    {
        PARSE_ERROR("Expected 'cap' after 'spittin'\n", currentLine);
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_DEBUGGING_HIDE;
    node->line = currentLine;
    return node;
}

ASTNode *parseAddition(const char **input)
{
    Token tok = skipComments(input);

    if (tok.type != TOKEN_IDENTIFIER && tok.type != TOKEN_INT_LITERAL && tok.type != TOKEN_FLOAT_LITERAL)
    {
        PARSE_ERROR("Expected a variable identifier or literal at the beginning of addition\n", currentLine);
    }
    char *leftValue = tok.value;
    TokenType leftType = tok.type;

    tok = skipComments(input);

    if (tok.type != TOKEN_ADDITION)
    {
        PARSE_ERROR("Expected '+' after a variable identifier or literal\n", currentLine);
    }

    tok = skipComments(input);

    if (tok.type != TOKEN_IDENTIFIER && tok.type != TOKEN_INT_LITERAL && tok.type != TOKEN_FLOAT_LITERAL)
    {
        PARSE_ERROR("Expected a variable identifier or literal after '+'\n", currentLine);
    }
    char *rightValue = tok.value;
    TokenType rightType = tok.type;

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_ADDITION;
    node->line = currentLine;
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
    left->line = currentLine;

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
    right->line = currentLine;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *parseSubtraction(const char **input)
{
    Token tok = skipComments(input);

    if (tok.type != TOKEN_IDENTIFIER && tok.type != TOKEN_INT_LITERAL && tok.type != TOKEN_FLOAT_LITERAL)
    {
        PARSE_ERROR("Expected a variable identifier or literal at the beginning of subtraction\n", currentLine);
    }
    char *leftValue = tok.value;
    TokenType leftType = tok.type;

    tok = skipComments(input);

    if (tok.type != TOKEN_SUBTRACTION)
    {
        PARSE_ERROR("Expected '-' after a variable identifier or literal\n", currentLine);
    }

    tok = skipComments(input);

    if (tok.type != TOKEN_IDENTIFIER && tok.type != TOKEN_INT_LITERAL && tok.type != TOKEN_FLOAT_LITERAL)
    {
        PARSE_ERROR("Expected a variable identifier or literal after '-'\n", currentLine);
    }
    char *rightValue = tok.value;
    TokenType rightType = tok.type;

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_SUBTRACTION;
    node->line = currentLine;
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
    left->line = currentLine;

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
    right->line = currentLine;
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
        Token tok = nextToken(input, &currentLine);

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
            Token next = nextToken(input, &currentLine);
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
                PARSE_ERROR("Unexpected variable identifier without type declaration\n", currentLine);
            }
        }
        else if (tok.type == TOKEN_SPITTIN)
        {
            Token next = nextToken(input, &currentLine);
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
                PARSE_ERROR("Unexpected 'spittin' without following 'fax' or 'cap'\n", currentLine);
            }
        }
        else
        {
            PARSE_ERROR("Unexpected token at top level\n", currentLine);
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