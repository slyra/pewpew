LIBS=-lgc -lGLEW -lglut -lGL -lGLU -lm
EXTRA_FLAGS=-Wall -Wextra -Wuninitialized -std=c++0x
PROG=pewpew
all: ${PROG}
game_object.o: game_object.cpp game_object.h
	gcc -c game_object.cpp ${EXTRA_FLAGS}
vector.o: vector.h vector.cpp
	g++ -c vector.cpp -lm ${EXTRA_FLAGS}
ships.o: ships.h ships.cpp	
	g++ ${LIBS} -c ships.cpp ${EXTRA_FLAGS}
drawables.o: drawables.cpp drawables.h
	g++ ${LIBS} -c drawables.cpp ${EXTRA_FLAGS}
peripherals.o:	peripherals.cpp peripherals.h
	g++ ${LIBS} -c peripherals.cpp -o peripherals.o ${EXTRA_FLAGS}
level.o:  level.cpp level.h
	g++ ${LIBS} -c 	level.cpp ${EXTRA_FLAGS}

OBJECTS=game_object.o ships.o vector.o drawables.o level.o
${PROG}:	${PROG}.cpp ${OBJECTS}
	g++ ${LIBS}  ${PROG}.cpp ${OBJECTS} -o ${PROG} ${EXTRA_FLAGS}
clean:
	rm *.o ${PROG}.tar



tar:
	tar -czvf ${PROG}.tar *
