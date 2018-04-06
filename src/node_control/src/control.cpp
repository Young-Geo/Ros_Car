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

int     movexyz(control_t *control, double x, double y, double z)
{
    double one, two, three, four;

    xassert(control);

    one = y - x + (z*A + z*B);
    two = y + x - (z*A + z*B);
    three = y - x - (z*A + z*B);
    four = y + x + (z*A + z*B);

    if (one > 100.0)
        one = 100;
    if (one < -100.0)
        one = -100.0;

    if (two > 100.0)
        two = 100.0;
    if (two < -100.0)
        two = -100.0;

    if (three > 100.0)
        three = 100.0;
    if (three < -100.0)
        three = -100.0;

    if (four > 100.0)
        four = 100.0;
    if (four < -100.0)
        four = -100.0;


    xmessage("owe %lf, two %lf, three %lf, four %lf\n", one, two, three, four);

    return move(control, one, two, three, four);
    //return 0;
}

//
int     move(control_t *control, double wheelone, double wheeltwo, double wheelthree, double wheelfour)
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

    if (xserial_send(control->fd, buf, strlen(buf))) {
        xmessage("send ok control %s\n", buf);
        return 0;
    }
    else
        return -1;
}

int     stop_move(control_t *control)
{
    xassert(control);

    return move(control, 0.0, 0.0, 0.0, 0.0);
}

int     make_move(double wheelone, double wheeltwo, double wheelthree, double wheelfour, char *out_buf, int len)
{
    char buf[512] = { 0 }, temp[124] = { 0 };

    xassert(out_buf);
    xassert(len > 0);

    xstrcat(buf, "KR-");

    if (wheelone > 0)
        xstrcat(buf, "F");
    else
        xstrcat(buf, "B");
    xsprintf(temp, "%d,", (int)fabs(wheelone));
    xstrcat(buf, temp);

    if (wheeltwo > 0)
        xstrcat(buf, "F");
    else
        xstrcat(buf, "B");
    xsprintf(temp, "%d,", (int)fabs(wheeltwo));
    xstrcat(buf, temp);

    if (wheelthree > 0)
        xstrcat(buf, "F");
    else
        xstrcat(buf, "B");
    xsprintf(temp, "%d,", (int)fabs(wheelthree));
    xstrcat(buf, temp);

    if (wheelfour > 0)
        xstrcat(buf, "F");
    else
        xstrcat(buf, "B");
    xsprintf(temp, "%d;", (int)fabs(wheelfour));
    xstrcat(buf, temp);

    if (xstrlen(buf) >= len) {
        xerror("in buf  small\n");
        return -1;
    }

    strcpy(out_buf, buf);
    return 0;
}
