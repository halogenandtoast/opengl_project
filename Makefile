SRC = main.cc
OBJ = ${SRC:.cc=.o}

CC = g++
FRAMEWORKS = -framework GLUT -framework OpenGL

all: umog

.cc.o:
	@echo CC $<
	@${CC} -c -o $@ $<

umog: ${OBJ}
	@${CC} ${FRAMEWORKS} ${OBJ} -o umog

clean:
	@echo cleaning
	@rm -f umog ${OBJ}

.PHONY: all clean
