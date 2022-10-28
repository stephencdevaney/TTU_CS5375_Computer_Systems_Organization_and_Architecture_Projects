/*
 * CS5375 Computer Systems Organization and Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2022
 * Orginal Simulator By Yong Chen Modified by Stephen Devaney
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include <limits.h>


//#define DBG


int LEVEL;                /* The levels of cache */


/*The data structure of direct-mapped cache*/
typedef struct ml_cache{
    unsigned *valid_field;   /* Valid field */
    unsigned *dirty_field;   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t *tag_field;     /* Tag field */
    char **data_field;  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
    int WAY_SIZE;             /* Associativity; 1-way = direct-mapped */
    int CACHE_SIZE;           /* Cache capacity in bytes (must be power of 2)*/
    int BLOCK_SIZE;           /* Cache block size (or cache line size) in bytes \\
                                     (must be power of 2). 4 Bytes = 1 Word */

    int NUM_BLOCKS;           /*(CACHE_SIZE / BLOCK_SIZE)*/
    int NUM_SETS;             /*(NUM_BLOCKS / WAY_SIZE)*/
} cache_t;


// Cache Access Function Prototypes
void cache_access(cache_t *cache, uint64_t address, int);  // Simulate cache access

// Helper Function Prototypes
int replacementPolicy(int);  // replacement policy utilized in cache_access function
uint64_t convert_address(char memory[]);  // Read the memory traces and convert it to binary
void parseArgs(int, char **, char **, FILE **, cache_t **);  // function to trim down code in main function and parse the arguements.
void setupCache(cache_t *);

// Print Function Prototypes
void printFinalOutput(cache_t *cache);  // print function for final output
void printUsage(char *);
void printHelp(char *);