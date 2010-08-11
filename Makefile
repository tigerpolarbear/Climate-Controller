
SRCFILES := main.c
OBJFILES := $(SRCFILES:%.c=%.o)
DEPFILES := $(OBJFILES:%.o=%.d)
CLEANFILES := $(CLEANFILES) $(DEPFILES) $(OBJFILES) climatecontroller
LIBS ?= `sdl-config --libs`  $(LUALIB) -lSDL_image -lSDL_mixer
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin

-include config.mk

CFLAGS ?= -O3 -DNDEBUG -g

climatecontroller: $(OBJFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJFILES) $(LIBS)

-include $(DEPFILES)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -MT "$*.d" -c -o $@ $<

# Clean
clean:
	$(RM) $(CLEANFILES)

.PHONY: clean
