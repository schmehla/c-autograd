#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

enum BIN_OP { ADD, SUB, MUL, DIV };
enum UN_OP { NEG };

enum NODE_TYPE {
    BIN_NODE,
    UN_NODE,
    VAR_NODE,
    NUM_NODE
}; // binary node, unary node

typedef struct Node Node;

typedef struct {
    Node *lhs; // left-hand side
    Node *rhs; // right-hand side
    enum BIN_OP op;
} BinNode;

typedef struct {
    Node *child;
    enum UN_OP op;
} UnNode;

typedef struct {
    char *name;
} VarNode;

typedef struct {
    float num;
} NumNode;

union NodeData {
    BinNode bin_node;
    UnNode un_node;
    VarNode var_node;
    NumNode num_node;
};

struct Node { // typedef is at the top
    enum NODE_TYPE type;
    union NodeData *data;
    float grad;
    float val;
};

typedef struct NodeList {
    Node *data;
    struct NodeList *next;
} NodeList;

Node *create_bin_node(enum BIN_OP op, Node *lhs, Node *rhs, NodeList **vars);
Node *create_un_node(enum UN_OP op, Node *n, NodeList **vars);
Node *create_var_node(const char *name, NodeList **vars);
Node *create_num_node(float num, NodeList **vars);

void zero_grad(Node *node);

void free_node(Node *node);

NodeList **empty_node_list();
void node_list_append(NodeList **node_list, Node *data);
void free_node_list(NodeList **node_list);
Node *find_by_name(NodeList **node_list, const char *name);

void print(Node *n);

#endif
