#ifndef ERRORHANDLING
#define ERRORHANDLING
typedef enum {
   INVALID_ESCAPE_SEQUENCE,
   INVALID_HEX_VALUE
} ErrorType ;

void print_error_message(ErrorType errorType, int line, int column);
#endif