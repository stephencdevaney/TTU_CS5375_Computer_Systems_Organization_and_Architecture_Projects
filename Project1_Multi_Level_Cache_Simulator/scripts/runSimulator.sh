#/bin/bash
# Stephen Devaney
# CS5375 - Computer Systems Organization and Architecture
# Programming Project 1
# Cache Simulator
# 10/04/2022

CACHISM_DIR=/home/stdevane/CS5375/CS5375_Computer_Systems_Organization_and_Architecture_Projects/Project1_Multi_Level_Cache_Simulator
RUN_DIRECT_MAPPED=true
RUN_NWAY_ASSOCIATIVE=true
RUN_FULLY_ASSOCIATIVE=true
RUN_2LEVEL=true
way=(2 4 8)
line_size=(16 32 128)
cache_size=(16384 32768 65536)
trace_files=("hw5_memoryaddr" "trace.stream_10" "trace.stream_20" "trace.stream" "trace.hpcg")


# if cachesim.exe does not exist in the current working directory try to compile it
if ! test -f "$CACHISM_DIR/multi_level_cachesim.exe"; then
	# if cachesim.c and cachemsim.h are in the current working directory compile them else output an error message
	if test -f "$CACHISM_DIR/multi_level_cachesim.c" && test -f "$CACHISM_DIR/multi_level_cachesim.h"; then
		# if the make file exist in the current workiing directory use it other wise call the commands to compile it
		if test -f "$CACHISM_DIR/Makefile"; then
			make -C $CACHISM_DIR
		else
			gcc -c -o $CACHISM_DIR/multi_level_cachesim.o $CACHISM_DIR/multi_level_cachesim.c
			gcc -o $CACHISM_DIR/multi_level_cachesim.exe $CACHISM_DIR/multi_level_cachesim.o -lm
			rm $CACHISM_DIR/multi_level_cachesim.o
		fi
	else
		echo "Please make sure multi_level_cachesim.c and multi_level_cachesim.h are both in your current directory!"
	fi
fi


# if the compilation was successful run the cache simulator other wise output error message
if test -f "$CACHISM_DIR/multi_level_cachesim.exe"; then
	# Create an output directory if it does not exists
	if ! test -d "$CACHISM_DIR/CachesimOutput"; then
		mkdir $CACHISM_DIR/CachesimOutput
	fi


	# Runs a direct mapped cache if RUN_DIRECT_MAPPED is true.
	if [[ $RUN_DIRECT_MAPPED == true ]] ; then	
		#Test the Direct Mapped Cache for all trace files and place all output into one output file
		echo -e "Simulating Direct-Mapped Cache for all trace files:" > $CACHISM_DIR/CachesimOutput/direct_mapped_output.txt
		for i in ${!trace_files[@]}; do
			echo -e "\n\nSimulating Direct Mapped Cache on tracefile ${trace_files[$i]}:" >> $CACHISM_DIR/CachesimOutput/direct_mapped_output.txt
			$CACHISM_DIR/multi_level_cachesim.exe 1-level -d -c 32768 $CACHISM_DIR/trace_for_students/${trace_files[$i]} >> $CACHISM_DIR/CachesimOutput/direct_mapped_output.txt
		done
	fi


	# Runs a N-Way Associative Cache if RUN_NWAY_ASSOCIATIVE is true.
	if [[ $RUN_NWAY_ASSOCIATIVE == true ]] ; then
		#Test the N-Way Associative Cache for all ways, trace files, line sizes, and cache sizes and place all output into the respective n-way cache output file
		for i in ${!way[@]}; do
			echo -e "Test all line sizes and cache sizes of a ${way[i]}-way associative cache for all trace files:" > $CACHISM_DIR/CachesimOutput/${way[$i]}way_associative_output.txt
			for j in ${!line_size[@]}; do
				for k in ${!trace_files[@]}; do
					echo -e "\n\nSimulating ${way[$i]}-way cache with a fixed cache size of 32768 bytes and a line size of ${line_size[$j]} bytes on tracefile ${trace_files[$k]}." >> $CACHISM_DIR/CachesimOutput/${way[$i]}way_associative_output.txt
					$CACHISM_DIR/multi_level_cachesim.exe 1-level -n ${way[i]} -b ${line_size[$j]} -c 32768 $CACHISM_DIR/trace_for_students/${trace_files[$k]} >> $CACHISM_DIR/CachesimOutput/${way[$i]}way_associative_output.txt
				done
			done

			for j in ${!cache_size[@]}; do
				for k in ${!trace_files[@]}; do
					echo -e "\n\nSimulating ${way[$i]} cache with a fixed line size of 64 bytes and a cache size of ${cache_size[$j]} bytes on tracefile ${trace_files[$k]}." >> $CACHISM_DIR/CachesimOutput/${way[$i]}way_associative_output.txt
					$CACHISM_DIR/multi_level_cachesim.exe 1-level -n ${way[i]} -c ${cache_size[$j]} $CACHISM_DIR/trace_for_students/${trace_files[$k]} >> $CACHISM_DIR/CachesimOutput/${way[$i]}way_associative_output.txt
				done
			done
		done
	fi


	# Runs a FULLY Associative Cache if RUN_FULLY_ASSOCIATIVE is true.
	if [[ $RUN_FULLY_ASSOCIATIVE == true ]] ; then
		echo -e "Test all line sizes and cache sizes of a fully associative cache for all trace files:" > $CACHISM_DIR/CachesimOutput/fully_associative_output.txt
		for i in ${!line_size[@]}; do
			for j in ${!trace_files[@]}; do
				echo -e "\n\nSimulating fully associative cache with a fixed cache size of 32768 bytes and a line size of ${line_size[$i]} bytes on tracefile ${trace_files[$j]}." >> $CACHISM_DIR/CachesimOutput/fully_associative_output.txt
				$CACHISM_DIR/multi_level_cachesim.exe 1-level -f -b ${line_size[$i]} -c 32768 $CACHISM_DIR/trace_for_students/${trace_files[$j]} >> $CACHISM_DIR/CachesimOutput/fully_associative_output.txt
			done
		done

		for i in ${!cache_size[@]}; do
			for j in ${!trace_files[@]}; do
				echo -e "\n\nSimulating fully associative cache with a fixed line size of 64 bytes and a cache size of ${cache_size[$i]} bytes on tracefile ${trace_files[$j]}." >> $CACHISM_DIR/CachesimOutput/fully_associative_output.txt
				$CACHISM_DIR/multi_level_cachesim.exe 1-level -f -c ${cache_size[$i]} $CACHISM_DIR/trace_for_students/${trace_files[$j]} >> $CACHISM_DIR/CachesimOutput/fully_associative_output.txt
			done
		done
	fi

	# Runs a 2 LEVEL Cache if RUN_2LEVEL is true.
	if [[ RUN_2LEVEL == true ]] ; then
		echo -e "Test a 2-level where L1 is a 2-way cache with a block size of 64 bytes and a cache size of 65536 bytes and where L2 is a 8-way cache with a block size of 64 bytes and a cache size of 1048576 bytes for all trace files:" > $CACHISM_DIR/CachesimOutput/multilevel_output.txt
		for i in ${!trace_files[@]}; do
			echo -e "\n\nSimulating 2-Level Cache on tracefile ${trace_files[$i]}:" >> $CACHISM_DIR/CachesimOutput/multilevel_output.txt
			$CACHISM_DIR/multi_level_cachesim.exe 2-level $CACHISM_DIR/trace_for_students/${trace_files[$i]} >> $CACHISM_DIR/CachesimOutput/multilevel_output.txt
		done
	fi

	# cleans up .exe file after the completion of the simulator
	if test -f "$CACHISM_DIR/Makefile"; then
		make clean -C $CACHISM_DIR/
	else
		rm -f $CACHISM_DIR/*.exe $CACHISM_DIR/*.o
	fi
else
	echo "Unable to run cache simulator multi_level_cachesim.exe not found!"
fi
