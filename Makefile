CC=gcc
OUTDIR=bin
OBJDIR=obj
SDIR=src
CFLAGS=-Wall -ggdb -I./$(SDIR) -I/usr/include/dbus-1.0/ -I/usr/lib/dbus-1.0/include/ -I/usr/include/glib-2.0/ -I/usr/include/glib-2.0/gio -I/usr/lib/glib-2.0/include/
INCLUDES=$(SDIR)/argument_parsing.h
OBJECTS=$(OBJDIR)/argument_parsing.o $(OBJDIR)/main.o $(OBJDIR)/stlist.o $(OBJDIR)/gdbus_wrapping.o $(OBJDIR)/storage.o
LIBS=-lglib-2.0 -lgio-2.0 -lm

$(OBJDIR)/%.o: $(SDIR)/%.c $(INCLUDES)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

$(OUTDIR)/univplayer: $(OBJECTS) $(INCLUDES)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

default: $(OUTDIR)/univplayer

.PHONY: clean

clean:
	rm -r $(OBJDIR)
