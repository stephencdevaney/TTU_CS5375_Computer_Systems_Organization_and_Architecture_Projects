/*
 * CS5375 Computer Systems Organization and Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2022
 * Orginal Simulator By Yong Chen Modified by Stephen Devaney
 */



README FOR multi_level_cachesim.C VERSION ONLY (Multi LEVEL VERSION)



## Requirements:
To compile this project a C compiler is required. One can be found at https://www.mingw-w64.org/downloads/
After installing the compiler make sure to add the compiler path to your path environment



## Compilating the Program:
Compilation using provided make file:
1.) Open command line interface
2.) Navigate to folder on command line interface
3.) In the command line interface type "make"  // the make command could be different based the downloaded compiler for example MinGW uses "mingw32-make". To find out for user navigate to your compiler folder and look for "make" contined in the executable file name.

Compilation without using provided make file:
1.) Open command line interface
2.) Navigate to folder on command line interface
3.) In the command line interface type "gcc -o multi_level_cachesim.exe multi_level_cachesim.c



## Running the Program:
Running the program on windows:
1.) If not in the directory that the program was compiled in navigate back to that directory on the command line
2.) Make sure you have permisson to run the file "ls -l multi_level_cachesim.exe" it should start with -rwx. If it does not type in "chmod 700 multi_level_cachesim.exe"
3.) Type "multi_level_cachesim.exe [options]"  // note options are not required and are listed below

Running the program on linux:
1.) If not in the directory that the program was compiled in navigate back to that directory on the command line
2.) Make sure you have permisson to run the file "ls -l multi_level_cachesim.exe" it should start with -rwx. If it does not type in "chmod 700 multi_level_cachesim.exe"
3.) Type "./multi_level_cachesim.exe [options]"  // note options are not required and are listed below



## Usage: 
	multi_level_cachesim.exe <n-level> <options> <trace file name>
	Help Usage 1: multi_level_cachesim.exe --help
	Help Usage 2: multi_level_cachesim.exe --h

<n-level>: (required)
        Simulates a single or multi level Cache.
        Replace 'n' by an integer to indicate the level size (lowest level cache).

<options>: (all optional)
	--direct, -d:
		Sets the current level cache to be direct mapped
        	Can be used multiple times for multiple levels of cache. Will be used in order
	--nway, -n:
      	Sets the current level cache to be n-way assocative
        	Can be used multiple times for multiple levels of cache. Will be used in order
        	Replace 'n' by an integer to indicate the way size.
	--fully, -f:
        	Sets the current level cache to be fully assocative
        	Can be used multiple times for multiple levels of cache. Will be used in order
	--block_size, -b:
        	Set a block size for the cache. (Must be a power of two and in bytes!)
        	Can be used multiple times for multiple levels of cache. Will be used in order
            	Example: -b 64 -b 128
        	Default for L1 is 64 bytes. Default for lower level caches will be the same as the previous level.
	--cache_size, -c:
        	Set the total size for the cache. (Must be a power of two and in bytes!)
        	Can be used multiple times for multiple levels of cache. Will be used in order
           		Example: -c 65536 -c 131072 (64KB and 128KB)
        	Default for L1 is 65536 bytes which is 64KB. Default for lower levels will be multiplied by 16
            	Example one arguement of 32768 is given for 2 levels: L1 CACHE_SIZE = 32768, L2 524288 (32KB and 512KB)--help, -h:
        	Will print out this help menu and exit the program.
        	Note: With this option <n-level> and <trace file name> arguements are no longer required.
           	This option will not run the cache even if <n-level> and <trace file name> arguements are provided.

<trace file name>: (required)
        Path from current working directory to the trace file.