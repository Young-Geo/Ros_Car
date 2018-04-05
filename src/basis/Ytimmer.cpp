/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#include "Ytimmer.h"


static int is_run_back = 0;
void    call_back_timmer(int sig)
{
    is_run_back = 1;
}


timmer_t* timmer_init(call_back_timmer_t back, void *arg, int ms)
{
    timmer_t *timmer = NULL;
    struct itimerval ms_gap ;

    xassert(timmer = (timmer_t *)xmalloc(sizeof(timmer_t)));


    timmer->arg = arg;
    timmer->sig = SIGALRM;
    timmer->call_back = back;

    signal(SIGALRM,  call_back_timmer);

    ms_gap.it_value.tv_sec = 1 ;//1秒之后开始启动定时器
    ms_gap.it_value.tv_usec = 0 ;
    ms_gap.it_interval.tv_sec = ms / 1000 ;//每间隔ms毫秒执行一次定时器
    ms_gap.it_interval.tv_usec = (ms % 1000) * 1000 ;
    setitimer(ITIMER_REAL, &ms_gap, NULL) ;//设定定时器

    return timmer;
}


int       timmer_spinOnce(timmer_t *timmer)
{
    xassert(timmer);

    if (is_run_back && timmer->call_back) {
        is_run_back = 0;
        timmer->call_back(timmer->arg);
    }
    return 0;
}


int       timmer_destory(timmer_t *timmer)
{
    xassert(timmer);
    return 0;
}
