#!/bin/bash
#SBATCH --job-name=CS5375_eraider_ID
#SBATCH --output=%j.%N.out
#SBATCH --error=%j.%N.err
#SBATCH --partition=matador
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --gpus-per-node=1
#SBATCH --account=cs5375
#SBATCH --reservation=cs5375_gpu

RUN_CPU=false
RUN_GPU_P1=false

#array=()
if [[ $RUN_CPU == true ]] ; then
	echo "RUN CPU"
fi
if [[ RUN_GPU_P1 == true ]] ; then
	echo "RUN GPU P1"
fi

#module load gcc cuda

#nvcc add_v5.cu -o add_v5.exe
#nvprof ./add_v5.exe
