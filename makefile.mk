############# OPERATOR ################
INCLUDE = -I./include
LIBPATH = 
LIBFILE = -lpthread
EXEFILE = AgentServer
OBJFILE = CircuitLock.o SocketOpt.o IniFile.o Session.o TraceService.o main.o

all : clean $(EXEFILE)

clean :
	-rm -f *.o
	-rm -f $(EXEFILE)

$(EXEFILE) : $(OBJFILE)
	g++ -w $(OBJFILE) -o  $(EXEFILE) $(INCLUDE) $(LIBPATH) $(LIBFILE)
	cp  $(EXEFILE) ../bin/
	-rm -f *.o $(EXEFILE)
	#################### Succeed! ###############################
	
.cpp.o:
	g++ -c -g  $(INCLUDE) $*.cpp