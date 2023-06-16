build:
	g++ -std=c++17 ./src/*.cpp ./src/SDL/*.cpp \
	-g -Wextra -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer \
	-o bunnyblocks

run:
	./bunnyblocks

clean:
	rm bunnyblocks
