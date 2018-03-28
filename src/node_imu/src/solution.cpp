#include "solution.h"

/*
JY901_t jy901;

void CopeSerialData(char ucData[],unsigned short usLength)
{
    static unsigned char chrTemp[2000];
    static unsigned char ucRxCnt = 0;
    static unsigned short usRxLength = 0;


    xmemcpy(chrTemp,ucData,usLength);
    usRxLength += usLength;
    while (usRxLength >= 11)
    {
        if (chrTemp[0] != 0x55)
        {
            usRxLength--;
            memcpy(&chrTemp[0],&chrTemp[1],usRxLength);
            continue;
        }
        switch(chrTemp[1])
        {
            case 0x50:	memcpy(&jy901.stcTime,&chrTemp[2],8);break;
            case 0x51:	memcpy(&jy901.stcAcc,&chrTemp[2],8);break;
            case 0x52:	memcpy(&jy901.stcGyro,&chrTemp[2],8);break;
            case 0x53:	memcpy(&jy901.stcAngle,&chrTemp[2],8);break;
            case 0x54:	memcpy(&jy901.stcMag,&chrTemp[2],8);break;
            case 0x55:	memcpy(&jy901.stcDStatus,&chrTemp[2],8);break;
            case 0x56:	memcpy(&jy901.stcPress,&chrTemp[2],8);break;
            case 0x57:	memcpy(&jy901.stcLonLat,&chrTemp[2],8);break;
            case 0x58:	memcpy(&jy901.stcGPSV,&chrTemp[2],8);break;
        }
        usRxLength -= 11;
        memcpy(&chrTemp[0],&chrTemp[11],usRxLength);
    }
}
*/

int     serial_data(int fd, char *buf, int len)
{
    int serFd = 0, ret = 0;
    xassert((serFd = fd) > 0);

    if (!xsk_can_read(serFd, 0, 0)) {
        xdebug("serial_data: serial not data\n");
        return -1;
    }

    if ((ret = xsk_rcv(serFd, buf, len, 0, 0)) < 0) {
        xerror("serial_data: xsk_rcv\n");
        return -1;
    }

    return ret;
}


int     serial_par(JY901_t *jy901, int fd)
{
    char buf[4096] = { 0 }, ret = 0, surplus = 0, i;
    char temdata[11] = { 0 };

    xassert(jy901);
    xassert(fd > 0);

    surplus = ret = serial_data(fd, buf, sizeof(buf));

    for (i = 0; i < ret && surplus >= 11;)
    {
        if (buf[i] != 0x55) {
            ++i;
            --surplus;
            continue;
        }

        switch (buf[i+1])
        {
            case 0x50: xmemcpy(jy901->datatime, &buf[i], 11);break;
            case 0x51: xmemcpy(jy901->acceleration, &buf[i], 11);break;
            case 0x52: xmemcpy(jy901->angularvelocity, &buf[i], 11);break;
            case 0x53: xmemcpy(jy901->angle, &buf[i], 11);break;
            case 0x54: xmemcpy(jy901->magnetic, &buf[i], 11);break;
            case 0x55: xmemcpy(jy901->portstatus, &buf[i], 11);break;
            case 0x56: xmemcpy(jy901->pressureheight, &buf[i], 11);break;
            case 0x57: xmemcpy(jy901->latitudelongitude, &buf[i], 11);break;
            case 0x58: xmemcpy(jy901->groundspeed, &buf[i], 11);break;
            //case 0x59: xmemcpy(jy901->fourelements, &buf[i], 11);break;
        }

        surplus -= 11;
    }
    jy901->isStart = 1;
    return 0;
}
