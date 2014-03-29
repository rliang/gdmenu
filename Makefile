TARGET  := gdmenu
SCRIPTS := gdmenu_run
CFLAGS  ?= -Os -pipe -march=native
LDFLAGS ?= -s
CFLAGS  += $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS += $(shell pkg-config --libs gtk+-3.0)
PREFIX  ?= /usr/local
DESTDIR ?= bin

all: $(TARGET)

install: $(TARGET) $(SCRIPTS)
	$(foreach I, $^, install -D $(I) $(PREFIX)/$(DESTDIR)/$(I);)
