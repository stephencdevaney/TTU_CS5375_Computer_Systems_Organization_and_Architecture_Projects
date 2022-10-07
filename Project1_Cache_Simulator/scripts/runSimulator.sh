#/bin/bash
# Stephen Devaney
# CS5375 - Computer Systems Organization and Architecture
# Programming Project 1
# Cache Simulator
# 10/04/2022


export RUN_DIRECT_MAPPED=false
export RUN_NWAY_ASSOCIATIVE=true
export RUN_FULLY_ASSOCIATIVE=false


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


	# Runs a N-Way Associative Cache if RUN_NWAY_ASSOCIATIVE is true.
	if [[ $RUN_NWAY_ASSOCIATIVE == true ]] ; then
		way=(2 4 8)
		line_size=(16 32 128)
		cache_size=(16384 32768 65536)
		for i in ${!way[@]}; do
			echo -e "Testing all line sizes and cache sizes of a ${!way[i]} way associative cache for all trace files.:" > CachesimOutput/${way[$i]}way_associative_output.txt
			for j in ${!line_size[@]}; do
				echo -e "Simulating ${way[$i]} cache with a fixed cache size of 32768 bytes and a line size of ${line_size[$j]} bytes on tracefile hw5_memoryaddr.\n" >> CachesimOutput/${way[$i]}way_associative_output.txt
				./cachesim.exe ${!way[i]} -b ${line_size[$j]} -c 32768 trace_for_students/hw5_memoryaddr >> CachesimOutput/${way[$i]}way_associative_output.txt
			done

			for k in ${!cache_size[@]}; do
				echo -e "${cache_size[$k]}\n" >> CachesimOutput/${way[$i]}way_associative_output.txt
			done
        done
	fi


	# Runs a FULLY Associative Cache if RUN_FULLY_ASSOCIATIVE is true.
	if [[ $RUN_FULLY_ASSOCIATIVE == true ]] ; then
		line_size=(16 32 128)
		cache_size=(16384 32768 65536)
        echo -e "" > fully_associative_output.txt
		for i in ${!line_size[@]}; do
			echo ${line_size[$i]}
		done

		for i in ${!cache_size[@]}; do
			echo ${cache_size[$i]}
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
