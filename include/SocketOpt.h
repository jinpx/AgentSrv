#ifndef _SocketOpt_Headfile_
#define _SocketOpt_Headfile_

#include <Utility.h>
/////////////////////////////////////////////////////////////////////////////////

namespace CSocketOpt
{
	// Create file descriptor for socket i/o operations.
	SOCKET CreateTCPFileDescriptor();
	
	// Disable blocking send/recv calls.
	bool Nonblocking(SOCKET fd);
	
	// Enable blocking send/recv calls.
	bool Blocking(SOCKET fd);

	// Disable nagle buffering algorithm
	bool DisableBuffering(SOCKET fd);

	// Enables nagle buffering algorithm
	bool EnableBuffering(SOCKET fd);

	// Set internal buffer size to socket.
	bool SetRecvBufferSize(SOCKET fd, UINT32 size);

	// Set internal buffer size to socket.
	bool SetSendBufferSize(SOCKET fd, UINT32 size);

	// Set timeout, in seconds
	bool SetTimeout(SOCKET fd, UINT32 timeout);

	// Closes socket completely.
	void CloseSocket(SOCKET fd);

	// Sets SO_REUSEADDR
	void ReuseAddr(SOCKET fd);
};

/////////////////////////////////////////////////////////////////////////////////
#endif
