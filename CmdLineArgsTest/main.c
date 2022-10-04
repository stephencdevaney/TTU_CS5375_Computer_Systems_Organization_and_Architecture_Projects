#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>


void printUsage(char *);


int main(int argc, char **argv)
{
    int BLOCK_SIZE = 64;               /* Cache block size (or cache line size) in bytes \\
                                     (must be power of 2). 4 Bytes = 1 Word */
    int WAY_SIZE = 1;                 /* Associativity; 1-way = direct-mapped */
    int CACHE_SIZE = 32768;               /* Cache capacity in bytes (must be power of 2)*/
    int NUM_BLOCKS;
    int NUM_SETS;
    short errorFlag = 0;
    if (argc < 3) {
        if (argc == 2){
            if(strcmp(argv[1], "--help") != 0 && strcmp(argv[1], "-h") != 0) errorFlag = 1;
        }
        else errorFlag = 1;
    }
    
    char* trace_file_name = argv[argc-1];
    printf(trace_file_name);
    
    if (errorFlag == 1){
        printUsage(argv[0]);
        return 1;
    }
    
    // get changes to default values for ndata, dim, kk, and the number of query points from the command line
    static struct option long_options[] = {
        {"block_size", 1, NULL, 'b'},
        {"cache_size", 1, NULL, 'c'},
        {NULL, 0, NULL, 0}
    };
    // modify default values based on values for ndata, dim, kk, and the number of query points recieved from the command line
    char c;
    while((c = getopt_long(argc, argv, "?b:c:", long_options, NULL)) != -1){
        switch(c){
            case 'b':
                BLOCK_SIZE = atoi(optarg);
                break;
            case 'c':
                CACHE_SIZE = atoi(optarg);
                break;
            default:
                // invalid
                printf("Unknown Arguement\n");
                return 1;
                break;
        }
    }
    NUM_BLOCKS = (CACHE_SIZE / BLOCK_SIZE);
    NUM_SETS = (NUM_BLOCKS / WAY_SIZE);
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
	return 0;
}


void printUsage(char *argv){
    printf("Usage: %s <direct> <options> <trace file name>\n", argv);
    printf("Help Usage 1: %s --help\n", argv);
    printf("Help Usage 2: %s --h\n", argv);
}