all:
	g++ -Wall -ftrapv -Wstrict-overflow -g -O3 -ffast-math -mavx2 main.cc -o main -Wl,--as-needed -Wl,-O1 -Wl,--strip-all

run:
	./main

clean:
	rm main
