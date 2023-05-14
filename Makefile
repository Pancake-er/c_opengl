CC=gcc
CFLAGS=-I include -I lib/include -o example.out
# for windows replace above line with: CFLAGS=-I include -I lib/include -o example.exe
CLIBS=-lglfw -lGL -lm -lX11 -lpthread -lXrandr -ldl
# for windows replace above line with: CLIBS=lib/src/libglfw3.a -lgdi32 -lopengl32 -lkernel32 -luser32 -pthread

compile: example/main.c
	$(CC) $(CFLAGS) example/main.c src/shader.c src/texture_atlas.c src/mem_util.c src/render.c src/matrix4f.c lib/src/glad.c $(CLIBS)