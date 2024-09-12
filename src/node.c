#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *create_bin_node(enum BIN_OP op, Node *lhs, Node *rhs, NodeList **vars) {
    Node *node = malloc(sizeof(Node));
    node->type = BIN_NODE;
    union NodeData *nd = malloc(sizeof(union NodeData));
    nd->bin_node.op = op;
    nd->bin_node.lhs = lhs;
    nd->bin_node.rhs = rhs;
    node->data = nd;
    node->grad = 0;
    return node;
}

Node *create_un_node(enum UN_OP op, Node *child, NodeList **vars) {
    Node *node = malloc(sizeof(Node));
    union NodeData *nd = malloc(sizeof(union NodeData));
    nd->un_node.op = NEG;
    nd->un_node.child = child;
    node->type = UN_NODE;
    node->data = nd;
    node->grad = 0;
    return node;
}

Node *create_var_node(const char *name, NodeList **vars) {
    Node *node = find_by_name(vars, name);
    if (node != NULL) {
        return node;
    }
    node = malloc(sizeof(Node));
    union NodeData *nd = malloc(sizeof(union NodeData));
    nd->var_node.name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(nd->var_node.name, name);
    node->type = VAR_NODE;
    node->data = nd;
    node->grad = 0;
    node_list_append(vars, node);
    return node;
}

Node *create_num_node(float num, NodeList **vars) {
    Node *node = malloc(sizeof(Node));
    union NodeData *nd = malloc(sizeof(union NodeData));
    nd->num_node.num = num;
    node->type = NUM_NODE;
    node->data = nd;
    node->grad = 0;
    return node;
}

void zero_grad(Node *node) {
    node->grad = 0;
    if (node->type == BIN_NODE) {
        zero_grad(node->data->bin_node.lhs);
        zero_grad(node->data->bin_node.rhs);
    }
    if (node->type == UN_NODE)
        zero_grad(node->data->un_node.child);
}

void free_node(Node *node) {
    if (node->type == VAR_NODE)
        return;
    if (node->type == BIN_NODE) {
        free_node(node->data->bin_node.lhs);
        free_node(node->data->bin_node.rhs);
    }
    if (node->type == UN_NODE) {
        free_node(node->data->un_node.child);
    }
    if (node->type == NUM_NODE) {
        // do nothing
    }
    free(node->data);
    free(node);
}

NodeList **empty_node_list() {
    NodeList **nl = malloc(sizeof(NodeList *));
    *nl = NULL;
    return nl;
}

void node_list_append(NodeList **node_list, Node *data) {
    NodeList *nl = malloc(sizeof(NodeList));
    nl->data = data;
    nl->next = NULL;
    if (*node_list == NULL) {
        *node_list = nl;
        return;
    }
    NodeList *cur = *node_list;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = nl;
}

void free_node_list(NodeList **node_list) {
    NodeList *cur = *node_list;
    while (cur != NULL) {
        NodeList *next = cur->next;
        free(cur->data->data->var_node.name);
        free(cur->data->data);
        free(cur->data);
        free(cur);
        cur = next;
    }
    free(node_list);
}

Node *find_by_name(NodeList **node_list, const char *name) {
    NodeList *cur = *node_list;
    while (cur != NULL) {
        if (strcmp(cur->data->data->var_node.name, name) == 0)
            return cur->data;
        cur = cur->next;
    }
    return NULL;
}

void print(Node *n) {
    printf("(");
    if (n->type == BIN_NODE) {
        print(n->data->bin_node.lhs);
        if (n->data->bin_node.op == ADD)
            printf("+");
        if (n->data->bin_node.op == SUB)
            printf("-");
        if (n->data->bin_node.op == MUL)
            printf("*");
        if (n->data->bin_node.op == DIV)
            printf("/");
        print(n->data->bin_node.rhs);
    }
    if (n->type == UN_NODE) {
        if (n->data->un_node.op == NEG)
            printf("-");
        print(n->data->un_node.child);
    }
    if (n->type == VAR_NODE) {
        printf("%s", n->data->var_node.name);
    }
    if (n->type == NUM_NODE) {
        printf("%f", n->data->num_node.num);
    }
    printf(")");
}
