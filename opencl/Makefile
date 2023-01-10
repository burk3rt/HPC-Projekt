# MacOS
macos:
	clang -framework OpenCL -o tsp-ant.out ant.c

# Windows
all:
	gcc -I"$(CUDA_PATH)/include" -L"$(CUDA_PATH)/lib/x64" -o tsp-ant.exe opencl-skeleton.c -lOpenCL
