LINKER_FLAGS = -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer

main: main.cpp
	 g++ $@.cpp -w $(LINKER_FLAGS) -o $@
	./main

