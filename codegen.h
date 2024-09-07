struct tnode{
 int val;
 char *op;
 char *varname;
 int nodetype;
 struct variable *head;
 struct tnode *left,*right;
};

struct variable{
    char *name;
    int binding;
    struct variable *next;
};

struct tnode* makeLeafNode(int n);
struct tnode* makeVarNode(char *c);
struct tnode* makeOperatorNode(char c, struct tnode *l,struct tnode *r);
struct tnode * makeConnectorNode(struct tnode *l, struct tnode *r);
struct tnode * makeReadNode();
struct tnode * makeWriteNode();
struct tnode * makeIfNode(struct tnode *expr, struct tnode *stmt, struct tnode *stmt2);
struct tnode *makeFunctionNode(struct tnode *expr);
struct variable *lookUp(char *name);
struct tnode *makeReturnNode(struct tnode *expr);
struct tnode *defineFunction(struct tnode *expr, struct tnode *stmt);

int getReg();
void freeReg(int i);
int genCode();
int codeeval(struct tnode *t);
void write1(struct tnode *t);
void read1(struct tnode *t);
void endProgram();