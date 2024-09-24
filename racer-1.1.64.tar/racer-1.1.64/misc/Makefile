# Note that this Makefile is not generally used as
# the game comes already with racer.6 created

all:
	pod2man --section=6 racer.pod > racer.6
	rm -f  racer.6.gz	
	gzip racer.6

clean:
	rm -f  racer.6*
