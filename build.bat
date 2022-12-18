gcc -o game ^
src/main.c ^
src/shader.c ^
src/texture.c ^
src/mem_util.c ^
src/render.c ^
src/matrix4f.c ^
lib/src/libglfw3.a lib/src/glad.c ^
-I include ^
-I lib/include ^
-lgdi32 -lopengl32 -lkernel32 -luser32 -pthread