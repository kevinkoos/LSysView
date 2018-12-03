EXE = lsysview 
MAIN = src/lsysview.cpp 

objects = src/functions.o src/quat.o src/gui.o src/turtle.o src/lsystem.o 

$(EXE):		$(MAIN) src/header.h $(objects)
		g++ -o $(EXE) $(MAIN) $(objects) -lGL -lGLU -lglut -lAntTweakBar

src/functions.o:	src/functions.cpp src/header.h
		g++ -c src/functions.cpp -o src/functions.o
		
src/quat.o:		src/quat.cpp src/header.h
		g++ -c src/quat.cpp -o src/quat.o
		
src/gui.o:		src/gui.cpp src/header.h
		g++ -c src/gui.cpp -o src/gui.o
	
src/turtle.o:	src/turtle.cpp src/turtle.hpp
		g++ -c src/turtle.cpp -o src/turtle.o
		
src/lsystem.o:	src/lsystem.hpp src/lsystem.cpp
		g++ -c src/lsystem.cpp -o src/lsystem.o

# force these targets to always run when given the command
.PHONY: 	clean build debug
clean:
		rm src/*.o $(EXE)

build:		$(MAIN) src/header.h $(objects)
		g++ -o $(EXE) $(MAIN) $(objects) -lGL -lGLU ./libglut.so -lAntTweakBar

# compile in whole using the cpp's to preserve information
debug:
		g++ -Og -ggdb -o $(EXE) $(MAIN) src/functions.cpp src/gui.cpp src/quat.cpp src/lsystem.cpp src/turtle.cpp -lGL -lGLU -lglut -lAntTweakBar
