#include <stdio.h>
#include <stdlib.h>

struct Node{
    struct Node* head;
    struct Node* tail;
    struct Node* next;
    int val;
};

void insertNode(int key);
int hashF(int);
void printHeadLL(int key);
int searchHashMap(int key, int toPrint); //toPrint: give 1 to print, anything else will only return.


struct Node hashMap[9999];

int main(int argc, char** argv) {

    if(argc != 2){
        printf("Error: No File Given\n");
        return 0;
    }
    //hash function: key modulo the number of buckets.

    char func;
    int key;

    FILE* fp = fopen(argv[1], "r");

    if(fp == NULL){
        printf("Error: File syntax wrong\n");
        return 0;
    }

    while((fscanf(fp, "%c %d\n", &func, &key)) != EOF) {

       switch (func) {
           case 'i':
               insertNode(key);
               break;

           case 's':
               searchHashMap(key, 1);
               break;

           default:
               printf("Error: File syntax wrong (Cannot process chars other than i(insert) s(search)\n");
       }
    }

    //while(!feof(fp)){
    //    fscanf(fp, "%c %d\n", &func, &key);
    //    printf("%c %d\n", func, key);
    //}

    /*insertNode(10002);
    insertNode(2);

    printHeadLL(2);

    searchHashMap(2);
    searchHashMap(1002);
     */

    fclose(fp);

}

void insertNode(int key){

    if(searchHashMap(key, 0) == 1){
        printf("%d is a duplicate\n", key);
        return;
    }

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

    printf("%d has been inserted\n", key);

}

void printHeadLL(int key){

    int index = hashF(key);
    if(hashMap[index].head != NULL){
        for(struct Node* temp = hashMap[index].head; temp != NULL; temp = temp -> next){
            printf("%d -> ", temp->val);
        }
        printf("NULL\n");
    }

}

int searchHashMap(int key, int toPrint){
    int index = hashF(key);
    if(hashMap[index].head == NULL){
        if(toPrint == 1) {
            printf("%d is absent\n", key);
        }
        return 0;
    } else {
        for(struct Node* temp = hashMap[index].head; temp != NULL; temp = temp -> next){
            if(temp->val == key){
                if(toPrint == 1) {
                    printf("%d is present\n", key);
                }
                return 1;
            }
        }
    }
    if(toPrint == 1) {
        printf("%d is absent\n", key);
    }
    return 0;
}

int hashF(int key){
    return key % 10000;
}