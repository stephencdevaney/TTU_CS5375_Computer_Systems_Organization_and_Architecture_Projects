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

RUN_CPU=true
RUN_GPU_P1=true
versions=()

module load gcc cuda

if [[ $RUN_CPU == true ]] ; then
	make matrixMul_cpu
	time ./matrixMul_cpu.exe &> matrixMul_cpu.txt
fi
if [[ $RUN_GPU_P1 == true ]] ; then
	versions+="matrixMul_gpu_p1"
fi

for i in ${!versions[@]}; do
	make ${versions[$i]}
	time ./${versions[$i]}.exe &> ${versions[$i]}.txt
done

make clean

#nvcc add_v5.cu -o add_v5.exe
#nvprof ./add_v5.exe
