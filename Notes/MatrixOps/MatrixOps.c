#include <stdio.h>
#include <stdlib.h>

struct Node {
    struct Node * next;
    int val;
};

struct Node* head = NULL;
struct Node* tail = NULL;

void insertNode(int);

struct Node* searchNode(int);

int main(int argc, char** argv) {
    //int** M = (int**)malloc(2* sizeof(int*));
    //M[0] = (int*)malloc(2* sizeof(int));
    //printf("Hello, World!\n");

    insertNode(5);
    insertNode(1);
    insertNode(10);
    insertNode(15);

    struct Node* temp;
    for(temp = head; temp != NULL; temp = temp ->next){
        printf("%d -> ", temp -> val);
    }
    printf("NULL\n");

    temp = searchNode(5);

    if(temp == NULL){
        printf("Can't find 5\n");
    } else {
        printf("found it!");
    }

    temp = searchNode(100);

    if(temp == NULL){
        printf("Can't find 100\n");
    } else {
        printf("found it!");
    }

    return 0;
}

void insertNode(int val){
    struct Node* n = (struct Node*)malloc(sizeof(struct Node)); //Survives outside function b/c malloc

    n -> next = NULL;
    n -> val = val;

    if(tail == NULL){
        head = n;
        tail = n;
    } else {
        tail -> next = n;
        tail = n;
    }
}

struct Node* searchNode(int val){
    struct Node* temp = head;
    for(temp = head; temp != NULL; temp = temp ->next){
        if(temp -> val == val){
            return temp;
        }
    }

    return NULL;

}