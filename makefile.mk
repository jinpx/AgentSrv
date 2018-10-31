############# OPERATOR ################
INCLUDE = -I./include
LIBPATH = 
LIBFILE = 
EXEFILE = AgentServer
OBJFILE = TraceService.o main.o

all : clean $(EXEFILE)

clean :
	-rm -f *.o
	-rm -f $(EXEFILE)

$(EXEFILE) : $(OBJFILE)
	g++ -w $(OBJFILE) -o  $(EXEFILE) $(INCLUDE) $(LIBPATH) $(LIBFILE)
	cp  $(EXEFILE) ../bin/
	-rm -f *.o
	#################### Succeed! ###############################
	
.cpp.o:
	g++ -c -g  $(INCLUDE) $*.cpp