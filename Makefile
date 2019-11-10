.DEFAULT_GOAL=release
CC=gcc
BINDIR=bin
OBJDIR=obj
SDIR=src
CFLAGS=-Wall -I./$(SDIR) -I/usr/include/glib-2.0/ -I/usr/lib/glib-2.0/include/
OBJECTS=$(OBJDIR)/argument_parsing.o $(OBJDIR)/main.o $(OBJDIR)/stlist.o $(OBJDIR)/gdbus_wrapping.o $(OBJDIR)/storage.o
LIBS=-lglib-2.0 -lgio-2.0 -lm

vpath %.c $(SDIR)
vpath %.o $(OBJDIR)

debug: CFLAGS += -DDEBUG -ggdb
debug: $(BINDIR)/debug/univplayers

release: $(BINDIR)/release/univplayers

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(BINDIR)/release/univplayers: $(OBJECTS) | $(BINDIR)/release
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(BINDIR)/debug/univplayers: $(OBJECTS) | $(BINDIR)/debug
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(OBJDIR):
	install -d $@
$(BINDIR)/release:
	install -d $@
$(BINDIR)/debug:
	install -d $@

.SUFFIXES: .o .c .h
.DEFAULT: $(.DEFAULT_GOAL)

.PHONY: clean
.PHONY: clean-all
.PHONY: release
.PHONY: debug
.PHONY: all
.PHONY: default

clean:
	rm -rf $(OBJDIR)

clean-all:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
