#include <stdio.h>
#include <stdlib.h>

struct Node{
    struct Node* head;
    struct Node* tail;
    struct Node* next;
    int val;
};

void insertNode(struct Node*, int val);
int hashF(int);

int main(int argc, char** argv) {

    if(argc != 2){
        printf("Error: No File Given\n");
        return 0;
    }

    struct Node* hashMap[9999];

    insertNode(hashMap[hashF(1)], 1);
    insertNode(hashMap[hashF(4)], 1);




    //hash function: key modulo the number of buckets.

    int key = 5;
    int index = key % 10000;


    printf("%d\n", key);

}

void insertNode(struct Node* hM, int key){
    struct Node* n = (struct Node*)malloc(sizeof(struct Node)); //Survives outside function b/c malloc

    int index = hashF(key);

    n -> next = NULL;
    n -> val = key;

    if(hM[index].tail == NULL){
        hM[index].head = n;
        hM[index].tail = n;
    } else {
        hM[index].tail -> next = n;
        hM[index].tail = n;
    }

}

int hashF(int key){
    return key % 10000;
}