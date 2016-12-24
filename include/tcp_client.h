#ifndef TCP_CLIENT_H__
#define TCP_CLIENT_H__


#include "user_config.h"

//TCP CLIENT Config

#define TCP_SERVER_IPADDR       "192.168.0.8"
#define TCP_SERVER_REMOTE_PORT  (7007)
#define TCP_CLIENT_GREETING     "Hello!This is a tcp client test\n"

#define TCP_CLIENT_KEEP_ALIVE_ENABLE 1

#define TCP_CLIENT_KEEP_ALIVE_IDLE_S (10)
#define TCP_CLIENT_RETRY_INTVL_S (5)
#define TCP_CLIENT_RETRY_CNT     (3)


#define DBG_PRINT(fmt,...)  do{\
        os_printf("[Dbg]");\
        os_printf(fmt,##__VA_ARGS__);\
    }while(0)

#define ERR_PRINT(fmt,...) do{\
        os_printf("[Err] Fun:%s Line:%d ",__FUNCTION__,__LINE__);\
        os_printf(fmt,##__VA_ARGS__);\
    }while(0)
#define DBG_LINES(v) os_printf("------------------%s---------------\n",v)


void tcp_client_start(void);

#endif /* TCP_CLIENT_H__ */
