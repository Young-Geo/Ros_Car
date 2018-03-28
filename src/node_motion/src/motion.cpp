#include "motion.h"
#include "afx.h"
#include "xlog.h"

int     motion_init(motion_t *motion)
{
    int fd = 0;

    xassert(motion);

    xassert((fd = xserial_open(DERIVCE_NAME)) > 0);

    xassert(xserial_init(fd, 115200, 8, 1, 'N', 0));

    motion->fd = fd;

    motion->isStart = 1;

    return 0;
}


int     movexyz(motion_t *motion, int x, int y, int z)
{
    int one, two, three, four;

    xassert(motion);

    one = y - x + (z*A + z*B);
    two = y + x - (z*A + z*B);
    three = y - x - (z*A + z*B);
    four = y + x + (z*A + z*B);

    return move(motion, one, two, three, four);
}

//
int     move(motion_t *motion, int wheelone, int wheeltwo, int wheelthree, int wheelfour)
{
    char buf[1024] = { 0 };
    xassert(motion);

    if (motion->fd <= 0) {
        xerror("motion fd error");
        return -1;
    }

    if (make_move(wheelone, wheeltwo, wheelthree, wheelfour, buf, sizeof(buf))) {
        xerror("move: make move ");
        return -1;
    }

    if (xserial_send(motion->fd, buf, strlen(buf)))
        return 0;
    else
        return -1;
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
