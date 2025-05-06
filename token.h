// define our token types
typedef enum
{
    TOKEN_YAP,     // print
    TOKEN_ARROW,   // syntax for message to print
    TOKEN_STRING,  // message to print
    TOKEN_COMMENT, // start of comment
    TOKEN_UNKNOWN, // unknown token
    TOKEN_EOF      // end of file token
} TokenType;

// define token
typedef struct
{
    TokenType type;
    char *value;
} Token;
