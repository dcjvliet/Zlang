#include "parser.c"

typedef struct
{
    char *name;
    char *type;
    int intValue;
    float floatValue;
} Variable;

#define MAX_VARS 1024
Variable varTable[MAX_VARS];
int varCount = 0;
int showDebugging = 0;

void executeProgram(ASTNode **nodes, int count)
{
    for (int i = 0; i < count; i++)
    {
        ASTNode *node = nodes[i];
        if (node->type == NODE_YAP)
        {
            printf("%s\n", node->value);
        }
        else if (node->type == NODE_VAR_DECL)
        {
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
        }
        else if (node->type == NODE_DEBUGGING_SHOW)
        {
            showDebugging = 1;
        }
        else if (node->type == NODE_DEBUGGING_HIDE)
        {
            showDebugging = 0;
        }
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