#include "assert.h"
#include "node.h"
#include "node_list.h"
#include "optimizer.h"
#include "parser.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        panic("Usage: %s <input_file>\n", argv[0]);
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        panic("Could not open file '%s'!\n", argv[1]);
    }

    NodeList *numeric_vars = new_node_list();
    Node *tree;

    char buf[2048];
    for (size_t i = 0; fgets(buf, sizeof(buf), file) != NULL; ++i) {
        if (strchr(buf, '\n') == NULL && !feof(file)) {
            panic("Line too long!\n");
        }
        buf[strcspn(buf, "\n")] = '\0';
        if (strlen(buf) == 0)
            break;
        if (i == 0) {
            tree = parse(buf, numeric_vars);
        } else {
            char var_name[100];
            float var_val;
            sscanf(buf, "%99s = %f", var_name, &var_val);
            _Elem *curr = numeric_vars->first;
            while (curr != NULL) {
                if (strcmp(curr->data->name, var_name) == 0) {
                    curr->data->val = var_val;
                }
                curr = curr->next;
            }
        }
    }
    fclose(file);

    eval(tree);
    printf("eval: %f\n", tree->val);
    float min = optim(tree, numeric_vars);
    printf("min: %f\n", min);
    _Elem *curr = numeric_vars->first;
    while (curr != NULL) {
        printf("%s = %f\n", curr->data->name, curr->data->val);
        curr = curr->next;
    }

    free_node(tree);
    free_node_list(numeric_vars);

    printf("\n");
    return 0;
}
