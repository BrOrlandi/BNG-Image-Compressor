#ifndef TREE_H
#define TREE_H
#define TRUE 1
#define FALSE 0

typedef struct Node{
    unsigned char byte;
    unsigned int frequency;
    char isLeaf;
    struct Node *l, *r;
}Node;

Node* Tree_newNode(unsigned char byte,unsigned int frequency, char isLeaf);
void Tree_insertNodeLeft(Node* node, Node* toInsert);
void Tree_insertNodeRight(Node* node, Node* toInsert);
void Tree_free(Node *node);

#endif // TREE_H
