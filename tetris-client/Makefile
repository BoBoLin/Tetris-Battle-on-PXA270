SRC = util.c frame.c shapes.c tetris_client.c
CC = cc
BIN = tetris_client
INSTALLPATH = /usr/local/bin/
CFLAGS = -Wall


tty-tetris : ${SRC}

	@echo "build ${SRC}"
	@echo "CC ${CFLAGS} ${SRC}"
	@${CC} ${CFLAGS} ${SRC} -o ${BIN} -lpthread

clean :

	@echo "cleaning"
	@echo "rm ${BIN}"
	@rm ${BIN}
	@echo "cleaned."

