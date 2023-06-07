build:
	g++ ./src/*.cpp -g -Wextra -lSDL2 -o bunnyblocks 

run:
	./bunnyblocks

clean:
	rm bunnyblocks