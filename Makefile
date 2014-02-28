uxtest: main.cc util.cc shaders.cc shapes.cc vertex.cc
	g++ -lglut -lGLU -lGL -Iinclude --std=c++11 main.cc util.cc shaders.cc shapes.cc vertex.cc -o uxtest

clean: 
	rm uxtest
