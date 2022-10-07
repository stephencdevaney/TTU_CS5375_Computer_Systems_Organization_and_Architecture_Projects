/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>
#include <math.h>

int BLOCK_SIZE;           /* Cache block size (or cache line size) in bytes \\
                                 (must be power of 2). 4 Bytes = 1 Word */
int WAY_SIZE;             /* Associativity; 1-way = direct-mapped */
int CACHE_SIZE;           /* Cache capacity in bytes (must be power of 2)*/

int NUM_BLOCKS;           /*(CACHE_SIZE / BLOCK_SIZE)*/
int NUM_SETS;             /*(NUM_BLOCKS / WAY_SIZE)*/

//#define DBG

/*The data structure of direct-mapped cache*/
struct cache_t {
    unsigned *valid_field;   /* Valid field */
    unsigned *dirty_field;   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t *tag_field;     /* Tag field */
    char **data_field;  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};



uint64_t convert_address(char memory[]);  // Read the memory traces and convert it to binary
int replacementPolicy();  // replacement policy utilized in cache_access function
void cache_access(struct cache_t *cache, uint64_t address);  // Simulate cache access
void printFinalOutput(struct cache_t *cache);  // print function for final output
void printUsage(char *);
void printHelp(char *);