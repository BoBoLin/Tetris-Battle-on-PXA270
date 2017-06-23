CROSS_COMPILE = arm-unknown-linux-gnu-gcc
CFLAGS = -gdwarf-2 -O0 -I/home/iodd/install_ncurses/include -static
LDFLAGS=-L/home/iodd/install_ncurses/lib
CC=$(CROSS_COMPILE) $(CFLAGS) $(LDFLAGS) -I $(LINUXDIR)/include
LINUXDIR = /home/iodd/lab02/microtime/pro/devkit/lsp/create-pxa270/linux-2.6.15.3

EXEC = tetris
OBJS = tetris_server.c shapes.c frame.c util.c creator_lib.o /home/iodd/install_ncurses/lib/libncurses.a

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS) -lpthread

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o *.out
