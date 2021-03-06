/*
 file mcu.cpp,
    basic communications to mcu
 */

#include <stdio.h>
#include <sys/mman.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <errno.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>
#include <sys/mount.h>

#include "../cfg.h"

#include "../dvrsvr/genclass.h"
#include "../dvrsvr/cfg.h"
#include "diomap.h"
#include "serial.h"
#include "mcu.h"

// functions from ioprocess.cpp
int dvr_log(const char *fmt, ...);

int mcu_handle = 0 ;
int mcu_baud = 115200 ;
char mcu_dev[100] = "/dev/ttyS3" ;

extern int mcu_oninput( char * inputmsg );

#ifndef MCU_DEBUG
#define MCU_DEBUG 1
#endif

// debug
#ifdef MCU_DEBUG
void mcu_debug_out1( char * msg, char * head )
{
	int b ;
	printf( head );
	for( b=0; b<*msg; b++) {
		printf(" %02x", (int)*((unsigned char *)msg+b) );
	}
}

void debug_out( char * msg )
{
	int w ;
    static int dbgout=0 ;
	if( dbgout <= 0 )
		dbgout = open("/var/dvr/dbgout", O_WRONLY|O_NONBLOCK) ;
	if( dbgout>0 ) {
		w = write( dbgout, msg, strlen(msg) );
		if( w<=0 ) {
			close( dbgout );
			dbgout = 0 ;
		}
	}
}

void mcu_debug_out( const char * msg, const char * head ) 
{
	char lbuf[512] ;
	char * p = lbuf ;
	int n ;
	int b ;
	n = sprintf( p, "%s", head );
	p+=n ;
	for( b=0; b<*msg; b++) {
		n = sprintf( p, " %02x", (int)*((unsigned char *)msg+b) );
		p+=n ;
	}
	sprintf( p, "\n" );
	debug_out( lbuf );
}

#else
void mcu_debug_out( char * msg, char * head ) 
{
}
#endif


// Check if data ready to read on serial port
//     return 0: no data
//            1: yes
int mcu_dataready(int usdelay, int * usremain)
{
    if( mcu_handle<=0 ) {
        return 0;
    }
    if( serial_dataready( mcu_handle, usdelay, usremain ) ) {
		return 1 ;
    }
    return 0;
}

int mcu_read(char * sbuf, size_t bufsize, int wait, int interval)
{
    size_t nread=0 ;
    while( bufsize>0 && mcu_dataready(wait) ) {
		int r = serial_read( mcu_handle, sbuf, bufsize );
		if( r>0 ) {
			bufsize-=r ;
			sbuf+=r ;
			nread+=r ;
			wait = interval ;
		}
		else 
			break;
    }
    return nread ;
}

// read one byte
static int mcu_read1( char * rbuf)
{
	return serial_read( mcu_handle, rbuf, 1 );
}

int mcu_write(void * buf, int bufsize)
{
    if( mcu_handle>0 ) {
        return write( mcu_handle, buf, bufsize);
    }
    return 0;
}

// clear receiving buffer
void mcu_clear(int delay)
{
    int i;
    char buf[100] ;
    for(i=0;i<10000;i++) {
        if( mcu_read( buf, sizeof(buf), 0, 0 )==0 ) {
            break ;
        }
    }
}

unsigned char checksum( unsigned char * data, int datalen )
{
    unsigned char ck=0;
    while( datalen-->0 ) {
        ck+=*data++;
    }
    return ck ;
}

// check data check sum
// return 0: checksum correct
char mcu_checksum( char * data )
{
    if( *data<6 || *data>120 ) {
        return (char)-1;
    }
    return (char)checksum((unsigned char *)data, *data);
}

// calculate check sum of data
static void mcu_calchecksum( char * data )
{
    char ck = 0 ;
    char i = *data ;

    while( --i>0 ) {
        ck-=*data++ ;
    }
    *data = ck ;
}

// send constructed message to mcu
// return 0 : failed
//        >0 : success
static int mcu_send( char * msg )
{
    mcu_calchecksum(msg );
    
#ifdef MCU_DEBUG
	mcu_debug_out( msg, "send: ");
#endif
    
    return mcu_write(msg, (int)(*msg));
}

// send command to mcu with variable argument
int mcu_sendcmd_target_data( int target, int cmd, int datalen, char * data )
{
    char mcu_buf[datalen+10] ;
    
    // validate command len
    if( datalen<0 || datalen>MCU_MAX_MSGSIZE-6 ) { 	// wrong command
        return 0 ;
    }

    mcu_buf[0] = (char)(datalen+6) ;
    mcu_buf[1] = (char)target ;
    mcu_buf[2] = (char)ID_HOST ;
    mcu_buf[3] = (char)cmd ;
    mcu_buf[4] = (char)2 ;				// this is a request
    
    if( datalen>0 )
		memcpy( mcu_buf+5, data, datalen );

    return mcu_send(mcu_buf) ;
}

// send command to mcu with variable argument
static int mcu_sendcmd_va( int target, int cmd, int datalen, va_list va )
{
	char data[datalen+1] ;
	for( int i=0 ; i<datalen ; i++ ) {
        data[i] = (char) va_arg( va, int );
    }
	return mcu_sendcmd_target_data( target, cmd, datalen, data);
}

// send command to mcu without waiting for responds
int mcu_sendcmd(int cmd, int datalen, ...)
{
	int r ;
    va_list v ;
    va_start( v, datalen ) ;
    r = mcu_sendcmd_va( ID_MCU, cmd, datalen, v ) ;
    va_end(v);
    return r ;
}

// send command to mcu without waiting for responds
int mcu_sendcmd_target(int target, int cmd, int datalen, ...)
{
	int r;
    va_list v ;
    va_start( v, datalen ) ;
    r = mcu_sendcmd_va( target, cmd, datalen, v ) ;
    va_end(v);
    return r;
    
}

// receive one data package from mcu, with static buffer
// return : received msg
//          NULL : failed or timeout
static char * mcu_recvmsg( int rdelay )
{
	static int  recvsiz = 0 ;
	static char recvbuf[MCU_MAX_MSGSIZE+10] ;

	int err = 0 ;
	while( mcu_dataready( rdelay, &rdelay) ) {
		mcu_read1( recvbuf+recvsiz ) ;
		recvsiz++ ;
		if( recvsiz>5 ) {
			if( recvsiz >= (int) recvbuf[0] ) {
				recvsiz = 0;

#ifdef MCU_DEBUG
			mcu_debug_out( recvbuf, "recv: ");
#endif
				if( recvbuf[1]==ID_HOST &&           	// correct target
					(recvbuf[4]==2 ||recvbuf[4]==3 ) &&	// correct send/recv
					mcu_checksum( recvbuf ) == 0 )   // correct checksum
				{
					return recvbuf ;
				}
				else {
					err = 1 ;
					break ;
				}
			}
            if( strncmp(recvbuf, "Enter", 5 )==0 ) {
                // MCU reboots, why?
                dvr_log("MCU power down!!!");
                sync();sync();sync();
                err = 1 ;
				recvsiz = 0;
                break ;
            }
		}
		else {
			int n=(int) recvbuf[0] ;
			if( n<5 || n>MCU_MAX_MSGSIZE ) {
				// some thing wrong
				err = 1 ;
				recvsiz = 0;
				break ;
			}
		}
	}
	if( err ) {
		mcu_clear(100000);
	}
	return NULL ;
}

// receive one data package from mcu, with static buffer
// return : received msg
//          NULL : failed
/*
static char * mcu_recvmsg_x( int usdelay )
{
    if( mcu_read(recvbuf, 1, usdelay ) ) {
        int n=(int) recvbuf[0] ;
        if( n>=5 && n<MCU_MAX_MSGSIZE-2 ) {
            n=mcu_read(recvbuf+1, n-1, usdelay, usdelay )+1 ;

#ifdef MCU_DEBUG
			mcu_debug_out( recvbuf, "recv: ");
#endif
            
            if( n==(int)recvbuf[0] &&            // correct size
                recvbuf[1]==ID_HOST &&           // correct target
                (recvbuf[4]==2 ||recvbuf[4]==3 ) &&	// correct send/recv
                mcu_checksum( recvbuf ) == 0 )   // correct checksum
            {
                return recvbuf ;
            }
            if( strncmp(recvbuf, "Enter", 5 )==0 ) {
                // MCU reboots, why?
                dvr_log("MCU power down!!!");
                sync();sync();sync();
            }
        }
    }
    mcu_clear(100000);
    return NULL ;
}
*/
	
// to receive a message from mcu
// return : received msg
//          NULL : failed
static char * mcu_recv( int usdelay, int * usremain)
{
	char * rmsg ;
    int recvsize ;
    while( mcu_dataready(usdelay, &usdelay) ) {
        if( (rmsg = mcu_recvmsg(usdelay))!=NULL ) {
            if( rmsg[4]=='\x02' ) {             // this is an input message ?
				mcu_oninput( rmsg );
            }
            else {
                if( usremain ) {
                    * usremain=usdelay ;
                }
                return rmsg ;                  // it could be a responding message
            }
        }
        else {
            break;
        }
    }
    if( usremain ) {
        * usremain=usdelay ;
    }
    return NULL ;
}

// wait for response from mcu
// return responds buffer
static char * mcu_waitrsp(int target, int cmd, int delay=MCU_CMD_DELAY)
{
    while( delay>=0 ) {                          	// wait until delay time out
		char * rsp = mcu_recv(delay, &delay ) ;
        if( rsp ) {    
			if( rsp[2]==(char)target &&         // resp from my target (MCU)
                rsp[4]==3 )                     // it is a responds
            {
	            if( rsp[3]==(char)cmd )         // correct resp for my CMD
				{
					return rsp ;
				}
				else if( rsp[3]==(char)0xff && 	// possibly not-supported command
					rsp[5] == cmd ) 			// not supported command
				{
					return (char *)-1 ;
				}
			}
            // ignor all other messages and continue wait for correct responds
        }	  
        else {
            break;
        }
    }
    return NULL ;		// timeout error
}

static int mcu_errors ;

// Send command to mcu and check responds
// return
//       response buffer
//       NULL if failed
char * mcu_cmd_target_data(int target, int cmd, int datalen, char * data )
{
    // validate command length
    if( datalen<0 || datalen>MCU_MAX_MSGSIZE-6 ) { 	// wrong command
        return NULL ;
    }

    int retry=3 ;
    while( retry-->0 ) {
        if( mcu_sendcmd_target_data( target, cmd, datalen, data ) ) {
			char * rsp = mcu_waitrsp(target, cmd) ;		// wait rsp from MCU
            if( rsp ) {
				if( rsp == (char *)-1 ) {
#ifdef MCU_DEBUG
        printf("MCU cmd 0x%02x not supported\n", cmd );
#endif
					rsp = NULL ;
				}
				mcu_errors=0 ;
				return rsp ;
            }
        }

#ifdef MCU_DEBUG
        printf("MCU no response, retry!!! - target:%d, cmd: 0x%02x\n", target, cmd );
#endif
    }
    
	if( ++mcu_errors>10 ) {
		sleep(1);
		mcu_restart();      // restart mcu port
		mcu_errors=0 ;
	}
	return NULL;
}

// Send command to mcu and check responds
// return
//       response buffer
//       NULL if failed
static char * mcu_cmd_target_va(int target, int cmd, int datalen, va_list va )
{
    // validate command length
    if( datalen<0 || datalen>MCU_MAX_MSGSIZE-6 ) { 	// wrong command
        return NULL ;
    }
    
    char data[datalen] ;
	for( int i=0 ; i<datalen ; i++ ) {
        data[i] = (char) va_arg( va, int );
    }
    return mcu_cmd_target_data(target, cmd, datalen, data );
}

// Send command to target mcu and check responds
// return
//       response buffer
//       NULL if failed
char * mcu_cmd_target(int target, int cmd, int datalen, ...)
{
    va_list v ;
    va_start( v, datalen );
    char * rsp = mcu_cmd_target_va( target, cmd, datalen, v );
    va_end( v );
    return rsp ;
}

// Send command to mcu and check responds
// return
//       response buffer
//       NULL if failed
char * mcu_cmd(int cmd, int datalen, ...)
{
    va_list v ;
    va_start( v, datalen );
    char * rsp = mcu_cmd_target_va( ID_MCU, cmd, datalen, v );
    va_end( v );
    return rsp ;
}

// check mcu input
// parameter
//      wait - micro-seconds waiting
// return
//		number of input message received.
//      0: timeout (no message)
int mcu_input(int usdelay)
{
    int res = 0 ;
    char * mcu_msg ;
    //while( mcu_dataready(usdelay, &usdelay) ) {
    while( (mcu_msg = mcu_recvmsg(usdelay))!=NULL ) {
		if( mcu_msg[4]=='\x02' ) {             // is it an input message ? ignor other message ( a delayed response )
			mcu_oninput( mcu_msg );
			res++;
		}
		// ignore other messages
		usdelay = 1000 ;
    }
    return res ;
}

// response to mcu
//    rmsg : original received input from MCU
void mcu_response(char * rmsg, int datalen, ... )
{
    int i ;
    va_list v ;
    char mcu_buf[datalen+10] ;
    mcu_buf[0] = datalen+6 ;
    mcu_buf[1] = rmsg[2] ;
    mcu_buf[2] = rmsg[1] ;         // back to sender
    mcu_buf[3] = rmsg[3] ;         // response to the same command
    mcu_buf[4] = 3 ;               // this is a response
    va_start( v, datalen ) ;
    for( i=0 ; i<datalen ; i++ ) {
        mcu_buf[5+i] = (char) va_arg( v, int );
    }
    va_end(v);
    mcu_send( mcu_buf );
}

// restart mcu port, some times serial port die
void  mcu_restart()
{
   if( mcu_handle > 0 ) {
        close( mcu_handle );
        mcu_handle = 0 ;
    }
    mcu_handle = serial_open( mcu_dev, mcu_baud );
    
printf( "Open serial port :%s :%d\n",     (char *)mcu_dev, mcu_baud );
    
    if( mcu_handle<=0 ) {
        // even no serail port, we still let process run
        dvr_log("Serial port failed!\n");
    }
}

// initialize serial port
void mcu_init( config & dvrconfig )
{
    string v ;
    double dv ;
    int i ;

    // initilize serial port
    v = dvrconfig.getvalue( "io", "serialport");
    if( v.length()>0 ) {
        strcpy( mcu_dev, v );
    }
    mcu_baud = dvrconfig.getvalueint("io", "serialbaudrate");
    if( mcu_baud<2400 || mcu_baud>115200 ) {
        mcu_baud=DEFSERIALBAUD ;
    }

    if( mcu_handle > 0 ) {
        close( mcu_handle );
        mcu_handle = 0 ;
    }
    
    mcu_handle = serial_open( mcu_dev, mcu_baud );

    if( mcu_handle<=0 ) {
        // even no serail port, we still let process run
        dvr_log("Can not open serial port %s.\n", mcu_dev);
    }

}

void mcu_finish()
{
    // close serial port
    if( mcu_handle>0 ) {
        close( mcu_handle );
        mcu_handle=0;
    }
}
