#include "control.h"

control_t *     control_init()
{
    int fd = 0;
    control_t *control = NULL;

    xassert(control = (control_t *)xmalloc(sizeof(control_t)));

    xassert((fd = xserial_open(DERIVCE_CHASSIS_NAME)) > 0);

    xassert(xserial_init(fd, 9600, 8, 1, 'N', 0));

    xassert((control->list = xlist_init()));

    xchain_init(&control->chain);
    pose_init(&control->pose);


    control->fd = fd;

    control->isStart = 1;
    //error

    return control;
err:
    return NULL;
}


int     control_destory(control_t *control)
{
    xassert(control);
    stop_move(control);
    control->isStart = 0;
    if (control->fd > 0)
        xserial_close(&control->fd);

    xlist_clean(&control->list);
    xchain_clear(&control->chain);
    pose_destory(&control->pose);
}


int     make_distence(xchain *chain, xlist *list)
{
    unsigned char buf[MOTION2SHANGPACK] = { 0 };
    int one = 0, two = 0, three = 0, four = 0;
    int onef = 0, twof = 0, threef = 0, fourf = 0;
    int tbuf[4] = { 0 }, t = 0;

    xassert(chain);
    xassert(list);

    while (xchain_size(chain) > MOTION2SHANGPACK)
    {
        //par
        if (!pkt_match_tag(chain, 0xD5))
            continue;

        xchain_get(chain, (void *)buf, MOTION2SHANGPACK);
        xchain_delete(chain, MOTION2SHANGPACK);
        if (!(0xD5 == buf[0] && 0X5D ==buf[1] && 0xA5 == buf[10] && 0x5A == buf[11]))
            continue;

        one = buf[2];
        two = buf[3];
        three = buf[4];
        four = buf[5];

        onef = buf[6];
        if (onef == FU)
            onef = -1;
        twof = buf[7];
        if (twof == FU)
            twof = -1;
        threef = buf[8];
        if (threef == FU)
            threef = -1;
        fourf = buf[9];
        if (fourf == FU)
            fourf = -1;

        tbuf[0] = one * onef;
        tbuf[1] = two * twof;
        tbuf[2] = three * threef;
        tbuf[3] = four * fourf;

        t = tbuf[0];
        tbuf[0] = tbuf[2];
        tbuf[2] = t;

        t = tbuf[1];
        tbuf[1] = tbuf[2];
        tbuf[2] = t;


        xmessage("one %d, two %d, three %d, four %d, f1 %d, f2 %d, f3 %d, f4 %d\n", one, two, three, four, onef, twof, threef, fourf);

        xlist_add(list, NULL, XLIST_STRING, (char *)xmemdup((void *)tbuf, sizeof(tbuf)));
    }
    return 0;
}

int     control_data_processing(control_t *control, ros::Publisher &pub)
{
    unsigned char buf[1024] = { 0 };
    int i = 0, count = 0;
    xassert(control);

    //////

    if ((count = xserial_recv(control->fd, (char *)buf, sizeof(buf), 0)) <= 0) {
        xmessage("control not data\n");
        return -1;
    }
    xmessage("rec %d \n", count);

    xchain_add(&control->chain, (void *)buf, count);
    make_distence(&control->chain, control->list);
    xmessage("a %d \n", count);
    pose_calculation(pub, control->list, &control->pose);
    xmessage("b %d \n", count);

    return 0;
}

/*
void mecanumRun(float xSpeed, float ySpeed, float aSpeed)
{
    float speed1 = ySpeed - xSpeed + aSpeed;
    float speed2 = ySpeed + xSpeed - aSpeed;
    float speed3 = ySpeed - xSpeed - aSpeed;
    float speed4 = ySpeed + xSpeed + aSpeed;

    float max = speed1;
    if (max < speed2)   max = speed2;
    if (max < speed3)   max = speed3;
    if (max < speed4)   max = speed4;

    if (max > maxLinearSpeed)
    {
        speed1 = speed1 / max * maxLinearSpeed;
        speed2 = speed2 / max * maxLinearSpeed;
        speed3 = speed3 / max * maxLinearSpeed;
        speed4 = speed4 / max * maxLinearSpeed;
    }

    setEachMotorSpeed(speed1, speed2, speed3, speed4);
}*/

int     movexyz(control_t *control, double x, double y, double z)
{
/*
    float speed1, speed2, speed3, speed4;

    xassert(control);

    speed1 = (y - x + (z*A + z*B));
    speed2 = (y + x - (z*A + z*B));
    speed3 = (y - x - (z*A + z*B));
    speed4 = (y + x + (z*A + z*B));

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
        four = -100;*/

    float maxLinearSpeed = 70;
    float speed1 = y - x + z;
    float speed2 = y + x - z;
    float speed3 = y - x - z;
    float speed4 = y + x + z;

    float max = speed1;

    xassert(control);

    if (max < speed2)   max = speed2;
    if (max < speed3)   max = speed3;
    if (max < speed4)   max = speed4;

    if (max > maxLinearSpeed)
    {
        speed1 = speed1 / max * maxLinearSpeed;
        speed2 = speed2 / max * maxLinearSpeed;
        speed3 = speed3 / max * maxLinearSpeed;
        speed4 = speed4 / max * maxLinearSpeed;
    }



    return move(control, speed1, speed2, speed3, speed4);

}

//
int     move(control_t *control, int wheelone, int wheeltwo, int wheelthree, int wheelfour)
{
    char buf[1024] = { 0 };
    int ret = 0;
    xassert(control);

    ret = wheeltwo;
    wheeltwo = wheelthree;
    wheelthree = ret;


    ret = wheelthree;
    wheelthree = wheelone;
    wheelone = ret;



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
    xassert(len > SHANG2MOTIONPACK);

    buf[0] = 0xA5;
    buf[1] = 0x5A;

    buf[2] = wheelone;
    buf[3] = wheeltwo;
    buf[4] = wheelthree;
    buf[5] = wheelfour;

    buf[6] = 0xD5;
    buf[7] = 0x5D;

    xmemcpy(out_buf, (char *)buf, SHANG2MOTIONPACK);
    return SHANG2MOTIONPACK;
}
