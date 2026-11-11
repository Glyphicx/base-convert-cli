#ifndef CONVERT_H
#define CONVERT_H

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#define BITS_IN_INT 32
#define LAST_BIT 0x80000000U
#define FORMATTED_STR_MAX_LEN 99
#define FORMATTING_ERROR_MESSAGE                                                         \
  "Usage: baseconv <val1> [val2 ...]\nFormats: 123 (decimal), 0x123 (hex), 0o123 "       \
  "(oct), \"a\" "                                                                        \
  "(ASCII), b1001 (binary)\n"
#define INPUT_OUT_OF_BOUNDS_ERROR_MESSAGE "Inputs must be between 2^63 - 1 and -2^63\n"
#define INPUT_TOO_LONG_ERROR_MESSAGE "Inputs must not exceed 99 characters\n"
#define HEADER_ROW                                                                       \
  "Decimal" WHITESPACE "Hexadecimal" WHITESPACE "Octal" WHITESPACE "ASCII" WHITESPACE    \
  "Binary\n"

int convert_to_binary(uint32_t number, char *binary_storage, int storage_size);
int convert_from_binary(char *binary_storage);
void print_binary(char *binary_storage);
bool read_string_to_int(char *input_str, int *output_int);
bool display_telemetry(char *input_str);

#endif
