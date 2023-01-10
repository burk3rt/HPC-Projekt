all:
	gcc -g -o tsp-ant.exe ant.c q../cities/csv-input.c -fopenmp -lm
exec:
	gcc -g -o tsp-ant.exe ant.c ../cities/csv-input.c -lm
	tsp-ant.exe
exec-opt:
	gcc -g -o tsp-ant.exe ant.c ../cities/csv-input.cc -O3 -lm
	tsp-ant.exe

mac:
	gcc-12 -o tsp-ant.out ant.c ../cities/csv-input.c -lm
mac-omp:
	gcc-12 -fopenmp -o tsp-ant.out ant.c ../cities/csv-input.c -lm
mac-omp-opt:
	gcc-12 -fopenmp -O3 -o tsp-ant.out ant.c ../cities/csv-input.c -lm