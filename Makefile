.DEFAULT_GOAL=release
CC=gcc
OUTDIR=bin
OBJDIR=obj
SDIR=src
CFLAGS=-Wall -I./$(SDIR) -I/usr/include/glib-2.0/ -I/usr/lib/glib-2.0/include/
INCLUDES=$(SDIR)/argument_parsing.h
OBJECTS=$(OBJDIR)/argument_parsing.o $(OBJDIR)/main.o $(OBJDIR)/stlist.o $(OBJDIR)/gdbus_wrapping.o $(OBJDIR)/storage.o
LIBS=-lglib-2.0 -lgio-2.0 -lm

debug: CFLAGS += -DDEBUG -ggdb
debug: $(OUTDIR)/debug/univplayers

release: $(OUTDIR)/release/univplayers

$(OBJDIR)/%.o: $(SDIR)/%.c $(INCLUDES)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(OUTDIR)/release/univplayers: $(OBJECTS) $(INCLUDES)
	install -d $(OUTDIR)/release
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(OUTDIR)/debug/univplayers: $(OBJECTS) $(INCLUDES)
	install -d $(OUTDIR)/debug
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

default: release

.PHONY: clean
.PHONY: release
.PHONY: debug
.PHONY: all
.PHONY: default

clean:
	rm -r $(OBJDIR)
