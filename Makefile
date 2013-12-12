EXEC=gdmenu
LIBS=gtk+-3.0 gmodule-2.0
CFLAGS=-std=c99 -Os -Wall -Wextra -pedantic -s -pipe

RES=$(wildcard *.ui)
RESXML=resources.xml
RESSRC=$(RESXML:.xml=.c)


SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

CFLAGS+=`pkg-config --cflags ${LIBS}`
LDFLAGS=`pkg-config --libs ${LIBS}`

PREFIX?=/usr/local
BINDIR?=${PREFIX}/bin


.PHONY: resources clean install

all: ${EXEC}

${EXEC}: resources ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o $@

resources: ${RESSRC}

${RESSRC}: ${RES}
	glib-compile-resources --generate-source ${RESXML} --target=$@

clean:
	rm ${RESSRC} ${OBJS} ${EXEC}

install: ${EXEC}
	install -Dm 755 ${EXEC} ${DESTDIR}${BINDIR}/${EXEC}
	install -Dm 755 ${EXEC}_run ${DESTDIR}${BINDIR}/${EXEC}_run

