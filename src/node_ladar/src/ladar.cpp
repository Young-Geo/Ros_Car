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

    xassert(xserial_given_init(fd, 128000, 8, 1, 'N', 0));

    ladar->fd = fd;
    ladar->isStart = 0;
    ladar->isSendStart = 0;

    xchain_init(&ladar->chain);
    xassert(ladar->datas = xlist_init());

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

    ladar->isSendStart = 1;

    return 0;
}


int     ladar_stop(ladar_t *ladar)
{
    unsigned char buf[2] = {0xA5, 0x65};
    int ret = 0;

    xassert(ladar);

    if (ladar->fd <= 0) {
        xerror("ladar fd not open");
        return -1;
    }

    ret = xserial_send(ladar->fd, (char *)buf, sizeof(buf));

    xchain_clear(&ladar->chain);
    xlist_clean(&ladar->datas);

    return ret;
}

static int par_data(xchain *chain, xlist *datas)
{
    unsigned char databuf[BUFFER_SIZE] = { 0 }, *start = NULL;
    int data_len = 0;
    unsigned short check = 0;
    point_cloud_data_t data;

    xassert(chain);
    xassert(datas);

    //data_len = XXMIN(sizeof(databuf), xchain_size(databuf));
    //xchain_get(chain, (void *)databuf, data_len);

    /*//par
    if (NULL == (start = pkt_match_short_head(databuf, data_len, 0x55AA))) {
        xerror("pkt_match_head error");
        return -1;
    }*/

    if (pkt_match_short_tag(chain, 0x55AA) <= 0) {
        xerror("pkt_match_head error");
        return -4;
    }

    xchain_get(chain, (void *)databuf, LADARPACKSIZE);
    xchain_delete(chain, LADARPACKSIZE);
    start = databuf;

    check = *(start+8);
    *((unsigned short*)(start+8)) = 0;
    //check yan zheng

    if (pkt_build_short_check_sum(start, LADARPACKSIZE) != check) {
        xerror("check error");
        return -1;
    }

    start+=2;
    data.data_type= *start;

    if (*start == 0x01) {
        ///
        if (1 != (data_len = *(++start))) {
            xerror("0 data len error");
            return -1;
        }
    } else if (*start == 0x00) {
        data_len = *(++start);
    }
    data.data_len = data_len;
    ++start;
    data.start_position = *start;
    start += 2;
    data.end_position = *start;
    start += 4;
    data.data = *start;
    start += 2;

    data.data = (data.data >> 2);//data length jie suan / 4
    data.start_position = ((data.start_position >> 1) >> 6);//   /64
    data.end_position = ((data.end_position >> 1) >> 6);//  /4

    xlist_add(datas, NULL, XLIST_STRING, (char *)xmemdup(&data, sizeof(data)));
    return 0;
}

int checkStart(xchain *chain)
{
    char buf[9] = { 0 };
    xassert(chain);
    //par
    if (!pkt_match_short_tag(chain, 0xA55A)) {
        xerror("checkStart pkt_match_short_tag not find 0xA55A");
        return -1;
    }

    xchain_get(chain, (void *)buf, 7);
    xchain_delete(chain, 7);


    if ((buf[5]&0xC0) != 0x1) {
        xerror("checkStart pkt_match_head 0x1");
        return -1;
    }

    xmessage("start ladar\n");
    return 0;
}

int     ladar_analytic_data(ladar_t *ladar)
{
    unsigned char data[4096] = { 0 }, *start = NULL;
    int ret = 0;

    xassert(ladar);
    xassert(&ladar->chain);

    if (!ladar->isSendStart || ladar->fd <= 0) {
        xerror("ladar_analytic_data  isSendStart || fd error");
        return -1;
    }

    if ((ret = xserial_recv(ladar->fd, (char *)data, sizeof(data), 0)) <= 0) {
        xerror("xserial_recv error");
        return -1;
    }

    xchain_add(&ladar->chain, (void *)data, ret);

    if (ladar->isSendStart && !ladar->isStart) {
        if (!checkStart(&ladar->chain)) ladar->isStart = 1;
        else return -1;
    }

    while (xchain_size(&ladar->chain) > LADARPACKSIZE)
    {
        //
        if (-4 == par_data(&ladar->chain, ladar->datas)) {
            break;
        }
    }

    if (xlist_isempty(ladar->datas)) {
        return 0;
    }

    //ke yi zuo hou xu chu li

    return 0;
}

