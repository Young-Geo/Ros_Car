#include "control.h"
#include "afx.h"
#include "xlog.h"

control_t *     control_init()
{
    int fd = 0;
    control_t *control = NULL;

    xassert(control = (control_t *)xmalloc(sizeof(control_t)));

    xassert((fd = xserial_open(DERIVCE_CHASSIS_NAME)) > 0);

    xassert(xserial_init(fd, 9600, 8, 1, 'N', 0));

    control->fd = fd;

    control->isStart = 1;

    return control;
}


int     control_destory(control_t *control)
{
    xassert(control);
    stop_move(control);
    control->isStart = 0;
    if (control->fd > 0)
        xserial_close(&control->fd);
}

int     control_data_processing(control_t *control)
{
    unsigned char buf[1024] = { 0 };
    int i = 0, count = 0;
    xassert(control);

    //////

    if ((count = xserial_recv(control->fd, (char *)buf, sizeof(buf), 0)) <= 0) {
        xmessage("control not data\n");
        return -1;
    }
    xmessage("rec %d buf %s\nhex:", count, (char *)buf);
    for (i = 0; i < count; ++i)
    {
        printf("%x %d ==\t", 0xff & buf[i], buf[i]);
    }
    xmessage("\n");
    return 0;
}

int     movexyz(control_t *control, double x, double y, double z)
{
    int one, two, three, four;

    xassert(control);

    one = (int)(y - x + (z*A + z*B));
    two = (int)(y + x - (z*A + z*B));
    three = (int)(y - x - (z*A + z*B));
    four = (int)(y + x + (z*A + z*B));

    if (one > 100)
        one = 100;
    if (one < -100)
        one = -100;

    if (two > 100)
        two = 100;
    if (two < -100)
        two = -100;

    if (three > 100)
        three = 100;
    if (three < -100)
        three = -100;

    if (four > 100)
        four = 100;
    if (four < -100)
        four = -100;


    return move(control, one, two, three, four);

}

//
int     move(control_t *control, int wheelone, int wheeltwo, int wheelthree, int wheelfour)
{
    char buf[1024] = { 0 };
    int ret = 0;
    xassert(control);

    if (control->fd <= 0) {
        xerror("motion fd error");
        return -1;
    }

    if ((ret = make_move(wheelone, wheeltwo, wheelthree, wheelfour, buf, sizeof(buf))) <= 0) {
        xerror("move: make move ");
        return -1;
    }

    xmessage("one %d, two %d, three %d, four %d\n", wheelone, wheeltwo, wheelthree, wheelfour);

    if ((ret = xserial_send(control->fd, buf, ret)) > 0) {
        xmessage("send ok control %s  size = %d\n", buf, ret);
        return 0;
    }
    else
        return -1;
}

int     stop_move(control_t *control)
{
    xassert(control);

    return move(control, 0, 0, 0, 0);
}


/*
int     make_move(int wheelone, int wheeltwo, int wheelthree, int wheelfour, char *out_buf, int len)
{
    char buf[512] = { 0 }, temp[124] = { 0 };

    xassert(out_buf);
    xassert(len > 0);

    xstrcat(buf, "KR-");

    if (wheelone > 0)
        xstrcat(buf, "F");
    else
        xstrcat(buf, "B");
    xsprintf(temp, "%d,", wheelone);
    xstrcat(buf, temp);

    if (wheeltwo > 0)
        xstrcat(buf, "F");
    else
        xstrcat(buf, "B");
    xsprintf(temp, "%d,", wheeltwo);
    xstrcat(buf, temp);

    if (wheelthree > 0)
        xstrcat(buf, "F");
    else
        xstrcat(buf, "B");
    xsprintf(temp, "%d,", wheelthree);
    xstrcat(buf, temp);

    if (wheelfour > 0)
        xstrcat(buf, "F");
    else
        xstrcat(buf, "B");
    xsprintf(temp, "%d;", wheelfour);
    xstrcat(buf, temp);

    if (xstrlen(buf) >= len) {
        xerror("in buf  small\n");
        return -1;
    }

    strcpy(out_buf, buf);
    return xstrlen(buf) + 1;
}*/



int     make_move(int wheelone, int wheeltwo, int wheelthree, int wheelfour, char *out_buf, int len)
{
    unsigned char buf[512] = { 0 };

    xassert(out_buf);
    xassert(len > 8);

    buf[0] = 0xA5;
    buf[1] = 0x5A;

    buf[2] = wheelone;
    buf[3] = wheeltwo;
    buf[4] = wheelthree;
    buf[5] = wheelfour;

    buf[6] = 0xD5;
    buf[7] = 0x5D;

    xmemcpy(out_buf, (char *)buf, 8);
    return 8;
}
