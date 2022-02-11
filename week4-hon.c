//Week 4 honors assignment
#include <stdio.h>
#include <stdlib.h>

typedef int Data;

// Defining a node
typedef struct node{
    Data d;
    struct node  *left;
    struct node *right;
}Node;
typedef Node* BTree;

// Creating a new node, not adding it to any tree yet, or even giving it data.
Node *new_node(void){
    return malloc(sizeof(Node));
}

// Creating and initializing a new node.
Node *init_node(Data d, Node *left, Node *right){
    Node *t;
    t = new_node();
    t->d = d;
    t->left = left;
    t->right = right;
    return t;
}
// Standard in order traversal of a binary tree
void in_order(BTree root){
    if(root == NULL)
        return;
    else{
        in_order(root->left);
        printf("%d\n", root->d);
        in_order(root->right);
    }
}
// This method creates a new tree recursively
BTree create_tree(Data a[], int i, int size){
    if (i>=size)
        return NULL;
    else
        return init_node(a[i], create_tree(a, i*2+1, size), create_tree(a, i*2+2, size));

}
void read_file(FILE *ifp, int d[], int size){

    int i;
    for(i = 0; i < size; i++){
        fscanf(ifp, "%d", &d[i]);
    }
}

int main(int argc, char *argv[]){

    FILE *ifp; //input file
    int n; // Size of the array
    if(argc != 2){                       //Checking no of arguments
        printf("Wrong number of arguments, please specify only input file name\n");
        exit(1);
    }
    // Opening the file
    ifp = fopen(argv[1], "r");
    if(fscanf(ifp, "%d", &n) != 1){
        printf("File empty\n");
        exit(1);
    }
    int data[n];
    read_file(ifp, data, n);
    BTree bt;
    bt = create_tree(data, 0, n);
    in_order(bt);
    fclose(ifp);
    return 0;
}
