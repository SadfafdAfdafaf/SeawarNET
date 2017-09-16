INC_DIR  := -I C:/cext/sdl2/include \
            -I C:/cext/glew/include\
            -I C:/cext/sdlnet  
LIB_DIR  := -L C:/cext/sdl2/lib \
            -L C:/cext/glew/lib \
            -L C:/cext/sdlnet
LIB      := -lglew32 -lSDL2main -lSDL2_net -lSDL2 -lopengl32 -lglu32

client: 
	gcc src/main2.c src/kernel_gl.c src/subsys.c src/client.c src/game.c  $(INC_DIR) $(LIB_DIR) -std=c11 -O2 -g3 -Werror -Wall -pedantic $(LIB) -o client.exe
server: 
	gcc src/main1.c src/kernel_gl.c src/subsys.c src/server.c src/game.c  $(INC_DIR) $(LIB_DIR) -std=c11 -O2 -g3 -Werror -Wall -pedantic $(LIB) -o server.exe