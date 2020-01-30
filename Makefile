main : main.c
	gcc -pthread -o main main.c

clean :
	rm ./main
