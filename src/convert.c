#include "convert.h"
#include <ctype.h>
#include <stdio.h>

void convert_to_binary(int number, char *binary_storage) {
    int bit_index = 0;
    for (; bit_index < BITS_IN_INT; bit_index++) {
        if (number & (LAST_BIT >> bit_index)) {
            binary_storage[bit_index] = (char)'1';
        } else {
            binary_storage[bit_index] = (char)'0';
        }
    }
    binary_storage[bit_index] = '\0';
}

int convert_from_binary(char binary_storage[BITS_IN_INT + 1]) {
    int bit_index = 0;
    int value = 0;
    for (; binary_storage[bit_index] != '\0' && binary_storage[bit_index] != ' '; bit_index++) {
        value *= 2;
        if (binary_storage[bit_index] == '1') {
            value += 1;
        }
    }
    return value;
}

void print_binary(char binary_storage[BITS_IN_INT + 1]) {
    int bit_index = 0;
    while (bit_index < (BITS_IN_INT - 1) && binary_storage[bit_index] == '0') {
        bit_index++;
    }
    for (; bit_index < BITS_IN_INT; bit_index++)
        printf("%c", binary_storage[bit_index]);
}

void display_telemetry(int input_val) {
    char binary_storage[BITS_IN_INT + 1];
    convert_to_binary(input_val, binary_storage);

    printf("Decimal  Hexadecimal  ASCII  Binary\n");
    if (isprint(input_val)) {
        printf("%-7d  0x%-9x  %-5c  ", input_val, (unsigned int)input_val, input_val);
    } else {
        printf("%-7d  0x%-9x  Inval  ", input_val, (unsigned int)input_val);
    }

    print_binary(binary_storage);
    printf("\n");
}
