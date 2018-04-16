/*
 *  Created on: 2017-12-26
 *      Author: Young.Geo
 *          QQ: 473763733
 *       Email: anxan524@126.com
 */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "afx.h"
#include "xlog.h"

typedef struct _global_t
{
    int master_version, secondary_version;
} global_t;



//par
////

#define A 165 //16.5cm
#define B 160 //16cm
#define PI			3.1415926535897932384626433832795
#define DIAMETER 60 //60mm
#define MOTIONTIMER 20 //20ms
#define MECANUMPERIMETER (DIAMETER * PI) //麦克纳姆轮周长
#define MAICHONGNUM 22

////

extern global_t global;
////////////////////////////////////////////////////////////////////////////
void global_init(int argc , const char * argv[]) ;

#endif
