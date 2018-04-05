/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */
#ifndef __YTIMMER_H__
#define __YTIMMER_H__

#include "afx.h"
#include "xlog.h"

typedef int     (*call_back_timmer_t)(void *);

typedef struct _timmer_t
{
    void *arg;
    call_back_timmer_t call_back;
    int sig;
} timmer_t __attribute__ ((packed));

timmer_t* timmer_init(call_back_timmer_t back, void *arg);
int       timmer_spinOnce(timmer_t*);
int       timmer_destory(timmer_t*);


#endif // __YTIMMER_H__
