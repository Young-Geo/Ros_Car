/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#include "ladar.h"
#include "pkt.h"



int     ladar_init(ladar_t *ladar)
{
    int fd = 0;

    xassert(ladar);
    xzero(ladar, sizeof(ladar_t));
    xassert((fd = xserial_open(DERIVCE_NAME)) > 0);

    xassert(xserial_init(fd, 115200, 8, 1, 'N', 0));

    ladar->fd = fd;
    ladar->isStart = 1;

    return 0;
}

int     ladar_start(ladar_t *ladar)
{
    unsigned char buf[2] = {0xA5, 0x60}, *start = NULL;
    char data[1024] = { 0 };

    int ret = 0;

    xassert(ladar);

    if (ladar->fd <= 0) {
        xerror("ladar fd not open");
        return -1;
    }

    if(!xserial_send(ladar->fd, (char *)buf, sizeof(buf))) {
        xerror("xserial_send error");
        return -1;
    }

    if ((ret = xserial_recv(ladar->fd, data, sizeof(data), 1000)) <= 0) {
        xerror("xserial_recv error");
        return -1;
    }

    //par
    if (NULL == (start = pkt_match_head((unsigned char *)data, ret, 0xA5))) {
        xerror("pkt_match_head error");
        return -1;
    }

    if (*(++start) != 0x5A) {
        xerror("pkt_match_head error");
        return -1;
    }

    start += 4;

    if (((*start)&0xC0) != 0x1) {
        xerror("pkt_match_head error");
        return -1;
    }

    return 0;
}


int     ladar_stop(ladar_t *ladar)
{
    unsigned char buf[2] = {0xA5, 0x65};

    xassert(ladar);

    if (ladar->fd <= 0) {
        xerror("ladar fd not open");
        return -1;
    }

    if(!xserial_send(ladar->fd, (char *)buf, sizeof(buf)))
        return -1;
    else
        return 0;
}
