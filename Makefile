.DEFAULT_GOAL=release
CC=gcc
BINDIR=bin
OBJDIR=obj
SDIR=src
CFLAGS=-Wall -I./$(SDIR) -I/usr/include/glib-2.0/ -I/usr/lib/glib-2.0/include/
INCLUDES=$(SDIR)/argument_parsing.h
OBJECTS=$(OBJDIR)/argument_parsing.o $(OBJDIR)/main.o $(OBJDIR)/stlist.o $(OBJDIR)/gdbus_wrapping.o $(OBJDIR)/storage.o
LIBS=-lglib-2.0 -lgio-2.0 -lm

debug: CFLAGS += -DDEBUG -ggdb
debug: $(BINDIR)/debug/univplayers

release: $(BINDIR)/release/univplayers

$(OBJDIR)/%.o: $(SDIR)/%.c $(INCLUDES) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(BINDIR)/release/univplayers: $(OBJECTS) $(INCLUDES) $(BINDIR)/release
	$(CC) -o $@ $(OBJECTS) $(INCLUDES) $(CFLAGS) $(LIBS)

$(BINDIR)/debug/univplayers: $(OBJECTS) $(INCLUDES) $(BINDIR)/debug
	$(CC) -o $@ $(OBJECTS) $(INCLUDES) $(CFLAGS) $(LIBS)

$(OBJDIR):
	install -d $(OBJDIR)
$(BINDIR)/release:
	install -d $(BINDIR)/release
$(BINDIR)/debug:
	install -d $(BINDIR)/debug

default: release

.PHONY: clean
.PHONY: clean_all
.PHONY: release
.PHONY: debug
.PHONY: all
.PHONY: default

clean:
	rm -rf $(OBJDIR)

clean_all:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
