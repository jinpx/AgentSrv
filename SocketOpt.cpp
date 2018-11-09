#include "Network.h"
#include "SocketOpt.h"
#include <sys/ioctl.h>

namespace SocketOpt
{
    // Create file descriptor for socket i/o operations.
    SOCKET CreateTCPFileDescriptor() {
        // create a socket for use with overlapped i/o.
        return socket(AF_INET, SOCK_STREAM, 0);
    }

    // Disable blocking send/recv calls.
    bool Nonblocking(SOCKET fd) {
        UINT32 arg = 1;
        return (::ioctl(fd, FIONBIO, &arg) == 0);
    }

    // Disable blocking send/recv calls.
    bool Blocking(SOCKET fd) {
        UINT32 arg = 0;
        return (ioctl(fd, FIONBIO, &arg) == 0);
    }

    // Disable nagle buffering algorithm
    bool DisableBuffering(SOCKET fd) {
        UINT32 arg = 1;
        return (setsockopt(fd, 0x6, 0x1, (const char*)&arg, sizeof(arg)) == 0);
    }

    // Enable nagle buffering algorithm
    bool EnableBuffering(SOCKET fd) {
        UINT32 arg = 0;
        return (setsockopt(fd, 0x6, 0x1, (const char*)&arg, sizeof(arg)) == 0);
    }

    // Set internal buffer size to socket.
    bool SetSendBufferSize(SOCKET fd, UINT32 size) {
        return (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(size)) == 0);
    }

    // Set internal buffer size to socket.
    bool SetRecvBufferSize(SOCKET fd, UINT32 size) {
        return (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size)) == 0);
    }

	// Set internal timeout.
	bool SetTimeout(SOCKET fd, UINT32 timeout) {
		struct timeval to;
		to.tv_sec = timeout;
		to.tv_usec = 0;
		if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&to, (socklen_t)sizeof(to)) != 0) {
			return false;
		}
		return (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, (socklen_t)sizeof(to)) == 0);
	}

    // Closes a socket fully.
    void CloseSocket(SOCKET fd) {
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }

    // Sets reuseaddr
    void ReuseAddr(SOCKET fd) {
        UINT32 option = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, 4);
    }
}