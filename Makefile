-include Makefile.config

TARGETS = src test demo

DIR := $(shell basename `pwd`)

DUMMY := $(shell rm -rf include ;         \
                 mkdir -p include ;       \
                 cd include ;             \
                 ln -sf ../src $(PROGRAM) \
)

MAKECMDGOALS ?= debug

debug release install uninstall:
	for TRG in $(TARGETS) ; do $(MAKE) -C $$TRG $(MAKECMDGOALS) ; done

clean:
	for TRG in $(TARGETS) ; do $(MAKE) -C $$TRG $(MAKECMDGOALS) ; done
	rm -f core core.* */core */core.*
	cd doc && $(MAKE) clean
	rm -rf include

new: clean all

force:

doc: force
	cd doc && $(MAKE) doc

tar:
	$(MAKE) clean
	cd .. ; tar cfvz $(PROGRAM)-$(VERSION).tar.gz $(DIR)
