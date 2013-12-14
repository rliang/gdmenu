EXEC=gdmenu
LIBS=gtk+-3.0 gmodule-2.0
CFLAGS=-std=c99 -Os -Wall -Wextra -pedantic -s -pipe
SCRIPTS=${EXEC}_run ${EXEC}_history ${EXEC}_bookmarks

RES=$(wildcard *.ui)
RESXML=resources.xml
RESSRC=$(RESXML:.xml=.c)


SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
EXECS=${EXEC} ${SCRIPTS}

CFLAGS+=`pkg-config --cflags ${LIBS}`
LDFLAGS=`pkg-config --libs ${LIBS}`

PREFIX?=/usr/local
BINDIR?=${PREFIX}/bin


.PHONY: resources clean install

all: ${EXEC}

${EXEC}: ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o $@

resources: ${RESSRC}

${RESSRC}: ${RES}
	glib-compile-resources --generate-source ${RESXML} --target=$@

clean:
	rm ${RESSRC} ${OBJS} ${EXEC}

install: ${EXECS}
	$(foreach i, ${EXECS}, install -Dm 755 ${i} ${DESTDIR}${BINDIR}/${i};)
