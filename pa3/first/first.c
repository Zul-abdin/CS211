#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Line {
    int valid;
    char* tag;
//    char* block;
} Line;

int checkInputValidity(int argc, char** argv);
int binToInt(char* s);
Line** initializeCache(int numSet, int assoc);

Line** cache;
int hits;
int misses;
int memRead;
int memWrite;

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

    memRead = 0;
    memWrite = 0;
    hits = 0;
    misses = 0;

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
        assoc = 0;
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
    int tagSize = addressSize - setIndexSize - blockOffsetSize;

    char tag[49];
    char setIndex[49];
    char blockOffset[49];

//----------------------Initialize Cache-------------------------------//
    struct Line cache[totalSets][assoc];

    for(int i = 0; i < totalSets; i++){
        for(int j = 0; j < assoc; j++){
//            cache[i][j].block = NULL;
            cache[i][j].tag = NULL;
            cache[i][j].valid = 0;
        }
    }

    int orderFIFO[totalSets][assoc]; // Elements represent relative assignment time. Higher Number = Older (FIFO)
    for(int i = 0; i < totalSets; i++) {
        for (int j = 0; j < assoc; j++) {
            orderFIFO[i][j] = 0;
        }
    }

//----------------------Start Caching---------------------------------//

    char* hexAddress = (char*)malloc(sizeof(char) * addressSize + 1);
    char operation;

//    printf("blockSize: %d\n", blockSize);
//    printf("tagSize: %d\n", tagSize);
//    printf("setIndexSize: %d\n", setIndexSize);
//    printf("blockOffsetSize: %d\n", blockOffsetSize);

    while(fscanf(fp, "%c 0x%s\n", &operation, hexAddress) != EOF && operation != '#') {

//----------------------Convert Hex -> Bin---------------------------------//
        char binAddress[48];
        int p = 0;
        int q = 0;
        while (hexAddress[p]) {
            switch (hexAddress[p]) {
                case '0':
                    binAddress[q] = '0';
                    binAddress[q + 1] = '0';
                    binAddress[q + 2] = '0';
                    binAddress[q + 3] = '0';
                    break;
                case '1':
                    binAddress[q] = '0';
                    binAddress[q + 1] = '0';
                    binAddress[q + 2] = '0';
                    binAddress[q + 3] = '1';
                    break;
                case '2':
                    binAddress[q] = '0';
                    binAddress[q + 1] = '0';
                    binAddress[q + 2] = '1';
                    binAddress[q + 3] = '0';
                    break;
                case '3':
                    binAddress[q] = '0';
                    binAddress[q + 1] = '0';
                    binAddress[q + 2] = '1';
                    binAddress[q + 3] = '1';
                    break;
                case '4':
                    binAddress[q] = '0';
                    binAddress[q + 1] = '1';
                    binAddress[q + 2] = '0';
                    binAddress[q + 3] = '0';
                    break;
                case '5':
                    binAddress[q] = '0';
                    binAddress[q + 1] = '1';
                    binAddress[q + 2] = '0';
                    binAddress[q + 3] = '1';
                    break;
                case '6':
                    binAddress[q] = '0';
                    binAddress[q + 1] = '1';
                    binAddress[q + 2] = '1';
                    binAddress[q + 3] = '0';
                    break;
                case '7':
                    binAddress[q] = '0';
                    binAddress[q + 1] = '1';
                    binAddress[q + 2] = '1';
                    binAddress[q + 3] = '1';
                    break;
                case '8':
                    binAddress[q] = '1';
                    binAddress[q + 1] = '0';
                    binAddress[q + 2] = '0';
                    binAddress[q + 3] = '0';
                    break;
                case '9':
                    binAddress[q] = '1';
                    binAddress[q + 1] = '0';
                    binAddress[q + 2] = '0';
                    binAddress[q + 3] = '1';
                    break;
                case 'A':
                case 'a':
                    binAddress[q] = '1';
                    binAddress[q + 1] = '0';
                    binAddress[q + 2] = '1';
                    binAddress[q + 3] = '0';
                    break;
                case 'B':
                case 'b':
                    binAddress[q] = '1';
                    binAddress[q + 1] = '0';
                    binAddress[q + 2] = '1';
                    binAddress[q + 3] = '1';
                    break;
                case 'C':
                case 'c':
                    binAddress[q] = '1';
                    binAddress[q + 1] = '1';
                    binAddress[q + 2] = '0';
                    binAddress[q + 3] = '0';
                    break;
                case 'D':
                case 'd':
                    binAddress[q] = '1';
                    binAddress[q + 1] = '1';
                    binAddress[q + 2] = '0';
                    binAddress[q + 3] = '1';
                    break;
                case 'E':
                case 'e':
                    binAddress[q] = '1';
                    binAddress[q + 1] = '1';
                    binAddress[q + 2] = '1';
                    binAddress[q + 3] = '0';
                    break;
                case 'F':
                case 'f':
                    binAddress[q] = '1';
                    binAddress[q + 1] = '1';
                    binAddress[q + 2] = '1';
                    binAddress[q + 3] = '1';
                    break;
                default:
                    break;
            }
            p++;
            q += 4;
        }
        int difference = addressSize - strlen(binAddress);
        char fullBinAddress[49];
        for (int i = 0; i < difference; i++) {
            fullBinAddress[i] = '0';
        }
        fullBinAddress[difference] = '\0';
        strcat(fullBinAddress, binAddress);

//----------------------******************************************----------------------------//
//----------------------fullBinAddress contains the 48-bit Address----------------------------//
//----------------------******************************************----------------------------//

//-------------------------Populating Addressing Variables------------------------------------//

        for (int i = 0; i < tagSize; i++) {
            tag[i] = fullBinAddress[i];
        }
        for (int i = 0; i < setIndexSize; i++) {
            setIndex[i] = fullBinAddress[i + tagSize];
        }
        for (int i = 0; i < blockOffsetSize; i++) {
            blockOffset[i] = fullBinAddress[i + tagSize + setIndexSize];
        }

        fullBinAddress[difference] = '\0';
        tag[tagSize] = '\0';
        setIndex[setIndexSize] = '\0';
        blockOffset[blockOffsetSize] = '\0';

//-----------------------******************************************----------------------------//
//-------------------------------Cache Reading and Writing-------------------------------------//
//-----------------------******************************************----------------------------//
        fullBinAddress[difference] = '\0';
        tag[tagSize] = '\0';
        setIndex[setIndexSize] = '\0';
        blockOffset[blockOffsetSize] = '\0';

        int setNum = binToInt(setIndex);
        int foundLine = 0;

//---------------Search Cache Set For A Line That Matches Tag And Is Valid---------------------//
        for (int i = 0; i < assoc; i++) {
            if (cache[setNum][i].valid == 1 && cache[setNum][i].tag == tag) {
                foundLine = 1;
//---------------Read Hit---------------------//
                if (operation == 'R') {
                    hits++;
                    break;
//---------------Write Hit---------------------//
                } else {
                    hits++;
                    writes++;
                    break;
                }
            }
        }

//---------------Search for Invalid Line to Fill---------------------//

        if(foundLine == 0) {
            int i;
            for (i = 0; i < assoc; i++) {
                if (cache[setNum][i].valid == 0) {
                    foundLine = 1;
//---------------Read Miss---------------------//
                    if (operation == 'R') {
                        misses++;
                        reads++;
                        break;
//---------------Write Miss---------------------//
                    } else {
                        misses++;
                        writes++;
                        reads++;
                        break;
                    }
                }
            }
            cache[setNum][i].valid = 1;
            cache[setNum][i].tag = tag;
        }

//------------------***************************************************------------------------//
//------------------Set is Full of Valid Lines. Need to Use FIFO or LRU------------------------//
//------------------***************************************************------------------------//

        if(foundLine == 0) {

//--------------------First In First Out (FIFO)-------------------//
            if (cachePolicy == 'f') {

                cache[setNum][orderFIFO[setNum][0]].valid = 1;
                cache[setNum][orderFIFO[setNum][0]].tag = tag;

//--------------------Last Recently Used (LRU)-------------------//
            } else {

            }
        }

//        printf("%c    %s --> %s\n", operation, hexAddress, fullBinAddress);
//        printf("Tag: %s    Set Index: %s    BLock Offset: %s    Set Num:%d\n", tag, setIndex, blockOffset, setNum);
    }

    printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", reads, writes, hits, misses);

    free(hexAddress);
//    free(binAddress);
}

//----------------------Helper Methods--------------------------------//

int checkInputValidity(int argc, char** argv){
    return 1;
}

int binToInt(char* s){
    return (int) strtol(s, NULL, 2);
}

Line** initializeCache(int numSet, int assoc){

    cache = (Line**)malloc(sizeof(Line*) * numSet);

    for(int i = 0; i < numSet; i++){
        cache[numSet] = (Line*)malloc(sizeof(Line) * assoc);
    }

    for (int i = 0; i < numSet; i++){
        for(int j = 0; j < numSet; j++){
            cache[i][j].valid = 0;
        }
    }

}