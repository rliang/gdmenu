EXEC=gdmenu
LIBS=gtk+-3.0
CFLAGS=-std=c99 -Os -Wall -Wextra -pedantic -s -pipe
EXECS=${EXEC} ${EXEC}_run ${EXEC}_history ${EXEC}_bookmarks


CFLAGS+=`pkg-config --cflags --libs ${LIBS}`

PREFIX?=/usr/local
BINDIR?=${PREFIX}/bin


.PHONY: clean install

all: ${EXEC}

clean:
	rm ${EXEC}

install: ${EXECS}
	$(foreach i, ${EXECS}, install -Dm 755 ${i} ${DESTDIR}${BINDIR}/${i};)
