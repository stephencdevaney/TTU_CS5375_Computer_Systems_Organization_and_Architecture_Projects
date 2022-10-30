# README FOR CACHESIM.C VERSION ONLY (SINGLE LEVEL VERSION)
/*
 * CS5375 Computer Systems Organization and Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2022
 * Orginal Simulator By Yong Chen Modified by Stephen Devaney
 */

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
3.) In the command line interface type "gcc -o cachesim.exe cachesim.c


## Running the Program:
Running the program on windows:
1.) If not in the directory that the program was compiled in navigate back to that directory on the command line
2.) Make sure you have permisson to run the file "ls -l cachesim.exe" it should start with -rwx. If it does not type in "chmod 700 cachesim.exe"
3.) Type "cachesim.exe [options]"  // note options are not required and are listed below

Running the program on linux:
1.) If not in the directory that the program was compiled in navigate back to that directory on the command line
2.) Make sure you have permisson to run the file "ls -l cachesim.exe" it should start with -rwx. If it does not type in "chmod 700 cachesim.exe"
3.) Type "./cachesim.exe [options]"  // note options are not required and are listed below


## USAGE:
    cachesim.exe <direct, n-way, fully> <options> <trace file name>
    cachesim.exe --help
    cachesim.exe --h

    <direct, n-way, fully>: (one of the three arguements is required)
        direct:
            Simulates a Direct-Mapped Cache.
        n-way:
            Simulates a N-Way Associative Cache.
            Replace 'n' by an integer to indicate the way size
        fully:
            Simulates a Fully Associative Cache.

    <options>: (all optional)
        --block_size, -b:
            Set a block size for the cache. (Must be a power of two and in bytes!)
            Default for L1 is 64 bytes.
        --cache_size, -c:
            Set the total size for the cache. (Must be a power of two and in bytes!)
            Default for L1 is 32768 bytes which is 32KB.
        --help, -h:
            Will print out this help menu and exit the program.
            Note: With this option <direct, n-way, fully> and <trace file name> arguements are no longer required.
            This option will not run the cache even if <direct, n-way, fully> and <trace file name> arguements are provided.

     <trace file name>: (required)
         Path from current working directory to the trace file.
