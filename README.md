# base-convert-cli
A simple command-line tool to convert between different base and number systems (decimal, hex, binary, ascii)

## Command Demonstration
```text
% baseconv 68
Decimal            Hexadecimal            Octal            ASCII            Binary
68                 0x44                   0o104            D                1000100

% baseconv "'C'"
Decimal            Hexadecimal            Octal            ASCII            Binary
67                 0x43                   0o103            C                1000011

% baseconv 0xFF
Decimal            Hexadecimal            Octal            ASCII            Binary
255                0xff                   0o377            NonPr            11111111

% baseconv -1
Decimal            Hexadecimal            Octal            ASCII            Binary
-1                 0xffffffff             0o37777777777    Inval            11111111111111111111111111111111
```
## Input Format Specification
- Decimal (42, -42)
- Hexadecimal (0x2A, -0x2A)
- Octal (0o52, -0o52)
- Binary (b101010, -b101010)
- ASCII literals ('c')

## Build & System Installation
- Prerequisites: C99-compliant compiler (gcc or clang), GNU make.
- Compilation: make
- System Installation: sudo make install (by default installs to /usr/local/bin)
- Uninstallation: sudo make uninstall
- Local Run: make run
