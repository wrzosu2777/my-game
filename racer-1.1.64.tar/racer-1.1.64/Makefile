
# DESTDIR is for choosing /usr or /usr/local

DESTDIR=/usr/local

# DATADIR=games  install to  /usr/share/games/racer
# DATADIR=.      install to  /usr/share/racer 
DATADIR=.


all:
	cd src && make

clean:
	cd src && make clean

distclean:
	cd src && make clean

install:
	mkdir -p '$(DESTDIR)/share/$(DATADIR)/racer/data'
	mkdir -p '$(DESTDIR)/share/$(DATADIR)/racer/gfx'
	mkdir -p '$(DESTDIR)/share/$(DATADIR)/racer/maps'
	mkdir -p '$(DESTDIR)/share/applications' '$(DESTDIR)/bin'
	mkdir -p '$(DESTDIR)/share/pixmaps'
	mkdir -p '$(DESTDIR)/share/pixmaps'
	mkdir -p '$(DESTDIR)/share/man/man6'
	install -D -m755 src/racer '$(DESTDIR)/bin'
	install -D -m644 data/data/* '$(DESTDIR)/share/$(DATADIR)/racer/data'
	install -D -m644 data/gfx/* '$(DESTDIR)/share/$(DATADIR)/racer/gfx'
	install -D -m644 data/maps/* '$(DESTDIR)/share/$(DATADIR)/racer/maps'
	install -D -m644 misc/racer.desktop '$(DESTDIR)/share/applications'
	install -D -m644 misc/racer.xpm     '$(DESTDIR)/share/pixmaps'
	install -D -m644 misc/racer.6.gz    '$(DESTDIR)/share/man/man6'

uninstall:
	rm -rf '$(DESTDIR)/share/$(DATADIR)/racer'
	rm -f  '$(DESTDIR)/bin/racer'
	rm -f  '$(DESTDIR)/share/applications/racer.desktop'
	rm -f  '$(DESTDIR)/share/pixmaps/racer.xpm'
	rm -f  '$(DESTDIR)/share/man/man6/racer.6.gz'

