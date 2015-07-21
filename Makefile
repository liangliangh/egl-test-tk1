
all:
	g++ main.cpp -o test -lEGL -lGL 
clean:
	rm -f test

