#include "multi_level_cachesim.h"



int main(int argc, char **argv){
    argc = 3;
    argv[1] = "2-level";
    argv[2] = "trace_for_students/hw5_memoryaddr";
    char *trace_file_name;
    FILE *fp;
    //char mem_request[20];
    //uint64_t address;
    cache_t *multilevel_cache;
    parseArgs(argc, argv, &trace_file_name, &fp, &multilevel_cache);
    setupCache(multilevel_cache);
    
    printf("LEVEL: %d\n", LEVEL);
    printf("FILE NAME: %s\n", trace_file_name);
    for(int i = 0; i < LEVEL; i++) {
        printf("LEVEL: %d, WAY_SIZE: %d\n", i+1, multilevel_cache[i].WAY_SIZE);
        printf("LEVEL: %d, CACHE_SIZE: %d\n", i+1, multilevel_cache[i].CACHE_SIZE);
        printf("LEVEL: %d, BLOCK_SIZE: %d\n\n", i+1, multilevel_cache[i].BLOCK_SIZE);
        
        //for(int j = 0; j < multilevel_cache[i].NUM_BLOCKS; j++) printf("LEVEL: %d, tag_field[%d]: %d\n\n", i+1, j, multilevel_cache[i].valid_field[j]);
    }
    return 0;
}


/* ========================================================================================================================================================
 * Helper Functions */
void parseArgs(int argc, char **argv, char **trace_file_name, FILE **fp, cache_t **multilevel_cache){
    // Check first and last command line arguements and makes sure they match the usage
    if (argc < 3){
        if (argc == 2){
            if(strcmp(argv[1], "--help") != 0 && strcmp(argv[1], "-h") != 0){
                printUsage(argv[0]);
                exit(1);
            }
        }
        else{
            printUsage(argv[0]);
            exit(1);
        }
    }
    else{
        if(strcmp(argv[1], "--help") != 0 && strcmp(argv[1], "-h") != 0){
            if(strstr(argv[1], "-level") != NULL){
                char *arguement_copy;
                arguement_copy = malloc(sizeof(char) * (strlen(argv[1])+1));
                strcpy(arguement_copy, argv[1]);
                LEVEL = atoi(strtok(arguement_copy, "-"));
                if(LEVEL < 1){
                    printUsage(argv[0]);
                    exit(1);
                }
            }
            else{
                printUsage(argv[0]);
                exit(1);
            }
            optind += 1;
        }
        if(strcmp(argv[argc-1], "--help") != 0 && strcmp(argv[argc-1], "-h") != 0){
            *trace_file_name = argv[argc-1];
            /* Opening the memory trace file */
            *fp = fopen(*trace_file_name, "r");
            if(fp == NULL){
                printf("Failed to open file %s\n", *trace_file_name);
                printUsage(argv[0]);
                exit(1);
            }
        }
    }
    
    *multilevel_cache = malloc(LEVEL * sizeof(cache_t));
    for(int i = 0; i < LEVEL; i++){
        (*multilevel_cache)[i].WAY_SIZE = -1;
        (*multilevel_cache)[i].CACHE_SIZE = -1;
        (*multilevel_cache)[i].BLOCK_SIZE = -1;
    }
    
    int WAY_SIZE_index = 0;
    int CACHE_SIZE_index = 0;
    int BLOCK_SIZE_index = 0;
    // get changes to default values for BLOCK_SIZE and CACHE_SIZE from the command line
    static struct option long_options[] = {
        {"direct", 0, NULL, 'd'},
        {"nway", 1, NULL, 'n'},
        {"fully", 0, NULL, 'f'},
        {"block_size", 1, NULL, 'b'},
        {"cache_size", 1, NULL, 'c'},
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    
    // modify default values based on values for BLOCK_SIZE and CACHE_SIZE recieved from the command line
    int c;
    while((c = getopt_long(argc, argv, "+:dn:fhb:c:", long_options, NULL)) != -1){
        switch(c){
            case 'h':
                printUsage(argv[0]);
                printHelp(argv[0]);
                exit(0);
            case 'd':
                if(WAY_SIZE_index < LEVEL) (*multilevel_cache)[WAY_SIZE_index++].WAY_SIZE = 1;
                else{
                    printf("%s: too many arguements for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    printHelp(argv[0]);
                    exit(1);
                }
                break;
            case 'n':
                if(WAY_SIZE_index < LEVEL) (*multilevel_cache)[WAY_SIZE_index++].WAY_SIZE = atoi(optarg);
                else{
                    printf("%s: too many arguements for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    printHelp(argv[0]);
                    exit(1);
                }
                if((*multilevel_cache)[WAY_SIZE_index - 1].WAY_SIZE <= 0){
                    printf("%s: invalid argument for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    exit(1);
                }
                break;
            case 'f':
                if(WAY_SIZE_index < LEVEL) (*multilevel_cache)[WAY_SIZE_index++].WAY_SIZE = INT_MAX;
                else{
                    printf("%s: too many arguements for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    printHelp(argv[0]);
                    exit(1);
                }
                break;
            case 'b':
                if(BLOCK_SIZE_index < LEVEL) (*multilevel_cache)[BLOCK_SIZE_index++].BLOCK_SIZE = atoi(optarg);
                else{
                    printf("%s: too many arguements for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    printHelp(argv[0]);
                    exit(1);
                }
                if((*multilevel_cache)[BLOCK_SIZE_index - 1].BLOCK_SIZE <= 0){
                    printf("%s: invalid argument for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    exit(1);
                }
                break;
            case 'c':
                if(CACHE_SIZE_index < LEVEL) (*multilevel_cache)[CACHE_SIZE_index++].CACHE_SIZE = atoi(optarg);
                else{
                    printf("%s: too many arguements for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    printHelp(argv[0]);
                    exit(1);
                }
                if((*multilevel_cache)[CACHE_SIZE_index - 1].CACHE_SIZE <= 0){
                    printf("%s: invalid argument for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    exit(1);
                }
                break;
            case ':':
                printf("%s: option '-%c' requires an argument\n", argv[0], optopt);
                exit(1);
            case '?':
            default:
                printf("%s: option '%c' is invalid\n", argv[0], optopt);
                printUsage(argv[0]);
                exit(1);
        }
    }
}


void setupCache(cache_t *multilevel_cache){
    if(multilevel_cache[0].CACHE_SIZE == -1) multilevel_cache[0].CACHE_SIZE = 65536;
    if(multilevel_cache[0].BLOCK_SIZE == -1) multilevel_cache[0].BLOCK_SIZE = 64;
    multilevel_cache[0].NUM_BLOCKS = multilevel_cache[0].CACHE_SIZE/multilevel_cache[0].BLOCK_SIZE;
    if(multilevel_cache[0].WAY_SIZE == -1) multilevel_cache[0].WAY_SIZE = 2;
    else if(multilevel_cache[0].WAY_SIZE > multilevel_cache[0].NUM_BLOCKS || multilevel_cache[0].WAY_SIZE == INT_MAX) multilevel_cache[0].WAY_SIZE = multilevel_cache[0].NUM_BLOCKS;
    multilevel_cache[0].NUM_SETS = multilevel_cache[0].NUM_BLOCKS/multilevel_cache[0].WAY_SIZE;
    
        /* Initialization */
    multilevel_cache[0].valid_field = (unsigned*)malloc(multilevel_cache[0].NUM_BLOCKS * sizeof(unsigned));
    multilevel_cache[0].dirty_field = (unsigned*)malloc(multilevel_cache[0].NUM_BLOCKS * sizeof(unsigned));
    multilevel_cache[0].tag_field = (uint64_t*)malloc(multilevel_cache[0].NUM_BLOCKS * sizeof(uint64_t));
    for (int i = 0; i < multilevel_cache[0].NUM_BLOCKS; i++) {
        multilevel_cache[0].valid_field[i] = 0;
        multilevel_cache[0].dirty_field[i] = 0;
        multilevel_cache[0].tag_field[i] = 0;
    }
    multilevel_cache[0].hits = 0;
    multilevel_cache[0].misses = 0;
    
    for(int i = 1; i < LEVEL; i++){
        if(multilevel_cache[i].CACHE_SIZE == -1) multilevel_cache[i].CACHE_SIZE = multilevel_cache[i - 1].CACHE_SIZE * 16;
        if(multilevel_cache[i].BLOCK_SIZE == -1) multilevel_cache[i].BLOCK_SIZE = multilevel_cache[i - 1].BLOCK_SIZE;
        multilevel_cache[i].NUM_BLOCKS = multilevel_cache[i].CACHE_SIZE/multilevel_cache[i].BLOCK_SIZE;
        if(multilevel_cache[i].WAY_SIZE == -1) multilevel_cache[i].WAY_SIZE = multilevel_cache[i-1].WAY_SIZE * 4;
        else if(multilevel_cache[i].WAY_SIZE > multilevel_cache[i].NUM_BLOCKS || multilevel_cache[i].WAY_SIZE == INT_MAX) multilevel_cache[i].WAY_SIZE = multilevel_cache[i].NUM_BLOCKS;
        multilevel_cache[i].NUM_SETS = multilevel_cache[i].NUM_BLOCKS/multilevel_cache[i].WAY_SIZE;
        
        /* Initialization */
        multilevel_cache[i].valid_field = (unsigned*)malloc(multilevel_cache[i].NUM_BLOCKS * sizeof(unsigned));
        multilevel_cache[i].dirty_field = (unsigned*)malloc(multilevel_cache[i].NUM_BLOCKS * sizeof(unsigned));
        multilevel_cache[i].tag_field = (uint64_t*)malloc(multilevel_cache[i].NUM_BLOCKS * sizeof(uint64_t));
        for (int j = 0; j < multilevel_cache[i].NUM_BLOCKS; j++) {
            multilevel_cache[i].valid_field[j] = 0;
            multilevel_cache[i].dirty_field[j] = 0;
            multilevel_cache[i].tag_field[j] = 0;
        }
        multilevel_cache[i].hits = 0;
        multilevel_cache[i].misses = 0;
    }
}


 /* ========================================================================================================================================================
 * Print Functions */
void printUsage(char *argv){
    // print the usage to run the program from the command line
    printf("Usage: %s <n-level> <options> <trace file name>\n", argv);
    printf("Help Usage 1: %s --help\n", argv);
    printf("Help Usage 2: %s --h\n", argv);
}


void printHelp(char *argv){
    // print the help informaion reguarding options to run the program from the command line
    printf("<n-level>: (required)\n");
    printf("\tSimulates a single or multi level Cache.\n");
    printf("Replace 'n' by an integer to indicate the level size (lowest level cache).\n\n");
    
    printf("<options>: (all optional)\n");
    printf("--block_size, -b:\n");
    printf("\tSet a block size for the cache. (Must be a power of two and in bytes!)\n");
    printf("\tCan be used multiple times for multiple levels of cache. Will be used in order\n");
    printf("\t\tExample: -b 64 -b 128\n");
    printf("\tDefault for L1 is 64 bytes. Default for lower level caches will be the same as the previous level.\n");
    printf("--cache_size, -c:\n");
    printf("\tSet the total size for the cache. (Must be a power of two and in bytes!)\n");
    printf("\tCan be used multiple times for multiple levels of cache. Will be used in order\n");
    printf("\t\tExample: -c 65536 -c 131072 (64KB and 128KB)\n");
    printf("\tDefault for L1 is 65536 bytes which is 64KB. Default for lower levels will be multiplied by 16\n");
    printf("\t\tExample one arguement of 32768 is given for 2 levels: L1 CACHE_SIZE = 32768, L2 524288 (32KB and 512KB)");
    printf("--help, -h:\n");
    printf("\tWill print out this help menu and exit the program.\n");
    printf("\tNote: With this option <n-level> and <trace file name> arguements are no longer required.\n");
    printf("\t      This option will not run the cache even if <n-level> and <trace file name> arguements are provided.\n\n");
    
     printf("<trace file name>: (required)\n");
     printf("\tPath from current working directory to the trace file.\n");
}