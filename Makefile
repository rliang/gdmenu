EXEC=gdmenu
LIBS=gtk+-3.0
CFLAGS=-std=c99 -Os -Wall -Wextra -pedantic -s -pipe
SCRIPTS=${EXEC}_run ${EXEC}_history ${EXEC}_bookmarks

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
EXECS=${EXEC} ${SCRIPTS}

CFLAGS+=`pkg-config --cflags ${LIBS}`
LDFLAGS=`pkg-config --libs ${LIBS}`

PREFIX?=/usr/local
BINDIR?=${PREFIX}/bin


.PHONY: clean install

all: ${EXEC}

${EXEC}: ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o $@

clean:
	rm ${OBJS} ${EXEC}

install: ${EXECS}
	$(foreach i, ${EXECS}, install -Dm 755 ${i} ${DESTDIR}${BINDIR}/${i};)
