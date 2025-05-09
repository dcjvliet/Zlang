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