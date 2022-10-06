/*
 * CS5375 Computer Systems Organization and Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2022
 * By Yong Chen
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "cachesim.h"

int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Usage: %s <direct, N-way, fully> <trace file name>\n", argv[0]);
        return 1;
    }
    
    BLOCK_SIZE = 128;
    CACHE_SIZE = 32768;
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
            printf("Usage: %s <direct, N-way, fully> <trace file name>\n", argv[0]);
            return 1;
        }
    }
    NUM_SETS = (NUM_BLOCKS / WAY_SIZE);

    char* trace_file_name = argv[2];
    struct cache_t d_cache;
    char mem_request[20];
    uint64_t address;
    FILE *fp;
    
    /* Initialization */
    d_cache.valid_field = (unsigned*)malloc(NUM_BLOCKS * sizeof(unsigned));
    d_cache.dirty_field = (unsigned*)malloc(NUM_BLOCKS * sizeof(unsigned));
    d_cache.tag_field = (uint64_t*)malloc(NUM_BLOCKS * sizeof(uint64_t));
    for (int i=0; i<NUM_BLOCKS; i++) {
        d_cache.valid_field[i] = 0;
        d_cache.dirty_field[i] = 0;
        d_cache.tag_field[i] = 0;
    }
    d_cache.hits = 0;
    d_cache.misses = 0;
    /* Opening the memory trace file */
    
//        if (strncmp(argv[1], "direct", 6)==0) { /* Simulating direct-mapped cache */
//            /* Read the memory request address and access the cache */
//            while (fgets(mem_request, 20, fp)!= NULL) {
//                address = convert_address(mem_request);
//                cache_access(&d_cache, address);
//            }
//            printFinalOutput(&d_cache);
//        }
    fp = fopen(trace_file_name, "r");
    while (fgets(mem_request, 20, fp)!= NULL) {
        address = convert_address(mem_request);
        cache_access(&d_cache, address);
    }
    printFinalOutput(&d_cache);
    fclose(fp);

    return 0;
}

uint64_t convert_address(char memory_addr[]){
/* Converts the physical 32-bit address in the trace file to the "binary" \\
 * (a uint64 that can have bitwise operations on it) */
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


int replacementPolicy(){
    int replacement_index = 0;
    if(WAY_SIZE > 1){  // if way size = 1 then the cache is a direct mapped then no replacement policy is needed, otherwise use random replacement policy
        replacement_index = rand() % WAY_SIZE; // calculates the random index in the set to be replaced
    }
    return replacement_index;
}


void cache_access(struct cache_t *cache, uint64_t address){
    /* The block address is the block address shifted right by log of the block size for all cache types
     * The index is the block address % number of blocks for direct mapped, block address % (number of blocks / way size) for n-way associative, and 0 for fully associative as fully associative has no index due to its design. 
            block_addr % NUM_SETS satisfies this requirement as NUM_SETS = NUM_BLOCKS for direct mapped, NUM_SETS = NUM_BLOCKS/WAY_SIZE for n-way, and NUM_SETS = 1 for fully associative.
     * The tag is the block address shifted right by log base two of number of blocks for direct mapped, the block address shifted right by log base two of (number of blocks / way size) for n-way associative, and just the block address for fully associative.
            similar to the previous bullet block_addr >> log2(NUM_SETS) satisfied this requirement as NUM_SETS = NUM_BLOCKS for direct mapped, NUM_SETS = NUM_BLOCKS/WAY_SIZE for n-way, and NUM_SETS = 1 for fully associative since log2(1) = 0 no bit shift is performed and fully associative is also satisified.
     */
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_SETS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_SETS);
    #ifdef DBG
        printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
    #endif
    
    // loop through the assocative of the queue note will only perform one iteration of the loop for direct mapped, n for n-way, and block size for fully associative
    short hitflag = 0;  // needed to indicate a miss after a hit has been found
    uint64_t way_index;  // keeps track of the index for n-way and fully associative cache. Will only remain 0 for a direct mapped cache based on for loop parameters.
    for(way_index = 0; way_index < WAY_SIZE; way_index++){
        // check for hits by checkint the valid field and comparing the tag to the current index in the tag field
        //      Note: way_index = 0 for direct and index = 0 for fully associative as required by cache types
        if(cache->valid_field[index * WAY_SIZE + way_index] && cache->tag_field[index * WAY_SIZE + way_index] == tag){ /* Cache hit */
            cache->hits += 1;
            hitflag = 1;
            #ifdef DBG
                printf("Hit!\n");
            #endif
            // dependent on question answer may need multilevel cache policy for hit
            break;
        }
        else if (cache->valid_field[index * WAY_SIZE + way_index] == 0){  // Stops the loop when the first empty slot is found. This allows for the cache to fill from low index to high index.
            break;
        }
    }
    
    if(!hitflag){  // if the hit flag was not raised we have a miss
        /* Cache miss */
        cache->misses += 1;
        way_index = replacementPolicy();
        #ifdef DBG
            printf("Miss!\n");
        #endif
        if (cache->valid_field[index] && cache->dirty_field[index]) {
            /* Write the cache block back to memory */
        }
        // place the address in the cache
        cache->tag_field[index * WAY_SIZE + way_index] = tag;
        cache->valid_field[index * WAY_SIZE + way_index] = 1;
        cache->dirty_field[index * WAY_SIZE + way_index] = 0;
        
        // need multilevel cache policy for miss
    }
}


void printFinalOutput(struct cache_t *cache){
    printf("\n=============================================\n");
    if(WAY_SIZE == 1) printf("L%d Cache type:    Direct-Mapped Cache\n", 1);
    else if(WAY_SIZE == NUM_BLOCKS) printf("L%d Cache type:    Fully Associative Cache\n", 1);
    else printf("L%d Cache type:    %d-Way Associative Cache\n", 1, WAY_SIZE);
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
    printf("================LOCAL RESULTS================\n");
    /*Print out the results*/
    printf("Cache Hits:    %d\n", cache->hits);
    printf("Cache Misses:  %d\n", cache->misses);
    printf("Cache Hit Rate:    %f\n", (double)cache->hits/(double)(cache->hits + cache->misses));
    printf("Cache Miss Rate:   %f\n", (double)cache->misses/(double)(cache->hits + cache->misses));
    printf("=============================================\n");
}