#ifndef CONVERT_H
#define CONVERT_H

#define BITS_IN_INT 32
#define LAST_BIT 0x80000000U

void convert_to_binary(int number, char *binary_storage);
int convert_from_binary(char binary_storage[BITS_IN_INT + 1]);
void print_binary(char binary_storage[BITS_IN_INT + 1]);
void display_telemetry(int input_val);

#endif
