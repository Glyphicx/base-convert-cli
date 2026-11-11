# base-convert-cli

A simple command-line tool to convert between different base and number systems (decimal,
hex, binary, ascii)

# Specification

### Invocation

Base-convert can be invoked using the command `baseconv {input}` Base-convert accepts a
minimum of one input (no maximum) containing either a numerical value in hex octal, or
decimal, or a single ASCII character and outputs a table with a header indicating the
number format of each column and the inputted argument translated into a row of equivalent
values in different number formats. If multiple arguments are inputted, permitting that
they are all in a valid format, base-convert treats each one as a separate call and
outputs each as a row in the conversion table.

### Accepted Inputs

Base-convert accepts input in five base formats. Decimal is inputted as an integer
(leading zeros are not permitted), binary is prefixed with either a b or a B (b1001 or
B1001), hexadecimal is prefixed with either 0x or 0X (0xFF or 0XFF), octal with 0o or 0O
(0o772 or 0O772), and ascii must be inputted in the shell as a single character wrapped in
two layers of matching quotes, either double-single "'C'" or single-double '"A"', as the
first layer is stripped away as a shell quote delimiter, such that the program recieves
only the character and inner layer of quotes. All formats of input except ASCII allow a -
sign to be placed at the start of the input (before any prefixes). Inputs must not exceed
99 characters in length, not counting base prefixes.

### Value Boundaries and Wrapping

Base-convert accepts values corresponding to integers within the bounds of a signed 64 bit
integer as inputs, but can only display signed 32 bit integer values (between -2147483648
and 2147483647). Any input exceeding the input bounds will throw an error, while any input
exceeding the output bounds will result in an overflow, with any values exceeding the
limit wrapping around starting from the opposite limit. The output columns, in order,
represent the inputted value in:

- Decimal form (signed, inputted leading 0s render input invalid)
- Hexadecimal form (unsigned, wraps around starting from 0xffffffff on negative input,
  discards inputted leading 0s)
- Ocal form (unsigned, wraps around starting from 0o37777777777 on negative input,
  discards inputted leading 0s)
- ASCII form (unsigned, returns as Inval (invalid) if input is negative or greater than
  255, and NonPr (non printable) if the character corresponding to the inputted value is
  not printable)
- Binary form (unisgned, wraps around starting from 11111111111111111111111111111111 on
  negative input, discards inputted leading 0s)

### Example Output and Error Message

A standard output appears in the following format:

```
Decimal            Hexadecimal            Octal            ASCII            Binary
33                 0x21                   0o41             !                100001
```

The following standard error appears in the event that an input does not follow any of the
permitted input formats.

```
Usage: baseconv <val1> [val2 ...]
Formats: 123 (decimal), 0x123 (hex), 0o123 (oct), "a" (ASCII), b1001 (binary)
```

In the event that the initial argument passes but a following one does not, all arguments
before the invalid one result in a standard output, the invalid one triggers a standard
error message, and all following arguments are scipped. Base-convert will exit with a
status of 0 if all passed in arguments are valid, and an exit status of 1 if any arguments
are invalid.

### Additional Examples

```
baseconv '"F"'
Decimal            Hexadecimal            Octal            ASCII            Binary
70                 0x46                   0o106            F                1000110
```

```
baseconv '"cat"'
Usage: baseconv <val1> [val2 ...]
Formats: 123 (decimal), 0x123 (hex), 0o123 (oct), "a" (ASCII), b1001 (binary)
```

```
baseconv B001001
Decimal            Hexadecimal            Octal            ASCII            Binary
9                  0x9                    0o11             NonPr            1001
```

```
baseconv 2147483648
Decimal            Hexadecimal            Octal            ASCII            Binary
-2147483648        0x80000000             0o20000000000    Inval            10000000000000000000000000000000
```

# Build & System Installation

- Prerequisites: C99-compliant compiler (gcc or clang), GNU make.
- Compilation: make
- System Installation: sudo make install (by default installs to /usr/local/bin)
- Uninstallation: sudo make uninstall
- Local Run: make run
