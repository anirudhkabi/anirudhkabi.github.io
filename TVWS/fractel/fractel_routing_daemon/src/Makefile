# build bwudp executable when user executes "make"
fractel_routing_daemon: fractel_routing_daemon.o
	$(CXX) $(LDFLAGS) fractel_routing_daemon.o -o fractel_routing_daemon --static

fractel_routing_daemon.o: fractel_routing_daemon.c
	$(CXX) $(CXXFLAGS) -c fractel_routing_daemon.c

# remove object files and executable when user executes "make clean"
clean:
	rm *.o fractel_routing_daemon

