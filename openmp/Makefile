all:
	#gcc -o output.exe openmp_test.c -fopenmp
	gcc -g -o tsp-ant.exe ant.c ./../cities/csv-input.c -fopenmp -lm

exec:
	gcc -g -o tsp-ant.exe ant.c ./../cities/csv-input.c -lm
	tsp-ant.exe
execomp:
	gcc -g -o tsp-ant.exe ant.c ./../cities/csv-input.c -fopenmp -lm
	tsp-ant.exe
clean:
	rm -rf *.exe