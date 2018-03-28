

#ifndef _H_XSERIAL_H_
#define _H_XSERIAL_H_

#include "afx.h"
#include "xlog.h"

int  xserial_open ( char * xserial_name ) ;
int  xserial_init ( int fd , int speed , int databit , int stopbit , char parity , int flow_ctrl ) ;
int  xserial_recv ( int fd , char * buf , int buflen , int timeout_minsecond ) ;
int  xserial_send ( int fd , char * buf , int buflen ) ;
void xserial_close ( int * fd ) ;

#endif /* serial.h */





















