CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DOSX
INCFLAGS = -I./glm-0.9.2.7 -I/usr/X11/include -I./include/
LDFLAGS = -framework GLUT -framework OpenGL -L./osxlib/ \
		-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		-lGL -lGLU -lm -lstdc++ -lfreeimage
else
CFLAGS = -g -DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I./glm-0.9.2.7 -I./include/ -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lglut -lGLU -lGL -lX11 -lfreeimage
endif

RM = /bin/rm -f 
all: transform
transform: main.o shaders.o Transform.o readfile.o variables.h readfile.h shaders.h Transform.h Texture.o Texture.h glm.o glm.h
	$(CC) $(CFLAGS) -o transforms shaders.o main.o Transform.o readfile.o Texture.o glm.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp shaders.h Transform.h variables.h Texture.h glm.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
shaders.o: shaders.cpp shaders.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
readfile.o: readfile.cpp readfile.h variables.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c readfile.cpp
Transform.o: Transform.cpp Transform.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp  
Texture.o: Texture.cpp Texture.h Transform.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Texture.cpp
glm.o: glm.cpp glm.h Texture.h Transform.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c glm.cpp
clean: 
	$(RM) *.o transforms *~
