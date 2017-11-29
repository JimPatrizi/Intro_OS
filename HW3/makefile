all:  simpleshell parallel shared

simpleshell:
	gcc simple-shell.c -o shell

parallel:
	gcc parallel_fork.c -o parallel_fork

shared:
	gcc shared_memory.c -o shared -lrt



clean:
	rm -f shell
	rm -f parallel_fork
	rm -f shared
