#include "parser.c"

typedef struct
{
    char *name;
    char *type;
    int intValue;
    float floatValue;
} Variable;

typedef enum
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DEFAULT
} ValueType;

typedef struct
{
    ValueType type;
    union
    {
        int intValue;
        float floatValue;
    };
} Value;

#define MAX_VARS 1024
Variable varTable[MAX_VARS];
int varCount = 0;
int showDebugging = 0;

Value evaluateNode(ASTNode *node)
{
    switch (node->type)
    {
    case NODE_YAP:
        printf("%s\n", node->value);
        break;

    case NODE_VAR_DECL:
        if (varCount >= MAX_VARS)
        {
            fprintf(stderr, "Variable table overflow\n");
            exit(1);
        }

        // add a new variable to the variable table
        Variable *v = &varTable[varCount++];
        v->name = node->name;
        v->type = node->varType;

        if (strncmp(node->varType, "int", 3) == 0)
        {
            v->intValue = atoi(node->value);
            if (showDebugging)
            {
                // confirm that we created the variable
                printf("Declared %s: %s = %d\n", v->name, v->type, v->intValue);
            }
        }
        else if (strncmp(node->varType, "float", 5) == 0)
        {
            v->floatValue = atof(node->value);
            if (showDebugging)
            {
                // confirm that we created the variable
                printf("Declared %s: %s = %f\n", v->name, v->type, v->floatValue);
            }
        }
        break;

    case NODE_DEBUGGING_SHOW:
        showDebugging = 1;
        break;

    case NODE_DEBUGGING_HIDE:
        showDebugging = 0;
        break;

    case NODE_IDENTIFIER:
        for (int i = 0; i < varCount; i++)
        {
            if (strcmp(varTable[i].name, node->value) == 0)
            {
                Value val;
                if (strncmp(varTable[i].type, "int", 3) == 0)
                {
                    val.type = TYPE_INT;
                    val.intValue = varTable[i].intValue;
                    return val;
                }
                else if (strncmp(varTable[i].type, "float", 5) == 0)
                {
                    val.type = TYPE_FLOAT;
                    val.floatValue = varTable[i].floatValue;
                    return val;
                }
            }
        }
        fprintf(stderr, "Undefined variable %s\n", node->value);
        exit(1);

    case NODE_ADDITION:
        Value leftVal = evaluateNode(node->left);
        Value rightVal = evaluateNode(node->right);
        Value result;

        if (leftVal.type == TYPE_FLOAT || rightVal.type == TYPE_FLOAT)
        {
            float left = (leftVal.type == TYPE_FLOAT) ? leftVal.floatValue : (float)leftVal.intValue;
            float right = (rightVal.type == TYPE_FLOAT) ? rightVal.floatValue : (float)rightVal.intValue;
            result.type = TYPE_FLOAT;
            result.floatValue = left + right;
            if (showDebugging)
            {
                printf("Result: %f\n", result.floatValue);
            }
        }
        else
        {
            result.type = TYPE_INT;
            result.intValue = leftVal.intValue + rightVal.intValue;
            if (showDebugging)
            {
                printf("Result: %d\n", result.intValue);
            }
        }

        return result;

    case NODE_SUBTRACTION:
        leftVal = evaluateNode(node->left);
        rightVal = evaluateNode(node->right);

        if (leftVal.type == TYPE_FLOAT || rightVal.type == TYPE_FLOAT)
        {
            float left = (leftVal.type == TYPE_FLOAT) ? leftVal.floatValue : (float)leftVal.intValue;
            float right = (rightVal.type == TYPE_FLOAT) ? rightVal.floatValue : (float)rightVal.intValue;
            result.type = TYPE_FLOAT;
            result.floatValue = left - right;
            if (showDebugging)
            {
                printf("Result: %f\n", result.floatValue);
            }
        }
        else
        {
            result.type = TYPE_INT;
            result.intValue = leftVal.intValue - rightVal.intValue;
            if (showDebugging)
            {
                printf("Result: %d\n", result.intValue);
            }
        }

        return result;

    default:
        fprintf(stderr, "Unknown expression node\n");
        exit(1);
    }

    Value empty;
    empty.type = TYPE_DEFAULT;
    return empty;
}

void executeProgram(ASTNode **nodes, int count)
{
    for (int i = 0; i < count; i++)
    {
        ASTNode *node = nodes[i];
        evaluateNode(node);
    }
}

char *readFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *buffer = malloc(length + 1);
    if (!buffer)
    {
        perror("Error allocating buffer");
        exit(1);
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s file\n", argv[0]);
        return 1;
    }

    char *code = readFile(argv[1]);
    const char *ptr = code;

    int count;
    ASTNode **program = parseProgram(&ptr, &count);
    executeProgram(program, count);

    free(code);

    return 0;
}