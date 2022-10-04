#/bin/bash


# Stephen Devaney
# CS5375 - Computer Systems Organization and Architecture
# Programming Project 1
# Cache Simulator
# 10/04/2022

export RUN_DIRECT_MAPPED=true

if [[ $RUN_DIRECT_MAPPED == true ]] ; then
	#Test the Direct Mapped Cache for all trace files and place all output into one output file
	echo -e "Simulating Direct Mapped Cache on tracefile 'hw5_memoryaddr'" > direct_mapped_output.txt
	./cachesim.exe direct trace_for_students/hw5_memoryaddr >> direct_mapped_output.txt
	echo -e "\n\nSimulating Direct Mapped Cache on tracefile 'trace.stream_10'" >> direct_mapped_output.txt
	./cachesim.exe direct trace_for_students/trace.stream_10 >> direct_mapped_output.txt
	echo -e "\n\nSimulating Direct Mapped Cache on tracefile 'trace.stream_20'" >> direct_mapped_output.txt
	./cachesim.exe direct trace_for_students/trace.stream_20 >> direct_mapped_output.txt
	echo -e "\n\nSimulating Direct Mapped Cache on tracefile 'trace.stream'" >> direct_mapped_output.txt
	./cachesim.exe direct trace_for_students/trace.stream >> direct_mapped_output.txt
	echo -e "\n\nSimulating Direct Mapped Cache on tracefile 'trace.hpcg'" >> direct_mapped_output.txt
	./cachesim.exe direct trace_for_students/trace.hpcg >> direct_mapped_output.txt
fi
