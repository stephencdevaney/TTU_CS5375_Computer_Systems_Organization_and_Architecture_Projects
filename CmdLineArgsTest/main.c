/*To give this the ability to check for multi level caches consider this logic:
 * <lowest level cache> <direct, nway, fully>0 <direct, nway, fully>1 ... <direct, nway, fully>lowestlevel <options>0 <options>1, ... <options>lowestlevel <trace_file_name>*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

int LEVEL_SIZE;           /* # of the levels of cache*/
int BLOCK_SIZE;           /* Cache block size (or cache line size) in bytes \\
                                 (must be power of 2). 4 Bytes = 1 Word */
int WAY_SIZE;             /* Associativity; 1-way = direct-mapped */
int CACHE_SIZE;           /* Cache capacity in bytes (must be power of 2)*/

int NUM_BLOCKS;           /*(CACHE_SIZE / BLOCK_SIZE)*/
int NUM_SETS;             /*(NUM_BLOCKS / WAY_SIZE)*/


void printFinalOutput();  // print function for final output
void printUsage(char *);
void printHelp(char *);


int main(int argc, char **argv)
{
    char *trace_file_name;
    LEVEL_SIZE = 1;
    BLOCK_SIZE = 64;
    WAY_SIZE = 1;
    CACHE_SIZE = 32768;
    if (argc < 4) {
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
        if(strcmp(argv[1], "--help") != 0 && strcmp(argv[1], "-h") != 0) optind += 2;
        if(strcmp(argv[argc-1], "--help") != 0 && strcmp(argv[argc-1], "-h") != 0){
            trace_file_name = argv[argc-1];
            printf("%s", trace_file_name);
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
                printHelp(argv[0]);
                return 0;
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
                return 1;
            case '?':
            default:
                printf("%s: option '%c' is invalid\n", argv[0], optopt);
                printUsage(argv[0]);
                return 1;
        }
    }
    if(strstr(argv[1], "-level") != NULL){
        char *arguement_copy; 
        arguement_copy = malloc(sizeof(char) * (strlen(argv[1])+1));
        strcpy(arguement_copy, argv[1]);
        WAY_SIZE = atoi(strtok(arguement_copy, "-"));
    }
    else{
        printUsage(argv[0]);
        return 1;
    }
    NUM_BLOCKS = (CACHE_SIZE / BLOCK_SIZE);
    if(strncmp(argv[2], "direct", 6) == 0) WAY_SIZE = 1;
    else if(strncmp(argv[2], "fully", 5) == 0) WAY_SIZE = NUM_BLOCKS;
    else{
        if(strstr(argv[2], "-way") != NULL){
            char *arguement_copy; 
            arguement_copy = malloc(sizeof(char) * (strlen(argv[2])+1));
            strcpy(arguement_copy, argv[2]);
            WAY_SIZE = atoi(strtok(arguement_copy, "-"));
        }
        else{
            printUsage(argv[0]);
            return 1;
        }
    }
    NUM_SETS = (NUM_BLOCKS / WAY_SIZE);
    
    printFinalOutput();
    return 0;
}


void printUsage(char *argv){
    // print the usage to run the program from the command line
    printf("Usage: %s <m-level> <direct, n-way, fully> <options> <trace file name>\n", argv);
    printf("Help Usage 1: %s --help\n", argv);
    printf("Help Usage 2: %s --h\n", argv);
}


void printHelp(char *argv){
    // print the help informaion reguarding options to run the program from the command line
    printf("m-level: (required)\n");
    printf("Select how many levels the cache has by replacing the placeholder 'm' with an integer.\n");
    
    printf("\n<direct, n-way, fully>: (one of the three arguements is required)\n");
    printf("direct:\n");
    printf("\tSimulates a Direct-Mapped Cache.\n");
    printf("n-way:\n");
    printf("\tSimulates a N-Way Associative Cache.\n");
    printf("Replace 'n' by an integer to indicate the way size.\n");
    printf("fully:\n");
    printf("\tSimulates a Fully Associative Cache.\n");
    
    printf("\nOPTIONS: (all optional)\n");
    printf("--block_size, -b:\n");
    printf("\tSet a block size for the cache. (Must be a power of two and in bytes!)\n");
    printf("\tDefault for L1 is 64 bytes.\n");
    printf("--cache_size, -c:\n");
    printf("\tSet the total size for the cache. (Must be a power of two and in bytes!)\n");
    printf("\tDefault for L1 is 32768 bytes which is 32KB.\n");
    printf("--help, -h:\n");
    printf("\tWill print out this help menu and exit the program.\n");
    printf("\tNote: With this option <direct, n-way, fully> and <trace file name> arguements are no longer required.\n");
    printf("\t      This option will not run the cache even if <direct, n-way, fully> and <trace file name> arguements are provided.\n");
}


void printFinalOutput(){
    // output cache type and information
    printf("=============================================\n");
    if(WAY_SIZE == 1) printf("L%d Cache type:    Direct-Mapped Cache\n", 1);
    else if(WAY_SIZE == NUM_BLOCKS) printf("L%d Cache type:    Fully Associative Cache\n", 1);
    else printf("L%d Cache type:    %d-Way Associative Cache\n", 1, WAY_SIZE);
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
}