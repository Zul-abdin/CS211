#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Line {
    int valid;
    unsigned long tag;
    int repIndex; //FIFO: Lower = Older
} Line;

int checkInputValidity(int argc, char** argv);
Line** initializeCache(int numSet, int assoc);
void freeCache(Line** cache, int numSet);
void printBits(size_t const size, void const * const ptr);
unsigned long getNthToKthBits(unsigned long x, int n, int k);
void FIFO(char operation, unsigned long tag, unsigned long setIndex, int assoc); //readFIFO and writeFIFO combined
void readFIFO(unsigned long tag, unsigned long setIndex, int assoc);
void writeFIFO(unsigned long tag, unsigned long setIndex, int assoc);
void LRU(char operation, unsigned long tag, unsigned long setIndex, int assoc); //readLRU and writeLRU combined
void readLRU(unsigned long tag, unsigned long setIndex, int assoc);
void writeLRU(unsigned long tag, unsigned long setIndex, int assoc);
void cacheData(char operation, char cachePolicy, unsigned long tag, unsigned long setIndex, int assoc); //FIFO and LRU combined
void fillLine(unsigned long tag, unsigned long setIndex, int lineIndex);

Line** cache;
int cacheMiss;
int cacheHit;
int memRead;
int memWrite;
int repCount;

int main(int argc, char** argv) {

//    const int addressSize = 48;

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
    char cachePolicy = argv[3][0]; // fifo (f) or lru (l)
//    char* associativity = argv[4]; //direct, assoc, or assoc:n
    FILE* fp = fopen(argv[5], "r");

//--------------Calculate Total Sets and Total Lines--------------------//
    unsigned int totalSets;
    int assoc;
    int setIndexSize = -1;
    unsigned int totalLines = cacheSize / blockSize;
    if(strcmp("direct", argv[4]) == 0){
        totalSets = totalLines;
        assoc = 1;
    } else if(strcmp("assoc", argv[4]) == 0){
        totalSets = 1;
        assoc = totalLines;
        setIndexSize = 0;
    } else {
        int linesPerSet = argv[4][6] - '0';
        totalSets = totalLines / linesPerSet;
        assoc = argv[4][6] - '0';
    }

//--------------------Initialize Addressing----------------------------//
    int blockOffsetSize = (int) log2(blockSize);
    if(setIndexSize != 0){
        setIndexSize = (int) log2(totalLines / assoc);
    }
//    int tagSize = addressSize - setIndexSize - blockOffsetSize;

//----------------------Initialize Cache-------------------------------//
    cache = initializeCache(totalSets, assoc);

    char operation;
    unsigned long address;

//    printf("Total Sets: %d\nAssoc: %d\nBlock Size: %d\nTag Size: %d\nCache Size: %d\nCache Policy: %c\nBlock Index Bits: %d\nSet Bits: %d\n", totalSets, assoc, blockSize, tagSize, cacheSize, cachePolicy, blockOffsetSize, setIndexSize);

//----------------------Start Caching File-------------------------------//

    while(fscanf(fp, "%c %lx\n", &operation, &address) != EOF && operation != '#') {

//        unsigned long blockOffset = getNthToKthBits(address, 1, blockOffsetSize);
        unsigned long setIndex = getNthToKthBits(address, blockOffsetSize + 1, blockOffsetSize + setIndexSize);
        unsigned long tag = getNthToKthBits(address, blockOffsetSize + setIndexSize + 1, 64);

        cacheData(operation, cachePolicy, tag, setIndex, assoc);
    }

    printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", memRead, memWrite, cacheHit, cacheMiss);

    freeCache(cache, totalSets);
    fclose(fp);
}

//----------------------Helper Methods--------------------------------//

int checkInputValidity(int argc, char** argv){
    return 1;
}

Line** initializeCache(int numSet, int assoc){

    cache = (Line**)malloc(sizeof(Line*) * numSet);

    for(int i = 0; i < numSet; i++){
        cache[i] = (Line*)malloc(sizeof(Line) * assoc);
    }

    for (int i = 0; i < numSet; i++){
        for(int j = 0; j < assoc; j++){
            cache[i][j].valid = 0;
        }
    }
    return cache;
}

void freeCache(Line** cache, int numSet){

    for(int i = 0; i < numSet; i++){
        free(cache[i]);
    }
    free(cache);
}

//----------------------******************************************----------------------------//
//-------------------------------For Testing Purposes Only-------------------------------------//
//----------------------******************************************----------------------------//
void printBits(size_t const size, void const * const ptr){
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--){
        for (j = 7; j >= 0 ;j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

unsigned long getNthToKthBits(unsigned long x, int n, int k){

    if(n > k){
        return 0;
    }

    x <<= (64 - k);
    x >>= (63 - k + n);

    return x;
}

void readFIFO(unsigned long tag, unsigned long setIndex, int assoc) {

    for (int i = 0; i < assoc; i++) {
//-------------------------------Fill All Invalid Lines-------------------------------------//
        if (cache[setIndex][i].valid == 0) {
            cacheMiss++;
            memRead++;
            fillLine(tag, setIndex, i);
            return;

//-------------------------------Check For Tag Match-------------------------------------//
        } else if(cache[setIndex][i].tag == tag) {
            cacheHit++;
            return;
        }
    }
//----------------------All Lines Are Valid And No Tag Matches----------------------------//
    cacheMiss++;
    memRead++;

//----------------Find Minimum Replacement Indexed Line and Replace----------------------//
    int minRep = 0;
    for(int i = 0; i < assoc; i++){
        if(cache[setIndex][i].repIndex < cache[setIndex][minRep].repIndex){
            minRep = i;
        }
    }
    fillLine(tag, setIndex, minRep);
}

void writeFIFO(unsigned long tag, unsigned long setIndex, int assoc) {

    for (int i = 0; i < assoc; i++) {
//-------------------------------Fill All Invalid Lines-------------------------------------//
        if (cache[setIndex][i].valid == 0) {
            cacheMiss++;
            memRead++;
            memWrite++;
            fillLine(tag, setIndex, i);
            return;

//-------------------------------Check For Tag Match-------------------------------------//
        } else if(cache[setIndex][i].tag == tag){
            cacheHit++;
            memWrite++;
            return;
        }
    }
//----------------------All Lines Are Valid And No Tag Matches----------------------------//
    cacheMiss++;
    memRead++;
    memWrite++;

//----------------Find Minimum Replacement Indexed Line and Replace----------------------//
    int minRep = 0;
    for(int i = 0; i < assoc; i++){
        if(cache[setIndex][i].repIndex < cache[setIndex][minRep].repIndex){
            minRep = i;
        }
    }
    fillLine(tag, setIndex, minRep);
}

//----------------------******************************************----------------------------//
//----------------------------ReadFIFO and WriteFIFO Combined---------------------------------//
//----------------------******************************************----------------------------//

void FIFO(char operation, unsigned long tag, unsigned long setIndex, int assoc){

    for (int i = 0; i < assoc; i++) {
//-------------------------------Fill All Invalid Lines-------------------------------------//
        if (cache[setIndex][i].valid == 0) {
            cacheMiss++;
            memRead++;
            fillLine(tag, setIndex, i);
            if(operation == 'W'){
                memWrite++;
            }
            return;

//-------------------------------Check For Tag Match-------------------------------------//
        } else if(cache[setIndex][i].tag == tag) {
            cacheHit++;
            if(operation == 'W'){
                memWrite++;
            }
            return;
        }
    }
//----------------------All Lines Are Valid And No Tag Matches----------------------------//
    cacheMiss++;
    memRead++;

    if(operation == 'W'){
        memWrite++;
    }

//----------------Find Minimum Replacement Indexed Line and Replace----------------------//
    int minRep = 0;
    for(int i = 0; i < assoc; i++){
        if(cache[setIndex][i].repIndex < cache[setIndex][minRep].repIndex){
            minRep = i;
        }
    }
    fillLine(tag, setIndex, minRep);
}

void readLRU(unsigned long tag, unsigned long setIndex, int assoc){

    for (int i = 0; i < assoc; i++) {
//-------------------------------Fill All Invalid Lines-------------------------------------//
        if (cache[setIndex][i].valid == 0) {
            cacheMiss++;
            memRead++;
            fillLine(tag, setIndex, i);
            return;

//-------------------------------Check For Tag Match-------------------------------------//
        } else if(cache[setIndex][i].tag == tag) {
            cacheHit++;
            fillLine(tag, setIndex, i);
            return;
        }
    }
//----------------------All Lines Are Valid And No Tag Matches----------------------------//
    cacheMiss++;
    memRead++;

//----------------Find Minimum Replacement Indexed Line and Replace----------------------//
    int minRep = 0;
    for(int i = 0; i < assoc; i++){
        if(cache[setIndex][i].repIndex < cache[setIndex][minRep].repIndex){
            minRep = i;
        }
    }
    fillLine(tag, setIndex, minRep);
}

void writeLRU(unsigned long tag, unsigned long setIndex, int assoc){

    for (int i = 0; i < assoc; i++) {
//-------------------------------Fill All Invalid Lines-------------------------------------//
        if (cache[setIndex][i].valid == 0) {
            cacheMiss++;
            memRead++;
            memWrite++;
            fillLine(tag, setIndex, i);
            return;

//-------------------------------Check For Tag Match-------------------------------------//
        } else if(cache[setIndex][i].tag == tag){
            cacheHit++;
            memWrite++;
            fillLine(tag, setIndex, i);
            return;
        }
    }
//----------------------All Lines Are Valid And No Tag Matches----------------------------//
    cacheMiss++;
    memRead++;
    memWrite++;

//----------------Find Minimum Replacement Indexed Line and Replace----------------------//
    int minRep = 0;
    for(int i = 0; i < assoc; i++){
        if(cache[setIndex][i].repIndex < cache[setIndex][minRep].repIndex){
            minRep = i;
        }
    }
    fillLine(tag, setIndex, minRep);
}

//----------------------******************************************----------------------------//
//-----------------------------ReadLRU and WriteLRU Combined----------------------------------//
//----------------------******************************************----------------------------//

void LRU(char operation, unsigned long tag, unsigned long setIndex, int assoc){

    for (int i = 0; i < assoc; i++) {
//-------------------------------Fill All Invalid Lines-------------------------------------//
        if (cache[setIndex][i].valid == 0) {
            cacheMiss++;
            memRead++;
            fillLine(tag, setIndex, i);
            if(operation == 'W'){
                memWrite++;
            }
            return;

//-------------------------------Check For Tag Match-------------------------------------//
        } else if(cache[setIndex][i].tag == tag) {
            cacheHit++;
            if(operation == 'W'){
                memWrite++;
            }
            fillLine(tag, setIndex, i);
            return;
        }
    }
//----------------------All Lines Are Valid And No Tag Matches----------------------------//
    cacheMiss++;
    memRead++;
    if(operation == 'W'){
        memWrite++;
    }

//----------------Find Minimum Replacement Indexed Line and Replace----------------------//
    int minRep = 0;
    for(int i = 0; i < assoc; i++){
        if(cache[setIndex][i].repIndex < cache[setIndex][minRep].repIndex){
            minRep = i;
        }
    }
    fillLine(tag, setIndex, minRep);
}

//----------------------******************************************----------------------------//
//---------------------------------FIFO and LRU Combined--------------------------------------//
//----------------------******************************************----------------------------//

void cacheData(char operation, char cachePolicy, unsigned long tag, unsigned long setIndex, int assoc){
    for (int i = 0; i < assoc; i++) {
//-------------------------------Fill All Invalid Lines-------------------------------------//
        if (cache[setIndex][i].valid == 0) {
            cacheMiss++;
            memRead++;
            fillLine(tag, setIndex, i);
            if(operation == 'W'){
                memWrite++;
            }
            return;

//-------------------------------Check For Tag Match-------------------------------------//
        } else if(cache[setIndex][i].tag == tag) {
            cacheHit++;
            if(operation == 'W'){
                memWrite++;
            }
            if(cachePolicy == 'l'){
                fillLine(tag, setIndex, i);
            }
            return;
        }
    }
//----------------------All Lines Are Valid And No Tag Matches----------------------------//
    cacheMiss++;
    memRead++;

    if(operation == 'W'){
        memWrite++;
    }

//----------------Find Minimum Replacement Indexed Line and Replace----------------------//
    int minRep = 0;
    for(int i = 0; i < assoc; i++){
        if(cache[setIndex][i].repIndex < cache[setIndex][minRep].repIndex){
            minRep = i;
        }
    }
    fillLine(tag, setIndex, minRep);
}

void fillLine(unsigned long tag, unsigned long setIndex, int lineIndex){
    repCount++;
    cache[setIndex][lineIndex].tag = tag;
    cache[setIndex][lineIndex].valid = 1;
    cache[setIndex][lineIndex].repIndex = repCount;
}