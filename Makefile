build:
	g++ ./src/*.cpp ./src/SDL/*.cpp -g -Wextra -lSDL2 -o bunnyblocks 

run:
	./bunnyblocks

clean:
	rm bunnyblocks