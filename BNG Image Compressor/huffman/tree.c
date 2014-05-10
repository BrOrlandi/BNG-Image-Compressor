#include "tree.h"
#include <stdlib.h>

Node* Tree_newNode(unsigned char byte,unsigned int frequency, char isLeaf){
    Node *n = (Node *)malloc(sizeof(Node));
    n->l = n->r = NULL;
    n->byte = byte;
    n->frequency = frequency;
    n->isLeaf = isLeaf;
    return n;
}

void Tree_insertNodeLeft(Node* node, Node* toInsert){
    node->l = toInsert;
}

void Tree_insertNodeRight(Node* node, Node* toInsert){
    node->r = toInsert;
}

void free_r(Node *n){
    if(n->l){
        Tree_free(n->l);
        free(n->l);
    }else if(n->r){
        Tree_free(n->r);
        free(n->r);
    }
    return;
}

void Tree_free(Node *n){
    free_r(n);
    free(n);
    return;
}
