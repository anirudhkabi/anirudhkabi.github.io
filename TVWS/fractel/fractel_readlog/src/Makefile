# build bwudp executable when user executes "make"
fractel_readlog: fractel_readlog.o
	$(CXX) $(LDFLAGS) fractel_readlog.o -o fractel_readlog --static

fractel_readlog.o: fractel_readlog.c
	$(CXX) $(CXXFLAGS) -c fractel_readlog.c

# remove object files and executable when user executes "make clean"
clean:
	rm *.o fractel_readlog

