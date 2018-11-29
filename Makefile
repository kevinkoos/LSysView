EXE = lsysview 
MAIN = lsysview.cpp 

objects = functions.o quat.o gui.o turtle.o lsystem.o 

$(EXE):		$(MAIN) header.h $(objects)
		g++ -o $(EXE) $(MAIN) $(objects) -lGL -lGLU ./libglut.so -lAntTweakBar

functions.o:	functions.cpp header.h
		g++ functions.cpp -c
		
quat.o:		quat.cpp header.h
		g++ quat.cpp -c
		
gui.o:		gui.cpp header.h
		g++ gui.cpp -c
	
turtle.o:	turtle.cpp turtle.hpp
		g++ turtle.cpp -c
		
lsystem.o:	lsystem.hpp lsystem.cpp
		g++ lsystem.cpp -c

# force these targets to always run when given the command
.PHONY: 	clean build debug
clean:
		rm *.o $(EXE)

build:		$(MAIN) functions.cpp gui.cpp quat.cpp header.h turtle lsystem
		g++ -o $(EXE) $(MAIN) $(objects) -lGL -lGLU ./libglut.so -lAntTweakBar

# compile in whole using the cpp's to preserve information
debug:		$(MAIN) functions.cpp gui.cpp quat.cpp header.h turtle.hpp turtle.cpp lsystem.hpp lsystem.cpp
		g++ -Og -ggdb -o $(EXE) $(MAIN) functions.cpp gui.cpp quat.cpp lsystem.cpp turtle.cpp -lGL -lGLU ./libglut.so -lAntTweakBar
