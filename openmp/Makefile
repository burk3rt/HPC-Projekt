all:
	gcc -g -o tsp-ant.exe ant.c q../cities/csv-input.c -fopenmp -lm
exec:
	gcc -g -o tsp-ant.exe ant.c ../cities/csv-input.c -lm
	tsp-ant.exe
execomp:
	gcc -g -o tsp-ant.exe ant.c ../cities/csv-input.c -fopenmp -lm
	tsp-ant.exe
clean:
	rm -rf *.exe
mac:
	gcc-12 -o tsp-ant.out ant.c ../cities/csv-input.c -lm
mac-omp:
	gcc-12 -fopenmp -o tsp-ant.out ant.c ../cities/csv-input.c -lm
mac-omp-opt:
	gcc-12 -fopenmp -O3 -o tsp-ant.out ant.c ../cities/csv-input.c -lm