uxtest: main.cc util.cc shaders.cc shapes.cc vertex.cc texture.cc
	g++ -L. -lShikoba -lSOIL -lglut -lGLU -lGL -Iinclude --std=c++11 main.cc util.cc shaders.cc shapes.cc vertex.cc texture.cc -o uxtest

clean: 
	rm uxtest
