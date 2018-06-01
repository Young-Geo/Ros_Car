#ifndef __YCLIENT_H__
#define __YCLIENT_H__

#include "xsocket.h"
#include "xchain.h"
#include "pkt.h"

#define CSOCKETPORT 9001
#define CSOCKETLISTENCOUNT 4
#define CLIENTPACKSIZE 8

typedef int (*call_back_t)(int, short angle, short distance);

typedef struct _CSocket_t
{
    int fd, cfd;
    xchain chain;
    call_back_t back;
} CSocket_t;

CSocket_t* client_socket_alloc();
void       client_socket_init(CSocket_t *);
void       client_socket_destory(CSocket_t *);

int        client_socket_setback(CSocket_t *, call_back_t);

int        client_socket_spinOnce(CSocket_t *);



#endif // __YCLIENT_H__
