#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Line {
//    char* valid;
//    char* tag;
//    char* block;
};

int checkInputValidity(int argc, char** argv);
//int isPowerOfTwo(int n);
int logBaseTwo(unsigned int n);
void append(char* s, char c);

int main(int argc, char** argv) {

    const int addressSize = 48;

    //Input Expression: ./first <cache size><block size><cache policy><associativity><trace file>
    if(argc != 6){
        printf("error\n");
        return -1;
    }

    if(!checkInputValidity(argc, argv)){
        printf("error");
    }

//----------------------Parsing Arguments-------------------------------//
    int cacheSize = atoi(argv[1]);
    int blockSize = atoi(argv[2]);
//    char* cachePolicy = argv[3]; // fifo or lru
//    char* associativity = argv[4]; //direct, assoc, or assoc:n
    FILE* fp = fopen(argv[5], "r");

//--------------Calculate Total Sets and Total Lines--------------------//
    unsigned int totalSets;
    unsigned int totalLines = cacheSize / blockSize;
    if(strcmp("direct", argv[4]) == 0){
        totalSets = totalLines;
    } else if(strcmp("assoc", argv[4]) == 0){
        totalSets = 1;
    } else {
        int linesPerSet = argv[4][6] - '0';
        totalSets = totalLines / linesPerSet;
    }

    printf("%d", cacheSize);

    return 1;

//--------------------Initialize Addressing----------------------------//
    int blockOffsetSize = logBaseTwo(blockSize);
    int setIndexSize = logBaseTwo(cacheSize / addressSize);
    int tagSize = addressSize - setIndexSize - blockOffsetSize;

//    char tag[tagSize];
//    char setIndex[setIndexSize];
//    char blockOffset[blockOffsetSize];

//----------------------Initialize Cache-------------------------------//
//    struct Line cache[totalSets][totalLines];

//----------------------Start Caching---------------------------------//

    char* address = (char*)malloc(sizeof(char) * addressSize + 1);
    char operation;

    printf("blockSize: %d\n", blockSize);
    printf("tagSize: %d\n", tagSize);
    printf("setIndexSize: %d\n", setIndexSize);
    printf("blockOffsetSize: %d\n", blockOffsetSize);

    while(fscanf(fp, "%c 0x%s\n", &operation, address) != EOF && operation != '#'){
        while(strlen(address) < 48){
            append(address, '0');
        }
        printf("Operation: %c    Address: %s\n", operation, address);

//        for(int i = 0; i < tagSize; i++) {
//            printf("this char");
//        }
//        for(int i = 0; i < setIndexSize; i++){
//            setIndex[i] = address[tagSize + i];
//        }
//        for(int i = 0; i < blockOffsetSize; i++){
//            blockOffset[i] = address[tagSize + setIndexSize + 1];
//        }

//        printf("Tag: %s    Set Index: %s    Block Offset: %s\n", tag, setIndex, blockOffset);
    }
}

//----------------------Helper Methods--------------------------------//

int checkInputValidity(int argc, char** argv){
    return 1;
}

int logBaseTwo(unsigned int n){

    int count = 0;

    while(n != 1){
        n /= 2;
        count++;
    }
    return count;
}

void append(char* s, char c){
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}