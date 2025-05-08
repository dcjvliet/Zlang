// define our token types
typedef enum
{
    // common use tokens
    TOKEN_COMMENT, // start of comment
    TOKEN_UNKNOWN, // unknown token
    TOKEN_EOF,     // end of file token
    TOKEN_SPITTIN, // start of debug token
    TOKEN_FAX,     // second part of debug token
    TOKEN_CAP,     // token to turn off debug

    // tokens for printing
    TOKEN_YAP,    // print
    TOKEN_ARROW,  // syntax for message to print
    TOKEN_STRING, // message to print

    // tokens for declaring variables
    TOKEN_IDENTIFIER,    // variable name
    TOKEN_COLON,         // :
    TOKEN_TYPE,          // type of the variable
    TOKEN_IS,            // is
    TOKEN_LOWKEY,        // lowkey; combined with 'is' is equivalent to =
    TOKEN_INT_LITERAL,   // value of an int type
    TOKEN_FLOAT_LITERAL, // value of a float type

    // tokens for doing simple arithmetic (5 main operations)
    TOKEN_ADDITION,       // +
    TOKEN_SUBTRACTION,    // -
    TOKEN_MULTIPLICATION, // *
    TOKEN_DIVISION,       // /
    TOKEN_MODULO          // %
} TokenType;

// define token
typedef struct
{
    TokenType type;
    char *value;
} Token;
