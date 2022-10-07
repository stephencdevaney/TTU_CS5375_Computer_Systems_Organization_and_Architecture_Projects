/*To give this the ability to check for multi level caches consider this logic:
 * <lowest level cache> <direct, nway, fully>0 <direct, nway, fully>1 ... <direct, nway, fully>lowestlevel <options>0 <options>1, ... <options>lowestlevel <trace_file_name>*/

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
    char* trace_file_name;
    FILE *fp;
    if (argc < 3) {
        if (argc == 2){
            if(strcmp(argv[1], "--help") != 0 && strcmp(argv[1], "-h") != 0){
                printUsage(argv[0]);
                return 1;
            }
        }
        else{
            printUsage(argv[0]);
            return 1;
        }
    }
    else{
        optind += 1;
        trace_file_name = argv[argc-1];
        fp = fopen(trace_file_name, "r");
        if(fp == NULL){
            printf("Failed to open file %s\n", trace_file_name);
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // get changes to default values for ndata, dim, kk, and the number of query points from the command line
    static struct option long_options[] = {
        {"block_size", 1, NULL, 'b'},
        {"cache_size", 1, NULL, 'c'},
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    // modify default values based on values for ndata, dim, kk, and the number of query points recieved from the command line
    int c;
    while((c = getopt_long(argc, argv, "+:hb:c:", long_options, NULL)) != -1){
        switch(c){
            case 'h':
                printUsage(argv[0]);
                return 0;
                break;
            case 'b':
                BLOCK_SIZE = atoi(optarg);
                if(BLOCK_SIZE == 0){
                    printf("%s: invalid argument for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    return 1;
                }
                break;
            case 'c':
                CACHE_SIZE = atoi(optarg);
                if(CACHE_SIZE == 0){
                    printf("%s: invalid argument for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    return 1;
                }
                break;
            case ':':
                printf("%s: option '-%c' requires an argument\n", argv[0], optopt);
                break;
            case '?':
            default:
                printf("%s: option '%c' is invalid\n", argv[0], optopt);
                printUsage(argv[0]);
                return 1;
                break;
        }
    }
    
    NUM_BLOCKS = (CACHE_SIZE / BLOCK_SIZE);
    if(strncmp(argv[1], "direct", 6) == 0) WAY_SIZE = 1;
    else if(strncmp(argv[1], "fully", 5) == 0) WAY_SIZE = NUM_BLOCKS;
    else{
        if(strstr(argv[1], "-way") != NULL){
            char *arguement_copy; 
            arguement_copy = malloc(sizeof(char) * (strlen(argv[1])+1));
            strcpy(arguement_copy, argv[1]);
            WAY_SIZE = atoi(strtok(arguement_copy, "-"));
        }
        else{
            printUsage(argv[0]);
            return 1;
        }
    }
    NUM_SETS = (NUM_BLOCKS / WAY_SIZE);
    
    
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
	return 0;
}


void printUsage(char *argv){
    printf("Usage: %s <direct, fully, associative> <options> <trace file name>\n", argv);
    printf("Help Usage 1: %s --help\n", argv);
    printf("Help Usage 2: %s --h\n", argv);
}