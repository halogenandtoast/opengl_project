SRC = main.cc
OBJ = ${SRC:.cc=.o}

CC = g++
FRAMEWORKS = -framework GLUT -framework OpenGL -lGLEW

all: umog

.cc.o:
	@echo CC $<
	@${CC} -c -ggdb -o $@ $<

umog: ${OBJ}
	@${CC} ${FRAMEWORKS} ${OBJ} -ggdb -o umog

clean:
	@echo cleaning
	@rm -f umog ${OBJ}

.PHONY: all clean
