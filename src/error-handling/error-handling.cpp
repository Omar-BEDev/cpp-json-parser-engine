#include "error-handling.h"
#include <iostream>
#include <string_view>
#include <unordered_map>

std::unordered_map<ErrorType, std::string_view> errorMessage {
    {INVALID_ESCAPE_SEQUENCE, "invalid escape sequence"},
    {INVALID_HEX_VALUE, "invalid hex value"},
};

void print_error_message(ErrorType errorType, int line, int column) {
    auto it = errorMessage.find(errorType);
    if (it == errorMessage.end()) {
        printf("undefined error");
        return;
    }
    printf("%s [Ln %d ,Col  %d]", it->second.data(), line, column);
}