#include <stdio.h>
#include <stdlib.h>

int get(unsigned short x, int n);
unsigned short comp(unsigned short x, int n);
unsigned short set(unsigned short x, int n, int v);

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Error: No File Given.\n");
        return 0;
    }

    FILE *fp = fopen(argv[1], "r");

    unsigned short x = 0;

    fscanf(fp, "%hu\n", &x);

    while(!feof(fp)){
        char c = 0;
        int n = 0;
        int v = 0;
        fscanf(fp, "%c", &c);
        if(c == 'g'){
            fscanf(fp, "%c", &c);
            fscanf(fp, "%c", &c);
            fscanf(fp, " %d ", &n);
            fscanf(fp, " %d ", &v);
            printf("%d\n", get(x, n));
        } else if(c == 'c'){
            fscanf(fp, "%c", &c);
            fscanf(fp, "%c", &c);
            fscanf(fp, "%c", &c);
            fscanf(fp, " %d ", &n);
            fscanf(fp, " %d ", &v);
            x = comp(x, n);
            printf("%hu\n", x);
        } else if(c == 's') {
            fscanf(fp, "%c", &c);
            fscanf(fp, "%c", &c);
            fscanf(fp, " %d ", &n);
            fscanf(fp, " %d ", &v);
            x = set(x, n, v);
            printf("%hu\n", x);
        }
    }
}

int get(unsigned short x, int n){
    return (x >> n) & 1;
}

unsigned short comp(unsigned short x, int n){
    return (1 << n) ^ x;
}

unsigned short set(unsigned short x, int n, int v) {
    if (v == 0) {
        return ~(1 << n) & x;
    } else {
        return (1 << n) | x;
    }
}