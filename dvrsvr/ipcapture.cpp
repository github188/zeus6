
#include "dvr.h"

#define IPCAMERAPORT 15115

ipeagle32_capture::ipeagle32_capture( int channel )
: capture(channel)
{
    config dvrconfig(dvrconfigfile);
    char cameraid[16] ;
    
    m_sockfd = -1 ;
    m_streamfd=-1 ;
    m_state = 0 ;
    m_streamthreadid = 0 ;
    m_type=CAP_TYPE_HIKIP;
    m_ipchannel = 0;
    m_updcounter = 0;
    
    sprintf(cameraid, "camera%d", m_channel+1 );
    
    m_ip = dvrconfig.getvalue( cameraid, "ip" );
    if( m_ip.length()==0 ) {
        return ; 
       // don't enable this channel without valid ip address
    }
    m_port = dvrconfig.getvalueint( cameraid, "port");
    if( m_port==0 ) {
        m_port = IPCAMERAPORT ;
    }
    m_ipchannel = dvrconfig.getvalueint( cameraid, "ipchannel");
    m_enable = m_attr.Enable ;
}

#if 1
void ipeagle32_capture::streamthread()
{
    struct cap_frame capframe ;
    struct cap_frame_header hdframe ;
    int i;
    int frametype ;
    int timeout ;
    capframe.channel=m_channel ;
    m_streamfd=0 ;
    while( m_state ) {

        if( m_streamfd<=0 ) {
          //  printf("net connect:\n");
            m_streamfd = net_connect (m_ip.getstring(), m_port) ;
            if( m_streamfd > 0 ) {
             //   printf("net connect succeed\n");
                if( dvr_openlive (m_streamfd, m_ipchannel, &m_hd264)<=0 ) {
                    closesocket( m_streamfd ) ;
                    m_streamfd=0 ;
                    printf("open live failed\n");
                } else{
                  printf("open live succeed\n");
                }
            } else{
               printf("net connect failed\n");
           }
        }
        if( m_streamfd<=0 ) {
            sleep(1);
            continue ;
        }

        // recevie frames
        i = net_recvok(m_streamfd, 100000) ;
        if( i<0 ) {
            break; 
        }
        else if ( i>0 ) {
         //   printf("ready to receive frame size\n"); 
            if( net_recv (m_streamfd, &hdframe, sizeof(struct cap_frame_header))>0 ) {
                capframe.frametype=hdframe.frametype;
                capframe.framesize=hdframe.framesize;
                capframe.timestamp=hdframe.timestamp;          
                capframe.framedata=(char *)mem_alloc (capframe.framesize) ;
                if(capframe.framedata){
			if( net_recv( m_streamfd, capframe.framedata, hdframe.framesize )<=0 ) {
                                printf("receive data failed\n");
				break;
			}
			onframe( &capframe );
			mem_free( capframe.framedata );	
                } else{
                   printf("memory allocation failed\n");
                }	
            }
            else {
                break;           // error or shutdown
            }
        }
        else {
            if( ++timeout> 20 ) {
                closesocket( m_streamfd );
                m_streamfd = 0 ;
            }
        }
    }//while( m_state )
    if( m_streamfd>0 ) {
        closesocket(m_streamfd);
        m_streamfd = 0 ;
    }
}
#endif

static void * ipeagle32_thread(void *param)
{
    ((ipeagle32_capture *)param) ->streamthread();
    return NULL ;
}

int ipeagle32_capture::connect()
{
    struct  DvrChannel_attr attr;
    struct dvrtime dvrt ;
    char * tzenv ;
    
    m_sockfd = net_connect (m_ip.getstring(), m_port) ;
    if( m_sockfd<0 ) {
        dvr_log("net_connect failed");
        return 0;
    }
    // sync time and time zone
    tzenv = getenv("TZ");
    if( tzenv && strlen(tzenv)>0 ) {
        dvr_settimezone(m_sockfd, tzenv) ;
    }
    
    time_utctime( &dvrt );
    dvr_adjtime(m_sockfd, &dvrt) ;
    
    // set ip cam attr
    attr = m_attr ;
    attr.Enable = 1 ;
    attr.RecMode = -1;					// No recording
    
    dvr_setchannelsetup (m_sockfd, m_ipchannel, &attr) ;
    
 //   updateOSD();
    
    return 1 ;
}

void ipeagle32_capture::start()
{
    if( m_enable ) {
        stop();
        if( connect() ) {
            m_state=1 ;
            pthread_create(&m_streamthreadid, NULL, ipeagle32_thread, this);
        } else{
           dvr_log("can't connect slave");
        }
    }		
}

void ipeagle32_capture::stop()
{
    m_state = 0;
    if( m_sockfd>0 ) {
        closesocket( m_sockfd ) ;
    }
    m_sockfd = -1 ;
    if( m_streamthreadid ) {
        pthread_join(m_streamthreadid, NULL);
    }
    m_streamthreadid = 0 ;
}

void ipeagle32_capture::setosd( struct hik_osd_type * posd )
{
    if( m_sockfd>0 && m_enable ) {
        dvr_sethikosd(m_sockfd, m_ipchannel, posd);
    }
}

// periodic called (0.125s)
/*
void ipeagle32_capture::update(int updosd)
{
    struct dvrtime dvrt ;
    int chstate ;
    int mot ;
    
    if( !m_enable ) {
        return ;
    }
    
    m_updcounter++ ;
    
    if( m_state==0 ) {
        start();
    }
    
    if( m_sockfd<=0 ) {
        if( m_updcounter%80 == 0 && m_streamfd>0 ) {
            // re-connect
            connect();
        }
        m_signal = 0;
        return ;
    }
    
    if( m_updcounter%2400 == m_ipchannel*1200 ) {
        // sync ip camera time
        time_utctime( &dvrt );
        if( dvr_adjtime(m_sockfd, &dvrt) == 0 ) {	// faile?
            closesocket( m_sockfd );
            m_sockfd = -1 ;
        }
    }
    else if( m_updcounter%9 == 0 ) {
        chstate=dvr_getchstate (m_sockfd, m_ipchannel) ;
        if( chstate == -1 ) {				// failed?
            closesocket( m_sockfd );
            m_sockfd = -1 ;
            m_signal = 0 ;
        }
        else {
            m_signal = chstate & 1 ;
            mot = (chstate&2)!=0 ;
            if( mot!=m_motion ) {
                m_motion= mot ;
                updosd=1 ;
            }
        }
    }
    if( m_state ) {
        capture::update( updosd );
    }
}
*/
