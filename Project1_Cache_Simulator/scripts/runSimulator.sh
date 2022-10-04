#/bin/bash


# Stephen Devaney
# CS5375 - Computer Systems Organization and Architecture
# Programming Project 1
# Cache Simulator
# 10/04/2022

export RUN_DIRECT_MAPPED=true

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
	if ! test -d "cachesimOutput"; then
		mkdir CachesimOutput
	fi

	# Runs a direct mapped cache if RUN_DIRECT_MAPPED is true.
	if [[ $RUN_DIRECT_MAPPED == true ]] ; then
		#Test the Direct Mapped Cache for all trace files and place all output into one output file
		echo -e "Simulating Direct Mapped Cache on tracefile 'hw5_memoryaddr'" > CachesimOutput/direct_mapped_output.txt
		./cachesim.exe direct trace_for_students/hw5_memoryaddr >> CachesimOutput/direct_mapped_output.txt
		echo -e "\n\nSimulating Direct Mapped Cache on tracefile 'trace.stream_10'" >> CachesimOutput/direct_mapped_output.txt
		./cachesim.exe direct trace_for_students/trace.stream_10 >> CachesimOutput/direct_mapped_output.txt
		echo -e "\n\nSimulating Direct Mapped Cache on tracefile 'trace.stream_20'" >> CachesimOutput/direct_mapped_output.txt
		./cachesim.exe direct trace_for_students/trace.stream_20 >> CachesimOutput/direct_mapped_output.txt
		echo -e "\n\nSimulating Direct Mapped Cache on tracefile 'trace.stream'" >> CachesimOutput/direct_mapped_output.txt
		./cachesim.exe direct trace_for_students/trace.stream >> CachesimOutput/direct_mapped_output.txt
		echo -e "\n\nSimulating Direct Mapped Cache on tracefile 'trace.hpcg'" >> CachesimOutput/direct_mapped_output.txt
		./cachesim.exe direct trace_for_students/trace.hpcg >> CachesimOutput/direct_mapped_output.txt
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
