#include "utils.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void panic(char *msg, ...) {
    va_list args;
    fprintf(stderr, "Panic: ");
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    exit(1);
}
