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

////

extern global_t global;
////////////////////////////////////////////////////////////////////////////
void global_init(int argc , const char * argv[]) ;

#endif
