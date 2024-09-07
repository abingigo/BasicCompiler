#include<stdlib.h>
#include<string.h>

struct variable *lst;
int a;

struct variable *lookUp(char *name)
{
    struct variable *x = lst;
    while(x != NULL)
    {
        if(strcmp(x->name, name) == 0)
            return x;
        x = x->next;
    }
    return NULL;
}

void install(char *name)
{
    if(strcmp(name, "argc") == 0)
        return;
    int pos = 1;
    if(lookUp(name) != NULL)
        return;
    if(lst == NULL)
    {
        lst = (struct variable *)malloc(sizeof(struct variable));
        lst->name = name;
        lst->binding = pos;
        lst->next = NULL;
    }
    else
    {
        pos++;
        struct variable *x = lst;
        while(x->next != NULL)
        {
            x = x->next;
            pos++;
        }
        x->next = (struct variable *)malloc(sizeof(struct variable));
        x->next->name = name;
        x->next->binding = pos;
        x->next->next = NULL;
    }
}

struct tnode* makeLeafNode(int n)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL;
    temp->val = n;
    temp->varname = NULL;
    temp->left = NULL;
    temp->right = NULL;
    temp->nodetype = 0;
    temp->head = NULL;
    return temp; 
}

struct tnode* makeVarNode(char *c)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL;
    temp->varname = c;
    temp->left = NULL;
    temp->right = NULL;
    temp->nodetype = 1;
    temp->head = NULL;
    install(c);
    return temp; 
}

struct tnode* makeOperatorNode(char c, struct tnode *l, struct tnode *r){
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = malloc(sizeof(char));
    *(temp->op) = c; 
    temp->varname = NULL;
    temp->left = l;
    temp->right = r;
    temp->nodetype = 2;
    temp->head = NULL;
    return temp;
}

struct tnode* makeConnectorNode(struct tnode *l, struct tnode *r)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL; 
    temp->varname = NULL;
    temp->left = l;
    temp->right = r;
    temp->nodetype = 3;
    temp->head = NULL;
    return temp;
}

struct tnode* makeReadNode()
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL; 
    temp->varname = NULL;
    temp->left = NULL;
    temp->right = NULL;
    temp->nodetype = 4;
    temp->head = NULL;
    return temp;
}

struct tnode* makeWriteNode()
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL; 
    temp->varname = NULL;
    temp->left = NULL;
    temp->right = NULL;
    temp->nodetype = 5;
    temp->head = NULL;
    return temp;
}

struct tnode * makeIfNode(struct tnode *expr, struct tnode *stmt, struct tnode *stmt2)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL; 
    temp->varname = NULL;
    temp->left = expr;
    temp->right = makeConnectorNode(stmt, stmt2);
    temp->nodetype = 6;
    temp->head = NULL;
    return temp;
}

struct tnode *makeFunctionNode(struct tnode *expr)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL; 
    temp->varname = "FUN";
    temp->left = expr;
    temp->right = NULL;
    temp->nodetype = 7;
    return temp;
}

struct tnode *makeReturnNode(struct tnode *expr)
{
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->op = NULL; 
    temp->varname = NULL;
    temp->left = expr;
    temp->right = NULL;
    temp->nodetype = 8;
    temp->head = NULL;
    return temp;
}

int registers[20] = {0};
int label_no = 0;

int getReg()
{
    for(int i = 0; i < 20; i++)
        if(registers[i] == 0)
        {
            registers[i] = 1;
            return i;
        }
}

int usedReg(int i)
{
    if(registers[i] == 1)
        return 0;
    return 1;
}

int getLabel()
{
    return ++label_no;
}

void freeReg(int i)
{
    registers[i] = 0;
}

void allocateMemory()
{
    int i = 0;
    struct variable *x = lst;
    while(x != NULL)
    {
        i++;
        x = x->next;
    }
    printf("ADD SP, %d\n", i);
}

void deAllocateMemory()
{
    int i = 0;
    struct variable *x = lst;
    while(x != NULL)
    {
        i++;
        x = x->next;
    }
    printf("SUB SP, %d\n", i);
}

int genCode()
{
	printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",0,2056,0,0,0,0,0,0);
    printf("MOV SP, 4096\n");
    return 0;
}

int codeEval(struct tnode *t, int x)
{
    int reg, reg1, reg2, reg3, a, b, c;
    switch(t->nodetype)
    {
        case 0: reg = getReg();
                printf("MOV R%d, %d\n",reg, t->val);
	            return reg;
                break;
         case 1:reg = getReg();
                reg1 = getReg();
                if(x == 1)
                {
                    if(strcmp(t->varname, "argc") == 0)
                    {
                        printf("MOV R%d, BP\n", reg1);
                        printf("SUB R%d, %d\n", reg1, 3);
                        printf("MOV R%d, R%d\n", reg, reg1);
                        freeReg(reg1);
                        return reg;
                    }
                    a = lookUp(t->varname)->binding;
                    printf("MOV R%d, BP\n", reg1);
                    printf("ADD R%d, %d\n", reg1, a);
                    printf("MOV R%d, R%d\n", reg, reg1);
                    freeReg(reg1);
                    return reg;
                }
                 if(strcmp(t->varname, "argc") == 0)
                {
                    printf("MOV R%d, BP\n", reg1);
                    printf("SUB R%d, %d\n", reg1, 3);
                    printf("MOV R%d, [R%d]\n", reg, reg1);
                    freeReg(reg1);
                    return reg;
                }
                a = lookUp(t->varname)->binding;
                printf("MOV R%d, BP\n", reg1);
                printf("ADD R%d, %d\n", reg1, a);
                printf("MOV R%d, [R%d]\n", reg, reg1);
                freeReg(reg1);
	            return reg;
                break;
        case 2: switch(*(t->op))
                {
                    case '+' : reg1 = codeEval(t->left, 0), reg2 = codeEval(t->right, 0);
                            printf("ADD R%d,R%d\n", reg1, reg2);
                            freeReg(reg2);
                            return reg1;
                            break;
                    case '-' : reg1 = codeEval(t->left, 0), reg2 = codeEval(t->right, 0);
                            printf("SUB R%d,R%d\n", reg1, reg2);
                            freeReg(reg2);
                            return reg1;
                            break;
                    case '*' : reg1 = codeEval(t->left, 0), reg2 = codeEval(t->right, 0);
                            printf("MUL R%d,R%d\n", reg1, reg2);
                            freeReg(reg2);
                            return reg1;
                            break;
                    case '=' : reg1 = codeEval(t->left, 1), reg2 = codeEval(t->right, 0);
                            printf("BRKP\n");
                            printf("MOV [R%d],R%d\n", reg1, reg2);
                            freeReg(reg2);
                            return reg1;
                            break;
                    case '<': reg = getReg();
                            reg3 = getReg();
                            reg1 = codeEval(t->left, 0), reg2 = codeEval(t->right, 0);
                            printf("MOV R%d, R%d\n", reg, reg1);
                            printf("MOV R%d, R%d\n", reg3, reg2);
                            printf("LE R%d, R%d\n", reg, reg3);
                            freeReg(reg1);
                            freeReg(reg2);
                            freeReg(reg3);
                            return reg;
                            break;
                    case '>': reg = getReg();
                            reg3 = getReg();
                            reg1 = codeEval(t->left, 0), reg2 = codeEval(t->right, 0);
                            printf("MOV R%d, R%d\n", reg, reg1);
                            printf("MOV R%d, R%d\n", reg3, reg2);
                            printf("GE R%d, R%d\n", reg, reg3);
                            freeReg(reg1);
                            freeReg(reg2);
                            freeReg(reg3);
                            return reg;
                            break;
                    case ',': reg = getReg();
                            reg3 = getReg();
                            reg1 = codeEval(t->left, 0), reg2 = codeEval(t->right, 0);
                            printf("MOV R%d, R%d\n", reg, reg1);
                            printf("MOV R%d, R%d\n", reg3, reg2);
                            printf("LT R%d, R%d\n", reg, reg3);
                            freeReg(reg1);
                            freeReg(reg2);
                            freeReg(reg3);
                            return reg;
                            break;
                    case '.': reg = getReg();
                            reg3 = getReg();
                            reg1 = codeEval(t->left, 0), reg2 = codeEval(t->right, 0);
                            printf("MOV R%d, R%d\n", reg, reg1);
                            printf("MOV R%d, R%d\n", reg3, reg2);
                            printf("GT R%d, R%d\n", reg, reg3);
                            freeReg(reg1);
                            freeReg(reg2);
                            freeReg(reg3);
                            return reg;
                            break;
                    case '~': reg = getReg();
                            reg3 = getReg();
                            reg1 = codeEval(t->left, 0), reg2 = codeEval(t->right, 0);
                            printf("MOV R%d, R%d\n", reg, reg1);
                            printf("MOV R%d, R%d\n", reg3, reg2);
                            printf("EQ R%d, R%d\n", reg, reg3);
                            freeReg(reg1);
                            freeReg(reg2);
                            freeReg(reg3);
                            return reg;
                            break;
                    case '!': reg = getReg();
                            reg3 = getReg();
                            reg1 = codeEval(t->left, 0), reg2 = codeEval(t->right, 0);
                            printf("MOV R%d, R%d\n", reg, reg1);
                            printf("MOV R%d, R%d\n", reg3, reg2);
                            printf("NE R%d, R%d\n", reg, reg3);
                            freeReg(reg1);
                            freeReg(reg2);
                            freeReg(reg3);
                            return reg;
                            break;
                }
                break;
        case 3: codeEval(t->left, 0);
                codeEval(t->right, 0);
                break;
        case 4: read1(t);
                break;
        case 5: write1(t);
                break;
        case 6: reg = codeEval(t->left, 0);
                a = getLabel();
                b = getLabel();
                c = getLabel();
                printf("JNZ R%d, L%d\n", reg, a);
                printf("JZ R%d, L%d\n", reg, b);
                printf("L%d:\n", a);
                freeReg(reg);
                codeEval(t->right->left, 0);
                printf("JMP L%d\n", c);
                printf("L%d:\n", b);
                if(t->right->right != NULL)
                    codeEval(t->right->right, 0);
                printf("L%d:\n", c);
                break;
        case 7: reg1 = getReg();
                reg = codeEval(t->left, 0);
                int j = 0;
                freeReg(reg);
                for(int i = 0; i < 20; i++)
                {
                    if(usedReg(i) == 0)
                        printf("PUSH R%d\n", i);
                }
                printf("PUSH R%d\n", reg);
                printf("PUSH R%d\n", reg1);
                printf("CALL L0\n");
                reg3 = getReg();
                printf("POP R%d\n", reg1);
                reg2 = getReg();
                printf("MOV R%d, R%d\n", reg2, reg1);
                printf("POP R%d\n", reg1);
                freeReg(reg2);
                freeReg(reg3);
                for(int i = 19; i >= 0; i--)
                {
                    if(usedReg(i) == 0)
                        printf("POP R%d\n", i);
                }
                registers[reg2] = 1;
                freeReg(reg1);
                return reg2;
                break;
        case 8: deAllocateMemory();
                reg = codeEval(t->left, 0);
                reg1 = getReg();
                printf("MOV R%d, BP\n", reg1);
                printf("SUB R%d, 2\n", reg1);
                printf("MOV [R%d], R%d\n", reg1, reg);
                freeReg(reg);
                printf("POP BP\n");
                printf("RET\n");
                break;
    }
}

void read1(struct tnode *t)
{
    int addr = 4096;
    int reg = getReg();
    printf("MOV R%d,\"Read\"\n", reg);
    printf("PUSH R%d\n", reg);
    printf("MOV R%d,-1\n", reg);
    printf("PUSH R%d\n", reg);
    printf("MOV R%d,%d\n", reg, addr);
    printf("PUSH R%d\n", reg);
    printf("PUSH R%d\n", reg);
    printf("PUSH R%d\n", reg);
    printf("CALL 0\n");
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    freeReg(reg);
    int x = getReg();
    for(int i = 0; i < 20; i++)
    {
        if(usedReg(i) == 0)
            printf("PUSH R%d\n", i);
    }
    printf("MOV R%d, [4096]\n", x);
    printf("PUSH R%d\n", x);
    printf("PUSH R%d\n", x);
    printf("CALL L0\n");
    printf("POP R%d\n", x);
    printf("MOV [4096], R%d\n", x);
    printf("POP R%d\n", x);
    for(int i = 19; i >= 0; i--)
    {
        if(usedReg(i) == 0)
            printf("POP R%d\n", i);
    }
    a = getLabel();
    freeReg(x);
    printf("JMP L%d\n", a);
    printf("L0:\n");
    printf("BRKP\n");
    printf("PUSH BP\n");
    printf("MOV BP, SP\n");
    allocateMemory();
}

void write1(struct tnode *t)
{
    int addr = 4096;
    int reg = getReg();
    printf("L%d:\n", a);
    printf("MOV R%d,\"Write\"\n", reg);
    printf("PUSH R%d\n", reg);
    printf("MOV R%d,-2\n", reg);
    printf("PUSH R%d\n", reg);
    printf("MOV R%d,[%d]\n", reg, addr);
    printf("PUSH R%d\n", reg);
    printf("PUSH R%d\n", reg);
    printf("PUSH R%d\n", reg);
    printf("CALL 0\n");
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    freeReg(reg);
}

void endProgram()
{
    int reg = getReg();
    printf("MOV R%d, \"Exit\"\n", reg);
    printf("PUSH R%d\n", reg);
    printf("PUSH R%d\n", reg);
    printf("PUSH R%d\n", reg);
    printf("PUSH R%d\n", reg);
    printf("PUSH R%d\n", reg);
    printf("CALL 0\n");
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    printf("POP R%d\n", reg);
    freeReg(reg);
}