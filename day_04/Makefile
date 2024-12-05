all:
	g++ -Wall -ftrapv -Wstrict-overflow -g -O0 -ffast-math -march=native main.cc -o main -fsanitize=address,leak,undefined

run:
	./main

clean:
	rm main
