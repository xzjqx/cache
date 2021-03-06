#include "stdio.h"
#include "string.h"
#include "cache.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

char ch, str[20][40];
char file[20];

unsigned int add;

cache L1;

void init(int argc, char *argv[]);
void printHead();
void printTail();

int main(int argc, char *argv[]) {
    if( argc < 7 ) {
    	printf( "You should enter enough(7) parameters\n" );
    	return 0;
    }
    init(argc, argv);
    freopen(file,"r",stdin);
    printHead();
    
    while( scanf( "%c %x", &ch, &add )!=EOF ) {
    	getchar();

        if(ch == 'r') {//read
            L1.read(add);
        }
        else {//write
            L1.write(add);
        }

	}
    printTail();
	return 0;
}

void init(int argc, char *argv[]) {
    int sum[6];
    memset( sum, 0, sizeof(sum) );
    for(int i = 1; i < argc-1; i ++) {
        int x = 1;
        for(int j = strlen(argv[i])-1; j >= 0; j --) {
            sum[i] += x * (argv[i][j] - '0');
            x *= 10;
        }
    }
    int bs, si, as, rp, wp;
    bs = sum[1], si = sum[2], as = sum[3], rp = sum[4], wp = sum[5];
    
    strcpy(file, argv[6]);
    int fileLen = strlen(file);
    file[fileLen++] = '.';
    file[fileLen++] = 't';
    file[fileLen++] = 'x';
    file[fileLen++] = 't';
    
    strcpy(str[0],"L1_BLOCKSIZE:");
    strcpy(str[1],"L1_SIZE:");
    strcpy(str[2],"L1_ASSOC:");
    strcpy(str[3],"L1_REPLACEMENT_POLICY:");
    strcpy(str[4],"L1_WRITE_POLICY:");
    strcpy(str[5],"trace_file:");
    strcpy(str[6],"a. number of L1 reads:");
    strcpy(str[7],"b. number of L1 read misses:");
    strcpy(str[8],"c. number of L1 writes:");
    strcpy(str[9],"d. number of L1 write misses:");
    strcpy(str[10],"e. L1 miss rate:");
    strcpy(str[11],"f. number of writebacks from L1:");
    strcpy(str[12],"g. total memory traffic:");
    strcpy(str[13],"1. average access time:");

    L1.setData(bs,si,as,rp,wp);

}

void printHead() {
    printf("  ===== Simulator configuration =====\n" );
    printf("  %-22s%13d\n", str[0], L1.blocksize);
    printf("  %-22s%13d\n", str[1], L1.size);
    printf("  %-22s%13d\n", str[2], L1.assoc);
    printf("  %-22s%13d\n", str[3], L1.replacement_policy);
    printf("  %-22s%13d\n", str[4], L1.write_policy);
    printf("  %-22s%13s\n", str[5], file);
    printf("  ===================================\n");
    printf("\n===== L1 contents =====\n" );
}

void printTail() {
    for(int i = 0; i < L1.set; i ++) {
        printf("set%4d:", i);
        for(int j = 0; j < L1.assoc; j ++) {
            printf("%8x ", L1.tag[i][j]);
            if(!L1.write_policy) {
                if(L1.flagD[i][j])
                    printf("D");
                else
                    printf(" ");
            }
        }
        puts("");
    }

    L1.e = (L1.b*1.0 + L1.d*1.0) / (L1.a*1.0 + L1.c*1.0);
    if(L1.write_policy) //WTNA
        L1.g = L1.b + L1.c;
    else //WBWA
        L1.g = L1.b + L1.d + L1.f;

    double ht = 0.25 + 2.5*(L1.size/512)/1024 + 0.025*(L1.blocksize/16) + 0.025*L1.assoc;
    double mp = 20 + 0.5*(L1.blocksize/16);
    L1.aat = ht + L1.e * mp;

    printf("\n  ====== Simulation results (raw) ======\n" );
    printf("  %-32s%6d\n", str[6], L1.a);
    printf("  %-32s%6d\n", str[7], L1.b);
    printf("  %-32s%6d\n", str[8], L1.c);
    printf("  %-32s%6d\n", str[9], L1.d);
    printf("  %-32s%.4lf\n", str[10], L1.e);
    printf("  %-32s%6d\n", str[11], L1.f);
    printf("  %-32s%6d\n\n", str[12], L1.g);
    printf("  ==== Simulation results (performance) ====\n");
    printf("  %-32s%.4lf ns", str[13], L1.aat );
}