all:
	#gcc -o output.exe openmp_test.c -fopenmp
	gcc  -o tsp-ant.exe ant.c ./../cities/csv-input.c -fopenmp -lm

exec:
	tsp-ant.exe
clean:
	rm -rf *.exe