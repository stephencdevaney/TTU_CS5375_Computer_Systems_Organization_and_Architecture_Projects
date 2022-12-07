all: matrixMul_cpu matrixMul_gpu_p1

matrixMul_cpu:
	g++ $@.cpp -o $@.exe

matrixMul_gpu_p1:
	nvcc $@.cu -o $@.exe

clean:
	rm *.exe
