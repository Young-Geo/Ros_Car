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
    xassert(control);

    //////
    return 0;
}

int     movexyz(control_t *control, int x, int y, int z)
{
    int one, two, three, four;

    xassert(control);

    one = y - x + (z*A + z*B);
    two = y + x - (z*A + z*B);
    three = y - x - (z*A + z*B);
    four = y + x + (z*A + z*B);

    return move(control, one, two, three, four);
}

//
int     move(control_t *control, int wheelone, int wheeltwo, int wheelthree, int wheelfour)
{
    char buf[1024] = { 0 };
    xassert(control);

    if (control->fd <= 0) {
        xerror("motion fd error");
        return -1;
    }

    if (make_move(wheelone, wheeltwo, wheelthree, wheelfour, buf, sizeof(buf))) {
        xerror("move: make move ");
        return -1;
    }

    if (xserial_send(control->fd, buf, strlen(buf)))
        return 0;
    else
        return -1;
}

int     stop_move(control_t *control)
{
    xassert(control);

    return move(control, 0, 0, 0, 0);
}

int     make_move(int wheelone, int wheeltwo, int wheelthree, int wheelfour, char *out_buf, int len)
{
    char buf[512] = { 0 }, temp[124] = { 0 };

    xassert(out_buf);
    xassert(len <= 0);

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
    return 0;
}
