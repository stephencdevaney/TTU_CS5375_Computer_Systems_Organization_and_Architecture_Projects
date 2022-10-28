/*
 * CS5375 Computer Systems Organization and Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2022
 * Orginal Simulator By Yong Chen Modified by Stephen Devaney
 */


#include "multi_level_cachesim.h"


int main(int argc, char **argv){
    argc = 3;
    argv[1] = "2-level";
    argv[2] = "trace_for_students/trace.hpcg";
    char *trace_file_name;
    FILE *fp;
    char mem_request[20];
    uint64_t address;
    cache_t *multilevel_cache;
    parseArgs(argc, argv, &trace_file_name, &fp, &multilevel_cache);  // parse command line arguements
    setupCache(multilevel_cache);  // perform cache setup
    
    // run memory trace file through cache
    while (fgets(mem_request, 20, fp)!= NULL){  // byte address from the trace file
        address = convert_address(mem_request);  // convert address to binary
        cache_access(multilevel_cache, address, 0);  // access the cache with the address
    }
    
    // print final output
    printFinalOutput(multilevel_cache);
    return 0;
}



/* ========================================================================================================================================================
 * Cache Access Functions */
void cache_access(cache_t *multilevel_cache, uint64_t address, int current_LEVEL){
    // Name: cache_access
    // Input: address: binary address from the trace file, current_LEVEL: current cache level, multilevel_cache[LEVEL]: array of caches
    // Output: multilevel_cache[LEVEL]: modified array of caches
    // Purpose: allows for access to the cache
    
    // The block address is the block address shifted right by log of the block size for all cache types
    // The index is the block address % number of blocks for direct mapped, block address % (number of blocks / way size) for n-way associative, and 0 for fully associative as fully associative has no index due to its design. 
    //      block_addr % NUM_SETS satisfies this requirement as NUM_SETS = NUM_BLOCKS for direct mapped, NUM_SETS = NUM_BLOCKS/WAY_SIZE for n-way, and NUM_SETS = 1 for fully associative.
    // The tag is the block address shifted right by log base two of number of blocks for direct mapped, the block address shifted right by log base two of (number of blocks / way size) for n-way associative, and just the block address for fully associative.
    //      similar to the previous bullet block_addr >> log2(NUM_SETS) satisfied this requirement as NUM_SETS = NUM_BLOCKS for direct mapped, NUM_SETS = NUM_BLOCKS/WAY_SIZE for n-way, and NUM_SETS = 1 for fully associative since log2(1) = 0 no bit shift is performed and fully associative is also satisified.
    uint64_t block_addr = address >> (unsigned)log2(multilevel_cache[current_LEVEL].BLOCK_SIZE);
    uint64_t index = block_addr % multilevel_cache[current_LEVEL].NUM_SETS;
    uint64_t tag = block_addr >> (unsigned)log2(multilevel_cache[current_LEVEL].NUM_SETS);
    #ifdef DBG
        printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
    #endif
    
    // loop through the assocative of the queue note will only perform one iteration of the loop for direct mapped, n for n-way, and block size for fully associative
    short hitflag = 0;  // needed to indicate a miss after a hit has been found
    uint64_t way_index = 0;  // keeps track of the index for n-way and fully associative cache. Will only remain 0 for a direct mapped cache based on for loop parameters.
    for(; way_index < multilevel_cache[current_LEVEL].WAY_SIZE; way_index++){
        // check for hits by checkint the valid field and comparing the tag to the current index in the tag field
        //      Note: way_index = 0 for direct and index = 0 for fully associative as required by cache types
        if(multilevel_cache[current_LEVEL].valid_field[index * multilevel_cache[current_LEVEL].WAY_SIZE + way_index] && multilevel_cache[current_LEVEL].tag_field[index * multilevel_cache[current_LEVEL].WAY_SIZE + way_index] == tag){ // Cache hit 
            multilevel_cache[current_LEVEL].hits += 1;
            hitflag = 1;
            #ifdef DBG
                printf("Hit!\n");
            #endif
            break;
        }
        else if (multilevel_cache[current_LEVEL].valid_field[index * multilevel_cache[current_LEVEL].WAY_SIZE + way_index] == 0){  // Stops the loop when the first empty slot is found. This allows for the cache to fill from low index to high index.
            break;
        }
    }
    
    if(!hitflag){  // if the hit flag was not raised we have a miss
        // Cache miss 
        multilevel_cache[current_LEVEL].misses += 1;
        if(way_index == multilevel_cache[current_LEVEL].WAY_SIZE) way_index = replacementPolicy(multilevel_cache[current_LEVEL].WAY_SIZE);  // if the cache is fulll get a replacement way index
        #ifdef DBG
            printf("Miss!\n");
        #endif
        if (multilevel_cache[current_LEVEL].valid_field[index] && multilevel_cache[current_LEVEL].dirty_field[index]) {
            // Write the cache block back to memory 
        }
        // place the address in the cache
        multilevel_cache[current_LEVEL].tag_field[index * multilevel_cache[current_LEVEL].WAY_SIZE + way_index] = tag;
        multilevel_cache[current_LEVEL].valid_field[index * multilevel_cache[current_LEVEL].WAY_SIZE + way_index] = 1;
        multilevel_cache[current_LEVEL].dirty_field[index * multilevel_cache[current_LEVEL].WAY_SIZE + way_index] = 0;
        
        // access lower level cache
        if(current_LEVEL + 1 < LEVEL) cache_access(multilevel_cache, address, current_LEVEL + 1);
    }
}



/* ========================================================================================================================================================
 * Helper Functions */
int replacementPolicy(int WAY_SIZE){
    // Name: replacementPolicy
    // Input: WAY_SIZE: the way size for the current cache
    // Output: replacement_index: the index determined to be replace by the replacement policy(random). Will return 0 for direct-mapped cache since it does not require a replacement policy
    // Purpose: determines the index to be replace based on a random replacement policy
    int replacement_index = 0;
    if(WAY_SIZE > 1){  // if way size = 1 then the cache is a direct mapped then no replacement policy is needed, otherwise use random replacement policy
        replacement_index = rand() % WAY_SIZE; // calculates the random index in the set to be replaced
    }
    return replacement_index;
}


uint64_t convert_address(char memory_addr[]){
/* Converts the physical 32-bit address in the trace file to the "binary" \\
 * (a uint64 that can have bitwise operations on it)
 ** Function Completely written by Dr. Chen **/
    uint64_t binary = 0;
    int i = 0;

    while (memory_addr[i] != '\n') {
        if (memory_addr[i] <= '9' && memory_addr[i] >= '0') {
            binary = (binary*16) + (memory_addr[i] - '0');
        } else {
            if(memory_addr[i] == 'a' || memory_addr[i] == 'A') {
                binary = (binary*16) + 10;
            }
            if(memory_addr[i] == 'b' || memory_addr[i] == 'B') {
                binary = (binary*16) + 11;
            }
            if(memory_addr[i] == 'c' || memory_addr[i] == 'C') {
                binary = (binary*16) + 12;
            }
            if(memory_addr[i] == 'd' || memory_addr[i] == 'D') {
                binary = (binary*16) + 13;
            }
            if(memory_addr[i] == 'e' || memory_addr[i] == 'E') {
                binary = (binary*16) + 14;
            }
            if(memory_addr[i] == 'f' || memory_addr[i] == 'F') {
                binary = (binary*16) + 15;
            }
        }
        i++;
    }
    #ifdef DBG
        printf("%s converted to %llu\n", memory_addr, binary);
    #endif
    return binary;
}


void parseArgs(int argc, char **argv, char **trace_file_name, FILE **fp, cache_t **multilevel_cache){
    // Name: parseArgs
    // Input: argc: command line argument count, argv: command line arguments
    // Output: trace_file_name: trace file name recieved from the command line, fp: pointer opened from the trace file, multilevel_cache[LEVEL]: array of caches
    // Purpose: Parses command line arguements for the cache and starts cache initiation
    
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
    // if first check of usage matches perform second check and setup arguements
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
    
    // allocate memory for cache and initalize values for WAY_SIZE, CACHE_SIZE, and BLOCK_SIZE
    *multilevel_cache = malloc(LEVEL * sizeof(cache_t));
    for(int i = 0; i < LEVEL; i++){
        (*multilevel_cache)[i].WAY_SIZE = -1;
        (*multilevel_cache)[i].CACHE_SIZE = -1;
        (*multilevel_cache)[i].BLOCK_SIZE = -1;
    }
    
    // create indexes for values that can be called multiple times
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
            case 'h':  // help
                printUsage(argv[0]);
                printHelp(argv[0]);
                exit(0);
            case 'd':  // direct-mapped
                if(WAY_SIZE_index < LEVEL) (*multilevel_cache)[WAY_SIZE_index++].WAY_SIZE = 1;
                else{
                    printf("%s: too many arguements for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    printHelp(argv[0]);
                    exit(1);
                }
                break;
            case 'n':  // n-way associative
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
            case 'f':  // fully associative
                if(WAY_SIZE_index < LEVEL) (*multilevel_cache)[WAY_SIZE_index++].WAY_SIZE = INT_MAX;
                else{
                    printf("%s: too many arguements for option '-%c'\n", argv[0], optopt);
                    printUsage(argv[0]);
                    printHelp(argv[0]);
                    exit(1);
                }
                break;
            case 'b':  // block size
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
            case 'c':  // cache size
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
    // Name: setupCache
    // Input: multilevel_cache[LEVEL]: partially intialized array of caches
    // Output: multilevel_cache[LEVEL]: fully setup array of caches
    // Purpose: finalizes setup for the multilevel cache
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
void printFinalOutput(cache_t *multilevel_cache){
    // Name: printFinalOutput
    // Input: multilevel_cache[LEVEL]: array of caches
    // Output: cache informaion outputted to the console
    // Purpose: prints the final output of the cache to the console
    if(LEVEL > 1){
        printf("================GLOBAL RESULTS===============\n");
        // Print out global results
        printf("LEVELS: %d\n", LEVEL);
        printf("---------------------------------------------\n");
        printf("Cache Hits:    %d\n", multilevel_cache[0].hits + multilevel_cache[0].misses - multilevel_cache[LEVEL-1].misses);  // global hits = total accesses - lowest level misses; total accesses = L1 hits + L1 misses
        printf("Cache Misses:  %d\n", multilevel_cache[LEVEL-1].misses);  // global misses = misses in lowest level cache
        printf("Cache Hit Rate:    %f\n", (double)(multilevel_cache[0].hits + multilevel_cache[0].misses - multilevel_cache[LEVEL-1].misses)/(double)(multilevel_cache[0].hits + multilevel_cache[0].misses));
        printf("Cache Miss Rate:   %f\n", (double)multilevel_cache[LEVEL-1].misses/(double)(multilevel_cache[0].hits + multilevel_cache[0].misses));
        printf("=============================================\n\n");
    }
    
    for(int i = 0; i < LEVEL; i++){
        // output cache type and information
        printf("===============L%d LOCAL RESULTS==============\n", i+1);
        if(multilevel_cache[i].WAY_SIZE == 1) printf("Cache type:    Direct-Mapped Cache\n");
        else if(multilevel_cache[i].WAY_SIZE == multilevel_cache[i].NUM_BLOCKS) printf("Cache type:    Fully Associative Cache\n");
        else printf("Cache type:    %d-Way Associative Cache\n", multilevel_cache[i].WAY_SIZE);
        printf("WAY SIZE = %d\n", multilevel_cache[i].WAY_SIZE);
        printf("BLOCK SIZE = %d Bytes\n", multilevel_cache[i].BLOCK_SIZE);
        printf("CACHE SIZE = %d Bytes\n", multilevel_cache[i].CACHE_SIZE);
        printf("NUMBER OF BLOCKS = %d\n", multilevel_cache[i].NUM_BLOCKS);
        printf("NUMBER OF SETS = %d\n", multilevel_cache[i].NUM_SETS);
        printf("---------------------------------------------\n");
        // Print out local results
        printf("Cache Hits:    %d\n", multilevel_cache[i].hits);
        printf("Cache Misses:  %d\n", multilevel_cache[i].misses);
        printf("Cache Hit Rate:    %f\n", (double)multilevel_cache[i].hits/(double)(multilevel_cache[i].hits + multilevel_cache[i].misses));
        printf("Cache Miss Rate:   %f\n", (double)multilevel_cache[i].misses/(double)(multilevel_cache[i].hits + multilevel_cache[i].misses));
        printf("=============================================\n\n");
    }
}


void printUsage(char *argv){
    // Name: printUsage
    // Input: argv: file name/path 
    // Output: usage outputted to the console
    // Purpose: prints the usage to run the program from the command line
    printf("Usage: %s <n-level> <options> <trace file name>\n", argv);
    printf("Help Usage 1: %s --help\n", argv);
    printf("Help Usage 2: %s --h\n", argv);
}


void printHelp(char *argv){
    // Name: printHelp
    // Input: argv: file name/path 
    // Output: help outputted to the console
    // Purpose: print the help informaion reguarding options to run the program from the command line
    printf("<n-level>: (required)\n");
    printf("\tSimulates a single or multi level Cache.\n");
    printf("\tReplace 'n' by an integer to indicate the level size (lowest level cache).\n\n");
    
    printf("<options>: (all optional)\n");
    printf("--direct, -d:\n");
    printf("\tSets the current level cache to be direct mapped\n");
    printf("\tCan be used multiple times for multiple levels of cache. Will be used in order\n");
    printf("--nway, -n:\n");
    printf("\tSets the current level cache to be n-way assocative\n");
    printf("\tCan be used multiple times for multiple levels of cache. Will be used in order\n");
    printf("\tReplace 'n' by an integer to indicate the way size.\n\n");
    printf("--fully, -f:\n");
    printf("\tSets the current level cache to be fully assocative\n");
    printf("\tCan be used multiple times for multiple levels of cache. Will be used in order\n");
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