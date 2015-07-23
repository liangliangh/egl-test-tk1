
all:
	g++ main.cpp -o test -lEGL -lGL -lgbm -lopencv_core -lopencv_highgui -lgomp
clean:
	rm -f test

