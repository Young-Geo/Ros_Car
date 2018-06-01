#include "YClient.h"


CSocket_t*  client_socket_alloc()
{
    int fd = 0;
    CSocket_t *csocket = NULL;

    if ((fd = xsk_init_listen_socket(CSOCKETPORT, CSOCKETLISTENCOUNT, NULL)) <= 0) {
        xerror("create socket error %d\n", fd);
        return NULL;
    }

    xassert(csocket = (CSocket_t *)xmalloc(sizeof(CSocket_t)));

    xchain_init(&csocket->chain);

    csocket->fd = fd;
    return csocket;
}

void       client_socket_init(CSocket_t *socket)
{
    int fd = 0;

    xassert(socket);

    if ((fd = xsk_init_listen_socket(CSOCKETPORT, CSOCKETLISTENCOUNT, NULL)) <= 0) {
        xerror("create socket error %d\n", fd);
        return ;
    }


    xchain_init(&socket->chain);

    xmessage("client_socket_init access listen fd %d\n", fd);

    socket->fd = fd;
}

void       client_socket_destory(CSocket_t *socket)
{
    xassert(socket);

    socket->fd = 0;
    socket->back = NULL;
    xchain_clear(&socket->chain);
    xfree(socket);
}

int        client_socket_setback(CSocket_t *socket, call_back_t back)
{
    xassert(socket);
    xassert(back);

    socket->back = back;
    xmessage("client_socket_setback access \n");
    return 0;
}


static int client_socket_accept(CSocket_t *socket)
{
    int cfd = 0;
    char buf[1024] = { 0 };

    xassert(socket);

    if (socket->fd <= 0) {
        xerror("client_socket_spinOnce fd %d\n", socket->fd);
        return -1;
    }

    xmessage("accept connect ...\n");

    if ((cfd = xsk_accept(socket->fd, buf)) <= 0) {
        xerror("client_socket_spinOnce xsk_accept fd %d %s\n", cfd, buf);
        return -1;
    }

    socket->cfd = cfd;
    xmessage("accept %d access %s\n", cfd, buf);

    return 0;
}

int        client_socket_spinOnce(CSocket_t *socket)
{
    char buf[BUFFER_SIZE] = { 0 };
    int reccount = 0;

    xassert(socket);
    xmessage("client_socket_spinOnce ...\n");

    if ((socket->cfd <= 0) && client_socket_accept(socket)) {
        xdebug("client_socket_spinOnce cfd <= 0\n");
        return -1;
    }

    /*
    if (socket->cfd <= 0 && socket->fd > 0) {
        client_socket_accept(socket);
        return 2;
    }*/


    if (!xsk_can_read(socket->cfd, 0, 0)) {
            xmessage("client_socket_spinOnce : cfd %d no can read\n", socket->cfd);
            return -1;
    }

    //read data


    /*
    if ((reccount = xsk_rcv(socket->cfd, buf, sizeof(buf), 0, 0)) <= 0) {
        xsk_close(&socket->cfd);
        xerror("client_socket_spinOnce  rec data size %d\n", reccount);
        return -1;
    }*/

    if ((reccount = read(socket->cfd, (void *)buf, sizeof(buf))) <= 0) {
        xsk_close(&socket->cfd);
        xerror("client_socket_spinOnce  rec data size %d\n", reccount);
        return -1;
    }

    xchain_add(&socket->chain, (void *)buf, reccount);

    if (!socket->back) {
        xerror("client_socket_spinOnce socket->back is NULL\n");
        return -1;
    }

    while (1)
    {
        if (xchain_size(&socket->chain) < CLIENTPACKSIZE) {
            xdebug("client_socket_spinOnce xchain_size %d\n", xchain_size(&socket->chain));
            break;
        }


        if (!pkt_match_tag(&socket->chain, 0xA1)) {
            xerror("client_socket_spinOnce not find 0xA1\n");
            return -1;
        }

        xchain_get(&socket->chain, (void *)buf, CLIENTPACKSIZE);
        xchain_delete(&socket->chain, CLIENTPACKSIZE);

        for (int i = 0; i < 8; ++i)
        {
            xmessage(" buf i %x \t", buf[i] & 0xff);
        }
        xmessage("\n");

        if ((buf[0]&0xff) == 0xA1 && (buf[1]&0xff) == 0x1A && (buf[6]&0xff) == 0xD1 && (buf[7]&0xff) == 0x1D) {

            short angle = 0, distance = 0;
            angle = (buf[2] | ((buf[3] & 0xff) << 8));
            distance = (buf[4] | ((buf[5] & 0xff) << 8));
            socket->back(socket->cfd, angle, distance);
        }
    }

    return 0;
}
