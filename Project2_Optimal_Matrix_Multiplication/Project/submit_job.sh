#!/bin/bash
#SBATCH --job-name=matrixMul
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
RUN_GPU_P2=false
RUN_GPU_P3=false
RUN_GPU_P3_V2=false
versions=()


module load gcc cuda


if [[ $RUN_CPU == true ]] ; then
	make matrixMul_cpu
	{ time ./matrixMul_cpu.exe; } &> matrixMul_cpu.txt
fi
if [[ $RUN_GPU_P1 == true ]] ; then
	versions+=("matrixMul_gpu_p1")
fi
if [[ $RUN_GPU_P2 == true ]] ; then
	versions+=("matrixMul_gpu_p2")
fi
if [[ $RUN_GPU_P3 == true ]] ; then
	versions+=("matrixMul_gpu_p3")
fi
if [[ $RUN_GPU_P3_V2 == true ]] ; then
        versions+=("matrixMul_gpu_p3_v2")
fi


for i in ${!versions[@]}; do
	make ${versions[$i]}
	{ nvprof ./${versions[$i]}.exe; } &> ${versions[$i]}.txt
done


make clean
rm *.err *.out
