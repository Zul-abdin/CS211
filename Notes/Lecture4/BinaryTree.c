/*
    Creating Binary Tree
 */
#include <stdlib.h>
#include <stdio.h>

// Initialize Node, head, and tail
struct Node {
    struct Node* left;
    struct Node* right;
    int val;
};

struct Node* CreateNode(int);
struct Node* InsertNode(struct Node*, int);
struct Node* SearchNode(struct Node*, int);
void PrintBinaryTree(struct Node*);
void FreeBinaryTree(struct Node*);
struct Node* RemoveNode(struct Node*, int);

int main(int argc, char** argv) {
    struct Node* root = NULL;
    root = InsertNode(root, 10);
    InsertNode(root, 5);
    InsertNode(root, 15);
    InsertNode(root, 8);
    InsertNode(root, 25);
    InsertNode(root, 19);
    InsertNode(root, 1);
    InsertNode(root, 2);
    InsertNode(root, 3);
    InsertNode(root, 11);
    PrintBinaryTree(root);
    
    if (SearchNode(root, 19) != NULL) {
        printf("19 found in the binary tree\n");
    } else {
        printf("19 not in binary tree\n");
    }
    
    if (SearchNode(root, 14) != NULL) {
        printf("14 found in the binary tree\n");
    } else {
        printf("14 not in binary tree\n");
    }
    
    RemoveNode(root, 19);
    RemoveNode(root, 15);
    RemoveNode(root, 25);
    RemoveNode(root, 1);
    RemoveNode(root, 5);
    RemoveNode(root, 8);
    RemoveNode(root, 11);
    
    printf("\n\n");
    PrintBinaryTree(root);
    
    FreeBinaryTree(root);
    return 0;
}

struct Node* CreateNode(int num) {
    struct Node* n = (struct Node*) malloc(sizeof(struct Node));
    n->val = num;
    n->left = NULL;
    n->right = NULL;
    return n;
}

struct Node* InsertNode(struct Node* node, int num) {
    if (node == NULL) {
        return CreateNode(num);
    }
    else if (num > node->val) {
        node->right = InsertNode(node->right, num);
    }
    else if (num < node->val) {
        node->left = InsertNode(node->left, num);
    }
    
    return node;
}

void PrintBinaryTree(struct Node* node) {
    if (node == NULL) return;
    PrintBinaryTree(node->left);
    printf("%d\n", node->val);
    PrintBinaryTree(node->right);
}

struct Node* SearchNode(struct Node* node, int n) {
    if (node == NULL) {
        return NULL;
    }
    if (node->val == n) {
        return node;
    }
    else if (n < node->val) {
        return SearchNode(node->left, n);
    }
    else if (n > node->val) {
        return SearchNode(node->right, n);
    }
    
    return NULL;
}

void FreeBinaryTree(struct Node* node) {
    if (node == NULL) return;
    FreeBinaryTree(node->left);
    FreeBinaryTree(node->right);
    return;
}

struct Node* FindMinimumValue(struct Node* node) {
    if (node->left == NULL) return node;
    return FindMinimumValue(node->left);
}

struct Node* RemoveNode(struct Node* node, int n) {
    if (node == NULL) return NULL;
    
    if (n < node->val) {
        node->left = RemoveNode(node->left, n);
    }
    else if (n > node->val) {
        node->right = RemoveNode(node->right, n);
    }
    else {
        if (node->left == NULL) {
            struct Node* temp = node->right;
            free(node);
            return temp;
        }
        else if (node->right == NULL) {
            struct Node* temp = node->left;
            free(node);
            return temp;
        }
        else {
            // Find the minimum value in the right subtree
            struct Node* min = FindMinimumValue(node->right);
            // Copy the value
            node->val = min->val;
            // Recursively delete the node containing the minimum value in the right subtree
            node->right = RemoveNode(node->right, min->val);
        }
    }
    
    return node;
}
