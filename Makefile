CC = g++
CFLAGS = -std=c++20

main: main.cpp
	$(CC) $(CFLAGS) main.cpp -o main

clean:
	rm main