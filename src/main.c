#include "convert.h"
#include <stdio.h>

int main(void) {
    int input_val;
    char input_char;
    char input_string[50];
    char binary_storage[BITS_IN_INT + 1];
    printf("Input a number in decimal, hexadecimal (starts 0x), binary (b), or ASCII (in \'\')\n");

    if (fgets(input_string, sizeof(input_string), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    if (sscanf(input_string, " '%c' ", &input_char) == 1) {
        display_telemetry(input_char);
    }

    else if (sscanf(input_string, "0x%x", &input_val) == 1) {
        display_telemetry(input_val);
    }

    else if (sscanf(input_string, "%d", &input_val) == 1) {
        display_telemetry(input_val);
    }

    else if (sscanf(input_string, "b%s", binary_storage) == 1) {
        input_val = convert_from_binary(binary_storage);
        display_telemetry(input_val);
    }

    else {
        printf("No valid input detected.");
    }
}
