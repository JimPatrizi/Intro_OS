all:  shubert_fork shubert_thread	server	client

server:
	gcc server.c -o server
client:
	gcc client.c -o client

shubert_fork:
	gcc shubert_pipe.c -o shubert_pipe -lm

shubert_thread:
	gcc shubert_thread.c -o shubert_thread -lm -lpthread


clean:
	rm -f client
	rm -f server
	rm -f shubert_pipe
	rm -f shubert_thread
