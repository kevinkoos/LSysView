EXE = lsysview 
MAIN = lsysview.cpp 

$(EXE):		$(MAIN) header.h functions quat gui turtle lsystem
		g++ -o $(EXE) $(MAIN) functions.o gui.o quat.o turtle.o lsystem.o -lGL -lGLU ./libglut.so -lAntTweakBar

functions:	functions.cpp header.h
		g++ functions.cpp -c
		
quat:		quat.cpp header.h
		g++ quat.cpp -c
		
gui:		gui.cpp header.h
		g++ gui.cpp -c
	
turtle:		turtle.cpp turtle.hpp
		g++ turtle.cpp -c
		
lsystem:	lsystem.hpp lsystem.cpp
		g++ lsystem.cpp -c

# force these targets to always run when given the command
.PHONY: 	clean build debug
clean:
		rm *.o $(EXE)

build:		$(MAIN) functions.cpp gui.cpp quat.cpp header.h turtle lsystem
		g++ -o $(EXE) $(MAIN) functions.o gui.o quat.o turtle.o lsystem.o -lGL -lGLU ./libglut.so -lAntTweakBar

debug:		$(MAIN) functions.cpp gui.cpp quat.cpp header.h turtle lsystem
		g++ -Og -ggdb -o $(EXE) $(MAIN) functions.cpp gui.cpp quat.cpp lsystem.cpp turtle.cpp -lGL -lGLU ./libglut.so -lAntTweakBar
