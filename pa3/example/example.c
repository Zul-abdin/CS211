#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/**************************Declare globle varible**********************/
int miss;
int hit;
int mr; //Memory Read
struct line** cache;
unsigned long int count;
int mw; //Memory Write
/**************************Create structure**********************/
typedef struct line{

    unsigned long int tag;
    int valid;
    unsigned long int time;

}line;

/**************************Declare funtion**********************/
void readsim(unsigned long int tagindex,unsigned long int setindex,int assoc);
line** createcache(int setnum,int assoc);
void writesim(unsigned long int tagindex,unsigned long int setindex,int assoc);
void readsiml(unsigned long int tagindex,unsigned long int setindex,int assoc);
void writesiml(unsigned long int tagindex,unsigned long int setindex,int assoc);
void printBits(size_t const size, void const * const ptr);
/**************************Main funtion**********************/
int main(int argc, char** argv){
/*argv[1] is cachesize
  argv[2] is blockSize
  argv[3] is cachepolicy
  argv[4] is assoc
  argv[5] is tracefile
  dirmap: ass: 1
  fullassoc: 1 set */

    int cachesize=atoi(argv[1]);
    int blocksize=atoi(argv[2]);
    int n;//set assoc;
    int setnum;
    int assoc;

    int b;
    int s;

    char work;
    unsigned long int address;
    unsigned long int setmask;
    unsigned long int tagindex;
    unsigned long int setindex;

    /**************************calculate assoc and setnum in different map form(dir,full,set asso)**********************/
    if(argv[4][0]=='d'){//direct map
        assoc=1;
        setnum=cachesize/blocksize;

    }else if(argv[4][5]!=':'){//fullassoc
        setnum=1;
        assoc=cachesize/blocksize;
    }else{//n way associat chache
        sscanf(argv[4],"assoc:%d",&n);
        assoc=n;
        setnum=cachesize/blocksize/n;
    }

    /**************************calculate how many bit in each index and mask**********************/
    b=log(blocksize)/log(2);
    s=log(setnum)/log(2);

    //blockmask=blocksize-1;
    setmask=((1<<s)-1);

    printf("%lu\n", setmask);
    printBits(sizeof(setmask), &setmask);

    /**************************update cache based on num**********************/
    cache=createcache(setnum,assoc);

    int cout = 0;

    FILE* fl;
    fl=fopen(argv[5],"r");

    while(fscanf(fl, "%c %lx\n", &work, &address)==2) {

        setindex = (address >> b) & setmask;
        tagindex = address >> (b + s);

/**************************fifo begin**********************/
        if (argv[3][0] == 'f') {

/**************************calculate the index**********************/

            if (cout == 0) {
                printf("Address: %lu\n", address);
                printBits(sizeof(address), &address);
                printf("Set Index: %lu\n", setindex);
                printBits(sizeof(setindex), &setindex);
                printf("Tag Index: %lu\n", tagindex);
                printBits(sizeof(tagindex), &tagindex);
                cout++;
            }

            if (work == 'R') {
                readsim(tagindex, setindex, assoc);

            } else if (work == 'W') {
                writesim(tagindex, setindex, assoc);
            }

/**************************lru begin**********************/
        } else if (argv[3][0] == 'l') {

            if (work == 'R') {
                readsiml(tagindex, setindex, assoc);

            } else if (work == 'W') {
                writesiml(tagindex, setindex, assoc);
            }

        } else {
            printf("policy not correct");
        }
    }
    printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", mr, mw, hit, miss);

    return 0;
}
/**************************malloc cache funtion**********************/
line** createcache(int setnum,int assoc){
    int i,j;
    cache=(line**)malloc(setnum*sizeof(line*));
    for(i=0;i<setnum;i++){
        cache[i]=(line*)malloc((assoc)*sizeof(line));
    }
    for(i=0;i<setnum;i++){
        for(j=0;j<assoc;j++){
            cache[i][j].valid=0;
        }
    }
    return cache;
}
///**************************read mode funtion**********************/
void readsim(unsigned long int tagindex,unsigned long int setindex,int assoc){

    int i,j,min;

    for(i=0;i<assoc;i++){
        if(cache[setindex][i].valid==0){
            miss++;
            mr++;
            count++;
            cache[setindex][i].valid=1;
            cache[setindex][i].tag=tagindex;
            cache[setindex][i].time=count;
            return;

        }else{

            if(cache[setindex][i].tag==tagindex){
                hit++;
                return;
            }
            if(i==(assoc-1)){
                miss++;
                mr++;
                min=0;
                for(j=0;j<assoc;j++){

                    if(cache[setindex][j].time<=cache[setindex][min].time){
                        min=j;
                    }
                }

                cache[setindex][min].valid=1;
                cache[setindex][min].tag=tagindex;
                count++;
                cache[setindex][min].time=count;
                return;
            }
        }
    }
    printf("210 problem");
}
///**************************write mode funtion**********************///
void writesim(unsigned long int tagindex,unsigned long int setindex,int assoc){

    int i,j,min;

    for(i=0;i<assoc;i++){
        if(cache[setindex][i].valid==0){
            miss++;
            mr++;
            mw++;
            count++;
            cache[setindex][i].valid=1;
            cache[setindex][i].tag=tagindex;
            cache[setindex][i].time=count;
            return;

        }else{
            if(cache[setindex][i].tag==tagindex){
                hit++;
                mw++;
                return;
            }

            if(i==(assoc-1)){
                miss++;
                mr++;
                mw++;
                min=0;
                for(j=0;j<assoc;j++){

                    if(cache[setindex][j].time<=cache[setindex][min].time){
                        min=j;
                    }
                }

                cache[setindex][min].valid=1;
                cache[setindex][min].tag=tagindex;
                count++;
                cache[setindex][min].time=count;
                return;
            }
        }
    }
    printf("360 problem");
}

void readsiml(unsigned long int tagindex,unsigned long int setindex,int assoc){

    int i,j,min;

    for(i=0;i<assoc;i++){
        if(cache[setindex][i].valid==0){
            miss++;
            mr++;
            count++;
            cache[setindex][i].valid=1;
            cache[setindex][i].tag=tagindex;
            cache[setindex][i].time=count;
            return;
        }else{
            if(cache[setindex][i].tag==tagindex){
                hit++;
                count++;
                cache[setindex][i].time=count;
                return;
            }

            if(i==(assoc-1)){
                miss++;
                mr++;

                min=0;
                for(j=0;j<assoc;j++){

                    if(cache[setindex][j].time<=cache[setindex][min].time){
                        min=j;
                    }
                }

                cache[setindex][min].valid=1;
                cache[setindex][min].tag=tagindex;
                count++;
                cache[setindex][min].time=count;

                return;
            }
        }
    }
    printf("210 problem");
}
///**************************write mode funtion**********************/
void writesiml(unsigned long int tagindex,unsigned long int setindex,int assoc){

    int i,j,min;

    for(i=0;i<assoc;i++){
        if(cache[setindex][i].valid==0){
            miss++;
            mr++;
            mw++;
            count++;
            cache[setindex][i].valid=1;
            cache[setindex][i].tag=tagindex;
            cache[setindex][i].time=count;
            return;
        }else{
            if(cache[setindex][i].tag==tagindex){
                hit++;
                mw++;
                count++;
                cache[setindex][i].time=count;
                return;
            }
            if(i==(assoc-1)){
                miss++;
                mr++;
                mw++;
                min=0;
                for(j=0;j<assoc;j++){

                    if(cache[setindex][j].time<=cache[setindex][min].time){
                        min=j;
                    }
                }
                cache[setindex][min].valid=1;
                cache[setindex][min].tag=tagindex;
                count++;
                cache[setindex][min].time=count;
                return;
            }
        }
    }
    printf("360 problem");
}

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}