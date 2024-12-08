all:
	clang++ -Wall -Wshadow -ftrapv -Wstrict-overflow -g -O3 -ffast-math -march=native main.cc -o main

# -fsanitize=address,leak,undefined

run:
	./main

clean:
	rm main
