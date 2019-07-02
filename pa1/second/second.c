#include <stdio.h>
#include <stdlib.h>

struct Node{
    struct Node* head;
    struct Node* tail;
    struct Node* next;
    int val;
};

void insertNode(int val);
int hashF(int);
struct Node hashMap[9999];

int main(int argc, char** argv) {

    if(argc != 2){
        printf("Error: No File Given\n");
        //return 0;
    }
    //hash function: key modulo the number of buckets.

    insertNode(4);
    printf("%d\n", hashMap[4].head->val);

}

void insertNode(int key){

    struct Node* n = (struct Node*)malloc(sizeof(struct Node)); //Survives outside function b/c malloc
    int index = hashF(key);

    n -> next = NULL;
    n -> val = key;

    if(hashMap[index].head == NULL){
        hashMap[index].head = n;
        hashMap[index].tail = n;
    } else {
        hashMap[index].tail -> next = n;
        hashMap[index].tail = n; //equal to tail = tail -> next
    }

}

int hashF(int key){
    return key % 10000;
}