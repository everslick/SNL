-include ../Makefile.config

TARGET  = lib$(PROGRAM).so.$(VERSION)

DEFINES = -DVERSION=\"$(VERSION)\"

LFLAGS += -lrt -shared -lpthread
CFLAGS += -fPIC -I../include -I$(PREFIX)/include -std=gnu99

MAKECMDGOALS ?= debug

ifeq ($(MAKECMDGOALS),debug)
DEFINES += -DDEBUG
CFLAGS  += -g3 -O0
else
CFLAGS += -s
endif

headers = $(wildcard *.h)
sources = $(wildcard *.c)
objects = $(subst .c,.o,$(wildcard *.c))

depend  = .depend

$(depend): Makefile
	$(CC) -MM $(CFLAGS) $(sources) > $@

debug release: $(depend) $(objects)
	$(CC) $(objects) $(LFLAGS) -o ../lib/$(TARGET)
	cd ../lib ; ln -sf $(TARGET) lib$(PROGRAM).so

install:
	mkdir -p $(PREFIX)/include/$(PROGRAM)
	cp -f $(headers) $(PREFIX)/include/$(PROGRAM)
	mkdir -p $(PREFIX)/lib
	cp -af ../lib/$(TARGET) $(PREFIX)/lib
	cd $(PREFIX)/lib ; ln -sf $(TARGET) lib$(PROGRAM).so

uninstall:
	rm -f $(PREFIX)/lib/lib$(PROGRAM).so*
	rm -rf $(PREFIX)/include/$(PROGRAM)

clean:
	rm -f ../lib/lib$(PROGRAM).so* *.o $(depend)

doc:
	PROJECT=$(PROJECT) PROGRAM=$(PROGRAM) VERSION=$(VERSION) doxygen

.c.o:
	$(COMPILE.c) $(DEFINES) $(CFLAGS) -c $< $(OUTPUT_OPTION)

-include $(depend)
