all:  openmp posix forkp threadS forkS

openmp:
	gcc q1_openmp.c -o openmp -fopenmp -lm

posix:
	gcc q2_threadp.c -o posix -lm -lpthread

forkp:
	gcc q3_forkp.c -o forkp -lm -lrt

threadS:
	gcc q4_threadS.c -o threadS -lm -lrt -lpthread

forkS:
	gcc q5_forkS.c -o forkS -lm -lrt -lpthread

clean:
	rm -f openmp
	rm -f posix
	rm -f forkp
	rm -f threadS
	rm -f forkS
