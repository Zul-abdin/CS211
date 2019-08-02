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

//--------------------Initialize Addressing----------------------------//
    int blockOffsetSize = logBaseTwo(blockSize);
    int setIndexSize = logBaseTwo(cacheSize * 8 / addressSize);
    int tagSize = addressSize - setIndexSize - blockOffsetSize;

//    char tag[tagSize];
//    char setIndex[setIndexSize];
//    char blockOffset[blockOffsetSize];

//----------------------Initialize Cache-------------------------------//
//    struct Line cache[totalSets][totalLines];

//----------------------Start Caching---------------------------------//

    char* hexAddress = (char*)malloc(sizeof(char) * addressSize + 1);
    char operation;

    printf("blockSize: %d\n", blockSize);
    printf("tagSize: %d\n", tagSize);
    printf("setIndexSize: %d\n", setIndexSize);
    printf("blockOffsetSize: %d\n", blockOffsetSize);

    while(fscanf(fp, "%c 0x%s\n", &operation, hexAddress) != EOF && operation != '#'){

//----------------------Convert Hex -> Bin---------------------------------//
        char binAddress[48];
        int i = 0;
        int j = 0;
        while(hexAddress[i]) {
            switch (hexAddress[i]) {
                case '0':
                    binAddress[j] = '0';
                    binAddress[j + 1] = '0';
                    binAddress[j + 2] = '0';
                    binAddress[j + 3] = '0';
                    break;
                case '1':
                    binAddress[j] = '0';
                    binAddress[j + 1] = '0';
                    binAddress[j + 2] = '0';
                    binAddress[j + 3] = '1';
                    break;
                case '2':
                    binAddress[j] = '0';
                    binAddress[j + 1] = '0';
                    binAddress[j + 2] = '1';
                    binAddress[j + 3] = '0';
                    break;
                case '3':
                    binAddress[j] = '0';
                    binAddress[j + 1] = '0';
                    binAddress[j + 2] = '1';
                    binAddress[j + 3] = '1';
                    break;
                case '4':
                    binAddress[j] = '0';
                    binAddress[j + 1] = '1';
                    binAddress[j + 2] = '0';
                    binAddress[j + 3] = '0';
                    break;
                case '5':
                    binAddress[j] = '0';
                    binAddress[j + 1] = '1';
                    binAddress[j + 2] = '0';
                    binAddress[j + 3] = '1';
                    break;
                case '6':
                    binAddress[j] = '0';
                    binAddress[j + 1] = '1';
                    binAddress[j + 2] = '1';
                    binAddress[j + 3] = '0';
                    break;
                case '7':
                    binAddress[j] = '0';
                    binAddress[j + 1] = '1';
                    binAddress[j + 2] = '1';
                    binAddress[j + 3] = '1';
                    break;
                case '8':
                    binAddress[j] = '1';
                    binAddress[j + 1] = '0';
                    binAddress[j + 2] = '0';
                    binAddress[j + 3] = '0';
                    break;
                case '9':
                    binAddress[j] = '1';
                    binAddress[j + 1] = '0';
                    binAddress[j + 2] = '0';
                    binAddress[j + 3] = '1';
                    break;
                case 'A':
                case 'a':
                    binAddress[j] = '1';
                    binAddress[j + 1] = '0';
                    binAddress[j + 2] = '1';
                    binAddress[j + 3] = '0';
                    break;
                case 'B':
                case 'b':
                    binAddress[j] = '1';
                    binAddress[j + 1] = '0';
                    binAddress[j + 2] = '1';
                    binAddress[j + 3] = '1';
                    break;
                case 'C':
                case 'c':
                    binAddress[j] = '1';
                    binAddress[j + 1] = '1';
                    binAddress[j + 2] = '0';
                    binAddress[j + 3] = '0';
                    break;
                case 'D':
                case 'd':
                    binAddress[j] = '1';
                    binAddress[j + 1] = '1';
                    binAddress[j + 2] = '0';
                    binAddress[j + 3] = '1';
                    break;
                case 'E':
                case 'e':
                    binAddress[j] = '1';
                    binAddress[j + 1] = '1';
                    binAddress[j + 2] = '1';
                    binAddress[j + 3] = '0';
                    break;
                case 'F':
                case 'f':
                    binAddress[j] = '1';
                    binAddress[j + 1] = '1';
                    binAddress[j + 2] = '1';
                    binAddress[j + 3] = '1';
                    break;
                default:
                    break;
            }
            i++;
            j += 4;
        }
        int difference = addressSize - strlen(binAddress);
        char fullBinAddress[49];
        for(int k = 0; k < difference; k++){
            fullBinAddress[k] = '0';
        }
        fullBinAddress[difference] = '\0';
        strcat(fullBinAddress, binAddress);

        printf("%c\n", operation);

        printf("%s --> %s\n",hexAddress, fullBinAddress);

//----------------------******************************************----------------------------//
//----------------------fullBinAddress contains the 48-bit Address----------------------------//
//----------------------******************************************----------------------------//

//------------------------Begin Reading and Writing from Cache------------------------------//



//        printf("Operation: %c    Hex Address: %s    Bin Address: %s \n", operation, hexAddress, binAddress);

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
    free(hexAddress);
//    free(binAddress);
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