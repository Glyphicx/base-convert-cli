#include "convert.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf(ERROR_MESSAGE);
        return 1;
    }
    int index = 1;
    for (; index < argc; index++) {
        if (!display_telemetry(argv[index]))
            return 1;
    }
    return 0;
}
