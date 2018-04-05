#include "imu.h"


imu_t * imu_init()
{
    imu_t *imu = NULL;
    int fd = 0;
    xassert(imu = (imu_t *)xmalloc(sizeof(imu_t)));
    xzero(imu, sizeof(imu_t));
    //xassert((fd = xsk_init_client("192.168.0.103", 9001, 0)) > 0);
    xassert((fd = xserial_open(DERIVCE_IMU_NAME)) > 0);
    xassert(xserial_init(fd, 9600, 8, 1, 'N', 0));

    imu->serial_fd = fd;
    xchain_init(&imu->chain);
    xassert((imu->lists = xlist_init()));
    imu->isStart = 1;
    return imu;
}


int     imu_destory(imu_t *imu)
{
    xassert(imu);
    imu->isStart = 0;
    xlist_clean(&imu->lists);
    xchain_clear(&imu->chain);
    xserial_close(&imu->serial_fd);
}

int  make_imu(imu_t *imu)
{
    xlist *start = NULL;
    char *buf = NULL;
    int i = 0, sum = 0;
    mpu_t *tmpu = NULL;

    xassert(imu);
    xassert(imu->lists);
    //

    tmpu = &imu->mpu;

    for (start = imu->lists; start && start->next; start = start->next)
    {
        buf = (char *)start->value;
        if (!(buf = (char *)start->value))
            continue;

        for (i = 0, sum = 0; i < 10; ++i)
        {
            sum += buf[i];
        }
        if (buf[10] != sum)
            continue;

        switch (buf[1]) {
        case 0x51:
            {
                tmpu->acceleration.x = ((short)(buf[3]  << 8) | buf[2]) / 32768 * (16 * 9.8);
                tmpu->acceleration.y = ((short)(buf[5]  << 8) | buf[4]) / 32768 * (16 * 9.8);
                tmpu->acceleration.z = ((short)(buf[7]  << 8) | buf[6]) / 32768 * (16 * 9.8);
                tmpu->acceleration.t = ((short)(buf[9]  << 8) | buf[8]) / 100;
            }
            break;
        case 0x52:
            {
                tmpu->angularspeed.x = ((short)(buf[3]  << 8) | buf[2]) / 32768 * 2000;
                tmpu->angularspeed.x = ((short)(buf[5]  << 8) | buf[4]) / 32768 * 2000;
                tmpu->angularspeed.x = ((short)(buf[7]  << 8) | buf[6]) / 32768 * 2000;
                tmpu->angularspeed.t = ((short)(buf[9]  << 8) | buf[8]) / 100;
            }
            break;
        case 0x53:
            {
                tmpu->angle.x = ((short)(buf[3]  << 8) | buf[2]) / 32768 * 180;
                tmpu->angle.x = ((short)(buf[5]  << 8) | buf[4]) / 32768 * 180;
                tmpu->angle.x = ((short)(buf[7]  << 8) | buf[6]) / 32768 * 180;
                tmpu->angle.t = ((short)(buf[9]  << 8) | buf[8]) / 100;
            }
            break;
        case 0x54:
            {
                tmpu->magnetic.x = ((short)(buf[3]  << 8) | buf[2]);
                tmpu->magnetic.x = ((short)(buf[5]  << 8) | buf[4]);
                tmpu->magnetic.x = ((short)(buf[7]  << 8) | buf[6]);
                tmpu->magnetic.t = ((short)(buf[9]  << 8) | buf[8]) / 100;
            }
            break;
        default:
            break;
        }
    }
}


int     imu_data_processing(imu_t *imu)
{
    xassert(imu);

    if (!imu->isStart) {
        xdebug("imu not start");
        return -1;
    }

    serial_par(imu->serial_fd, &imu->chain, imu->lists);
    make_imu(imu);
}

/*
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
}*/


int     serial_data(int fd, char *buf, int len)
{
    int serFd = 0, ret = 0;
    xassert((serFd = fd) > 0);


    if ((ret = xserial_recv(serFd, buf, len, 0)) <= 0) {
        xerror("serial_data: xsk_rcv\n");
        return -1;
    }

    return ret;
}


int     serial_par(int fd, xchain *chain, xlist *list)
{
    char buf[4096] = { 0 }, ret = 0, surplus = 0, i;
    //char temdata[11] = { 0 };
    JY901_t jy901;

    xassert(fd > 0);
    xassert(chain);
    xassert(list);

    surplus = ret = serial_data(fd, buf, sizeof(buf));
    xchain_add(chain, (void *)buf, ret);

    while (xchain_size(chain) > IMUPACKSIZE)
    {
        //
        if (!pkt_match_tag(chain, 0x55)) {
            return -1;
        }

        xchain_get(chain, buf, IMUPACKSIZE);
        xchain_delete(chain, IMUPACKSIZE);
        // ying gai ji suan sum
        xlist_add(list, NULL, XLIST_STRING, (char *)xmemdup((void *)buf, IMUPACKSIZE));
        xmessage("tag %x\n", buf[1]);

        switch (buf[1])
        {
            case 0x50: xmemcpy(jy901.datatime, &buf[0], 11);break;
            case 0x51: xmemcpy(jy901.acceleration, &buf[0], 11);break;
            case 0x52: xmemcpy(jy901.angularvelocity, &buf[0], 11);break;
            case 0x53: xmemcpy(jy901.angle, &buf[0], 11);break;
            case 0x54: xmemcpy(jy901.magnetic, &buf[0], 11);break;
            case 0x55: xmemcpy(jy901.portstatus, &buf[0], 11);break;
            case 0x56: xmemcpy(jy901.pressureheight, &buf[0], 11);break;
            case 0x57: xmemcpy(jy901.latitudelongitude, &buf[0], 11);break;
            case 0x58: xmemcpy(jy901.groundspeed, &buf[0], 11);break;
            //case 0x59: xmemcpy(jy901->fourelements, &buf[i], 11);break;
        }
    }

    /*
    for (i = 0; i < ret && surplus >= IMUPACKSIZE;)
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
    }*/


    return 0;
}
