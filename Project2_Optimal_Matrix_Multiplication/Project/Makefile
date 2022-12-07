all: matrixMul_cpu matrixMul_gpu_p1 matrixMul_gpu_p2

matrixMul_cpu:
	g++ $@.cpp -o $@.exe

matrixMul_gpu_p1:
	nvcc $@.cu -o $@.exe

matrixMul_gpu_p2:
	nvcc $@.cu -o $@.exe

clean:
	rm *.exe
