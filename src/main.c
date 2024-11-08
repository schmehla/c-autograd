// #include "optimizer.h"
#include "assert.h"
#include "node.h"
#include "node_list.h"
#include "optimizer.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    char buf[1001];
    FILE *file = fopen("../input.txt", "r");
    if (file == NULL) {
        assert(false);
    }

    NodeList *numeric_vars = new_node_list();
    Node *tree;

    for (size_t i = 0; fgets(buf, sizeof(buf), file) != NULL; ++i) {
        buf[strcspn(buf, "\n")] = '\0';
        if (strlen(buf) == 0)
            break;
        if (i == 0) {
            tree = parse(buf, numeric_vars);
        } else {
            char var_name[100];
            float var_val;
            sscanf(buf, "%99s = %f", var_name, &var_val);
            _Elem *curr = numeric_vars->first; // TODO _Elem is private!
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
