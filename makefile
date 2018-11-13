EXE = main 
MAIN = main.cpp

$(EXE):		$(MAIN) lsystem.o turtle.o main.hpp
		g++ -o $(EXE) $(MAIN) turtle.o lsystem.o
		
debug:		$(MAIN) lsystem.hpp lsystem.cpp turtle.hpp turtle.cpp main.hpp
		g++ -Og -ggdb -o $(EXE) $(MAIN) lsystem.cpp turtle.cpp
		
turtle:		turtle.cpp turtle.hpp
		g++ turtle.cpp -c
		
lsystem:	lsystem.hpp lsystem.cpp
		g++ lsystem.cpp -c
		
.PHONY:		clean build
clean:
		rm *.o $(EXE)

build:		$(MAIN) lsystem.o turtle.o main.hpp
		g++ -o $(EXE) $(MAIN) turtle.o lsystem.o
