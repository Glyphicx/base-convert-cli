#include "convert.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define WHITESPACE_NUMBER 12
#define WHITESPACE "            "

int convert_to_binary(uint32_t number, char *binary_storage, int storage_size) {
    bool first_one_is_placed = false;
    int leading_zeros = 0;
    if (binary_storage == NULL || storage_size <= 0)
        return 0; // Handle NULL pointer & 0 length binary_storage
    int bit_index = 0;
    while (bit_index < (storage_size - 1) && bit_index < BITS_IN_INT) {
        if (number & (LAST_BIT >> bit_index)) {
            binary_storage[bit_index] = '1';
            first_one_is_placed = true;
        } else {
            if (!first_one_is_placed)
                leading_zeros++;
            binary_storage[bit_index] = '0';
        }
        bit_index++;
    }
    binary_storage[bit_index] = '\0';
    if (number == 0)
        return leading_zeros - 1;
    return leading_zeros;
}

bool read_string_to_int(char *input_str, int *output_int) {
    // Skip initial whitespace
    while (*input_str == ' ')
        input_str++;
    char *str_start = input_str;
    char *str_end;

    switch (*str_start) {
    case '\"':
    case '\'':
        if ((*(str_start + 2) == '\'' || *(str_start + 2) == '\"') && *(str_start + 3) == '\0') {
            *output_int = (int)(*(str_start + 1));
            return true;
        } else
            return false; // Handle error
    case '-':
        switch (*(++str_start)) {
        case '0':
            if (*(str_start + 1) == ' ' || *(str_start + 1) == '\0') {
                *output_int = 0;
                return true;
            }
            if (*(str_start + 1) == 'x' || *(str_start + 1) == 'X') {
                *output_int = -strtol((str_start + 2), &str_end, 16);
                return (str_end > (str_start + 2) && *str_end == '\0');
            }
            if (*(str_start + 1) == 'o' || *(str_start + 1) == 'O') {
                *output_int = -strtol((str_start + 2), &str_end, 8);
                return (str_end > (str_start + 2) && *str_end == '\0');
            } else
                return false; // Handle error
        case 'b':
        case 'B':
            if (*(str_start + 1) == '0' || *(str_start + 1) == '1') {
                *output_int = -strtol((str_start + 1), &str_end, 2);
                return (str_end > (str_start + 1) && *str_end == '\0');
            } else
                return false; // Handle error
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            *output_int = -strtol(str_start, &str_end, 10);
            return (str_end > (str_start) && *str_end == '\0');
        default:
            return false; // Handle error
        }
    case '0':
        if (*(str_start + 1) == ' ' || *(str_start + 1) == '\0') {
            *output_int = 0;
            return true;
        }
        if (*(str_start + 1) == 'x' || *(str_start + 1) == 'X') {
            *output_int = strtol((str_start + 2), &str_end, 16);
            return (str_end > (str_start + 2) && *str_end == '\0');
        }
        if (*(str_start + 1) == 'o' || *(str_start + 1) == 'O') {
            *output_int = strtol((str_start + 2), &str_end, 8);
            return (str_end > (str_start + 2) && *str_end == '\0');
        } else
            return false; // Handle error
    case 'b':
    case 'B':
        if (*(str_start + 1) == '0' || *(str_start + 1) == '1') {
            *output_int = strtol((str_start + 1), &str_end, 2);
            return (str_end > (str_start + 1) && *str_end == '\0');
        } else
            return false; // Handle error
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        *output_int = strtol(str_start, &str_end, 10);
        return (str_end > (str_start) && *str_end == '\0');
    default:
        return false; // Handle error
    }
}

bool display_telemetry(char *input_str) {
    int input_val;
    int leading_zeros;
    bool read_string_result = read_string_to_int(input_str, &input_val);

    if (read_string_result == false) {
        printf(ERROR_MESSAGE);
        return false;
    }
    char binary_storage[BITS_IN_INT + 1];
    leading_zeros = convert_to_binary((uint32_t)input_val, binary_storage, BITS_IN_INT + 1);

    printf("Decimal" WHITESPACE "Hexadecimal" WHITESPACE "Octal" WHITESPACE "ASCII" WHITESPACE
           "Binary\n"); // Print table header

    printf("%-7d" WHITESPACE, input_val);               // Print decimal
    printf("0x%-*x", WHITESPACE_NUMBER + 9, input_val); // Print hex
    printf("0o%-*o", WHITESPACE_NUMBER + 3, input_val); // Print oct
    if (input_val < 0 || input_val > 255) {             // Print ascii
        printf("Inval" WHITESPACE);
    } else if (!isprint(input_val))
        printf("NonPr" WHITESPACE);
    else
        printf("%-5c" WHITESPACE, input_val);

    printf("%s\n", &binary_storage[leading_zeros]);
    return true;
}
