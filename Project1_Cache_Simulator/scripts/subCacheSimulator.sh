#!/bin/bash

#SBATCH -J cacheSim
#SBATCH -o %x.o%j
#SBATCH -e %x.e%j
#SBATCH -p nocona
#SBATCH --account cs5375
#SBATCH --reservation cs5375
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH -t 00:05:00


# Stephen Devaney
# CS5375 - Computer Systems Organization and Architecture
# Programming Project 1
# Cache Simulator
# 10/04/2022


RUN_DIRECT_MAPPED=true
RUN_NWAY_ASSOCIATIVE=true
RUN_FULLY_ASSOCIATIVE=true
way=(2 4 8)
line_size=(16 32 128)
cache_size=(16384 32768 65536)
trace_files=("hw5_memoryaddr" "trace.stream_10" "trace.stream_20" "trace.stream" "trace.hpcg")


# if cachesim.exe does not exist in the current working directory try to compile it
if ! test -f "./cachesim.exe"; then
	# if cachesim.c and cachemsim.h are in the current working directory compile them else output an error message
	if test -f "./cachesim.c" && test -f "./cachesim.h"; then
		# if the make file exist in the current workiing directory use it other wise call the commands to compile it
		if test -f "./Makefile"; then
			make
		else
			gcc -c -o cachesim.o cachesim.c
			gcc -o cachesim.exe cachesim.o
			rm cachesim.o
		fi
	else
		echo "Please make sure cachesim.c and cachesim.h are both in your current directory!"
	fi
fi


# if the compilation was successful run the cache simulator other wise output error message
if test -f "./cachesim.exe"; then
	# Create an output directory if it does not exists
	if ! test -d "CachesimOutput"; then
		mkdir CachesimOutput
	fi


	# Runs a direct mapped cache if RUN_DIRECT_MAPPED is true.
	if [[ $RUN_DIRECT_MAPPED == true ]] ; then	
		#Test the Direct Mapped Cache for all trace files and place all output into one output file
		echo -e "Simulating Direct-Mapped Cache for all trace files:" > CachesimOutput/direct_mapped_output.txt
		for i in ${!trace_files[@]}; do
			echo -e "\n\nSimulating Direct Mapped Cache on tracefile ${trace_files[$i]}:" >> CachesimOutput/direct_mapped_output.txt
			./cachesim.exe direct trace_for_students/${trace_files[$i]} >> CachesimOutput/direct_mapped_output.txt
		done
	fi


	# Runs a N-Way Associative Cache if RUN_NWAY_ASSOCIATIVE is true.
	if [[ $RUN_NWAY_ASSOCIATIVE == true ]] ; then
		#Test the N-Way Associative Cache for all ways, trace files, line sizes, and cache sizes and place all output into the respective n-way cache output file
		for i in ${!way[@]}; do
			echo -e "Test all line sizes and cache sizes of a ${way[i]} way associative cache for all trace files:" > CachesimOutput/${way[$i]}way_associative_output.txt
			for j in ${!line_size[@]}; do
				for k in ${!trace_files[@]}; do
					echo -e "\n\nSimulating ${way[$i]} cache with a fixed cache size of 32768 bytes and a line size of ${line_size[$j]} bytes on tracefile ${trace_files[$k]}." >> CachesimOutput/${way[$i]}way_associative_output.txt
					./cachesim.exe ${way[i]}-way -b ${line_size[$j]} -c 32768 trace_for_students/${trace_files[$k]} >> CachesimOutput/${way[$i]}way_associative_output.txt
				done
			done

			for j in ${!cache_size[@]}; do
				for k in ${!trace_files[@]}; do
					echo -e "\n\nSimulating ${way[$i]} cache with a fixed line size of 64 bytes and a cache size of ${cache_size[$j]} bytes on tracefile ${trace_files[$k]}." >> CachesimOutput/${way[$i]}way_associative_output.txt
					./cachesim.exe ${way[i]}-way -b 64 -c ${cache_size[$j]} trace_for_students/${trace_files[$k]} >> CachesimOutput/${way[$i]}way_associative_output.txt
				done
			done
		done
	fi


	# Runs a FULLY Associative Cache if RUN_FULLY_ASSOCIATIVE is true.
	if [[ $RUN_FULLY_ASSOCIATIVE == true ]] ; then
		echo -e "Test all line sizes and cache sizes of a fully associative cache for all trace files:" > CachesimOutput/fully_associative_output.txt
		for i in ${!line_size[@]}; do
			for j in ${!trace_files[@]}; do
				echo -e "\n\nSimulating fully associative cache with a fixed cache size of 32768 bytes and a line size of ${line_size[$i]} bytes on tracefile ${trace_files[$j]}." >> CachesimOutput/fully_associative_output.txt
				./cachesim.exe fully -b ${line_size[$i]} -c 32768 trace_for_students/${trace_files[$j]} >> CachesimOutput/fully_associative_output.txt
			done
		done

		for i in ${!cache_size[@]}; do
			for j in ${!trace_files[@]}; do
				echo -e "\n\nSimulating fully associative cache with a fixed line size of 64 bytes and a cache size of ${cache_size[$i]} bytes on tracefile ${trace_files[$j]}." >> CachesimOutput/fully_associative_output.txt
				./cachesim.exe fully -b 64 -c ${cache_size[$i]} trace_for_students/${trace_files[$j]} >> CachesimOutput/fully_associative_output.txt
			done
		done
	fi


	# cleans up .exe file after the completion of the simulator
	if test -f "./Makefile"; then
		make clean
	else
		rm -f *.exe *.o
	fi
else
	echo "Unable to run cache simulator cachesim.exe not found!"
fi
