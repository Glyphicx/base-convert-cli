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
  char formatted_str[FORMATTED_STR_MAX_LEN + 1] = {'\0'}; // Initialize array to all \0
  char *str_ptr = formatted_str;
  int base = 10;
  errno = 0;
  // Skip initial whitespace
  while (*input_str == ' ')
    input_str++;
  if (*input_str == '-') {
    *str_ptr = '-';
    str_ptr++;
    input_str++;
  }

  if (*input_str == '0') {
    // Handle single zero case
    if (*(input_str + 1) == '\0') {
      *str_ptr = '0';
    }

    else if ((*(input_str + 1) == 'x' || *(input_str + 1) == 'X') &&
             *(input_str + 2) != '\0') {
      base = 16;
      int index = 0;
      input_str += 2;
      for (; (str_ptr + index) != &formatted_str[100] && *(input_str + index) != '\0';
           index++) {
        if (isxdigit(*(input_str + index))) {
          *(str_ptr + index) = *(input_str + index);
        } else {
          formatted_str[0] = '\0';
          break;
        }
      }
    }

    else if ((*(input_str + 1) == 'o' || *(input_str + 1) == 'O') &&
             *(input_str + 2) != '\0') {
      base = 8;
      int index = 0;
      input_str += 2;
      for (; (str_ptr + index) != &formatted_str[100] && *(input_str + index) != '\0';
           index++) {
        if (*(input_str + index) >= '0' && *(input_str + index) <= '7') {
          *(str_ptr + index) = *(input_str + index);
        } else {
          formatted_str[0] = '\0';
          break;
        }
      }
    } else {
      formatted_str[0] = '\0';
    }
  }

  else if (*input_str == '\'') {
    base = 256; // Indicates ASCII format
    if (*(input_str + 1) != '\0' && *(input_str + 2) == '\'' &&
        *(input_str + 3) == '\0' && formatted_str[0] != '-') {
      *str_ptr = *(input_str + 1);
    } else {
      formatted_str[0] = '\0';
    }
  }

  else if (*input_str == '\"') {
    base = 256; // Indicates ASCII format
    if (*(input_str + 1) != '\0' && *(input_str + 2) == '\"' &&
        *(input_str + 3) == '\0' && formatted_str[0] != '-') {
      *str_ptr = *(input_str + 1);
    } else {
      formatted_str[0] = '\0';
    }
  }

  // Handle non-zero decimal cases
  else if (*input_str > '0' && *input_str <= '9') {
    int index = 0;
    for (; (str_ptr + index) != &formatted_str[100] && *(input_str + index) != '\0';
         index++) {
      if (isdigit(*(input_str + index))) {
        *(str_ptr + index) = *(input_str + index);
      } else {
        formatted_str[0] = '\0';
        break;
      }
    }
  }

  else if ((*input_str == 'b' || *input_str == 'B') && *(input_str + 1) != '\0') {
    base = 2;
    int index = 0;
    input_str += 1;
    for (; (str_ptr + index) != &formatted_str[100] && *(input_str + index) != '\0';
         index++) {
      if (*(input_str + index) == '0' || *(input_str + index) == '1') {
        *(str_ptr + index) = *(input_str + index);
      } else {
        formatted_str[0] = '\0';
        break;
      }
    }
  } else {
    formatted_str[0] = '\0';
  }

  if (formatted_str[0] == '\0') {
    fprintf(stderr, "%s", FORMATTING_ERROR_MESSAGE);
    return false;
  }

  if (formatted_str[99] != '\0') {
    fprintf(stderr, "%s", INPUT_TOO_LONG_ERROR_MESSAGE);
    return false;
  }

  else if (base == 256) {
    *output_int = formatted_str[0];
    return true;
  }

  else {
    *output_int = strtol(formatted_str, NULL, base);
    if (errno) {
      fprintf(stderr, "%s", INPUT_OUT_OF_BOUNDS_ERROR_MESSAGE);
      return false;
    }
    return true;
  }
}

bool display_telemetry(char *input_str) {
  static bool first_arg = true;
  int input_val;
  int leading_zeros;
  bool read_string_result = read_string_to_int(input_str, &input_val);

  if (read_string_result == false) {
    return false;
  }
  if (first_arg) {
    printf(HEADER_ROW);
    first_arg = false;
  }
  char binary_storage[BITS_IN_INT + 1];
  leading_zeros = convert_to_binary((uint32_t)input_val, binary_storage, BITS_IN_INT + 1);

  printf("%-*d", WHITESPACE_NUMBER + 7, input_val);   // Print decimal
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
