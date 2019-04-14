.SUFFIXES: .tar.gz .c 

override CFLAGS += -Wall -O0 -g 
pawsserver:pawsserver.c 

all: pawsserver pawsserver.tar.gz 

%.tar.gz: DIR=$(subst .tar.gz,,$@)
%.tar.gz: %.c 
	mkdir ./$(DIR)-0.1
	cp $^ Makefile ./$(DIR)-0.1
	tar -cz -f $@ ./$(DIR)-0.1

clean:
	rm pawsserver

