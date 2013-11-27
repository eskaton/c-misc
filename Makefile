VERSIONM= 1
VERSION = 1.0.0
CFLAGS  = -Wall -Wextra -O2
ARFLAGS = -rc
CC      = gcc
AR		  = ar

SRC     = hex_dump.c stack.c list.c olist.c htab.c
HDR     = ${SRC:.c=.h}
OBJ     = ${SRC:.c=.o}
PIC_OBJ = ${SRC:.c=.lo}

STATICLIB=libmisc.a
SHAREDLIB=libmisc.so
SHAREDLIBV=$(SHAREDLIB).$(VERSION)
SHAREDLIBVM=$(SHAREDLIB).$(VERSIONM)

prefix =/usr/local
libdir =${prefix}/lib
includedir =${prefix}/include/misc

all: $(STATICLIB) $(SHAREDLIBV)

.c.o:
	 @echo CC $<
	 @${CC} -c $< ${CFLAGS}

.SUFFIXES: .lo

.c.lo:
	$(CC) -fPIC -c $(CFLAGS) -o $@ $<

$(SHAREDLIBV): ${PIC_OBJ}
	${LD} -shared -fPIC ${LDFLAGS} ${PIC_OBJ} -o $@
	ln -fs $@ $(SHAREDLIB)
	ln -fs $@ $(SHAREDLIBVM)

$(STATICLIB): ${OBJ}
	$(AR) $(ARFLAGS) $@ ${OBJ}

install: $(STATICLIB) $(SHAREDLIBV)
	test -d $(includedir) || mkdir -p $(includedir)
	cp ${HDR} $(includedir)
	chmod 644 $(includedir)/stack.h $(includedir)/hex_dump.h
	test -d $(libdir) || mkdir -p $(libdir)
	cp $(STATICLIB) $(libdir)
	chmod 644 $(libdir)/$(STATICLIB)
	cp $(SHAREDLIBV) $(libdir)
	chmod 755 $(libdir)/$(SHAREDLIBV)
	rm -f $(libdir)/$(SHAREDLIB) $(libdir)/$(SHAREDLIBVM)
	ln -fs $(SHAREDLIBV) $(libdir)/$(SHAREDLIB)
	ln -fs $(SHAREDLIBV) $(libdir)/$(SHAREDLIBVM)
	(ldconfig -m || true) >/dev/null 2>&1

clean:
	@rm -f $(SHAREDLIB) $(SHAREDLIBV) $(SHAREDLIBVM) $(STATICLIB) ${OBJ} ${PIC_OBJ}
