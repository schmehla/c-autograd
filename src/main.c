#include "operations.h"
#include "optimizer.h"
#include <stdio.h>

int main(void) {
    char expr[] = "(x-2)*(x-3)*x + (y-10)*y";

    VarValue v[] = {{.name = "x", .value = 1}, {.name = "y", .value = 1}};
    VarValues var_values = {.arr = v, .len = 2};

    float min = optim(expr, &var_values);
    for (size_t i = 0; i < var_values.len; ++i) {
        printf("%s = %f\n", var_values.arr[i].name, var_values.arr[i].value);
    }
    printf("min: %f\n", min);
    printf("\n");
    return 0;
}
