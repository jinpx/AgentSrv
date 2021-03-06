############# OPERATOR ################
INCLUDE = -I./include
LIBPATH = 
LIBFILE = -lpthread
EXEFILE = AgentServer
OBJFILE = TraceService.o ThreadLock.o SocketOpt.o IniFile.o \
		  Network.o Session.o ServerSession.o LogonSession.o GameSession.o \
		  UserSession.o FunctionMap.o PacketHandler.o ServerInfo.o \
		  Acceptor.o Connector.o ServerHandler.o \
		  main.o


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