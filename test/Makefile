-include ../Makefile.config

TARGETS = server client

DEFINES = -DVERSION=\"$(VERSION)\"

LFLAGS += -L../lib -L$(PREFIX)/lib -lsnl
LFLAGS += -L- -Wl,-rpath=$(PWD)/../lib
LFLAGS += -L- -Wl,-rpath=$(PWD)/lib
CFLAGS += -I../include -I$(PREFIX)/include -std=gnu99 -g3 -O0

sources = $(wildcard *.c)
objects = $(subst .c,.o,$(sources))

depend  = .depend

$(depend): Makefile
	$(CC) -MM $(CFLAGS) $(sources) > $@

debug: $(depend) $(objects)
	for TRG in $(TARGETS) ; do $(CC) $$TRG.o $(LFLAGS) -o ../bin/$$TRG ; done

release install uninstall doc:

clean:
	cd ../bin ; rm -f $(TARGETS)
	rm -f *.o $(depend)

.c.o:
	$(COMPILE.c) $(DEFINES) $(CFLAGS) -c $< $(OUTPUT_OPTION)

-include $(depend)
