main : main.c server.c server.h
	gcc -o main main.c server.c

clean:
	rm main