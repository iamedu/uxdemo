uxtest: main.cc
	g++ -lglut -lGLU -lGL -Iinclude --std=c++11 main.cc util.cc -o uxtest
