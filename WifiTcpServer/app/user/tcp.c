#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "mem.h"
#include "espconn.h"
#include "driver/uart.h"
#include "tcp.h"
typedef enum
{
  teClient,
  teServer
}teType;
typedef struct
{
    BOOL linkEn;
  BOOL teToff;
    uint8_t linkId;
    teType teType;
    uint8_t repeaTime;
    uint8_t changType;
    uint8 remoteIp[4];
    int32_t remotePort;
    struct espconn *pCon;
}linkConType;

typedef struct
{
  BOOL linkEn;
  BOOL teToff;
  uint8_t linkId;
  teType teType;
  uint8_t repeaTime;
  struct espconn *pCon;
} espConnectionType;

linkConType pLink;
espConnectionType user_contype;
static struct espconn *pTcpServer;
//os_event_t    procTaskQueue[procTaskQueueLen];
bool Connect_break;
bool Send_Flag;
/*
 * 函数名:void Wifi_AP_Init()
 * 功能wifi_ap初始化
 */
void WIFIAPInit()
{
    struct softap_config apConfig;
    /***************************模式设置************************************/
         if(wifi_set_opmode(SOFTAP_MODE)){          //  设置为AP模式

         }else{

         }
    /***************************名字设通道置************************************/
          os_bzero(&apConfig, sizeof(struct softap_config));
          wifi_softap_get_config(&apConfig);
          apConfig.ssid_len=20;                      //设置ssid长度
          os_strcpy(apConfig.ssid,"thingsController");         //设置ssid名字
          os_strcpy(apConfig.password,"12345678");  //设置密码
          apConfig.authmode =3;                     //设置加密模式
          ETS_UART_INTR_DISABLE();                  //关闭串口中断
          wifi_softap_set_config(&apConfig);        //配置
          ETS_UART_INTR_ENABLE();                   //打开串口
}
/*
 *函数名:void TcpServer_Listen_Recv(void *arg, char *pdata, unsigned short len)
 *功能:接收监听函数
 */
void TcpServer_Listen_Recv(void *arg, char *pdata, unsigned short len)
{
    uart0_sendStr(pdata);
}
/*
 * 函数名:void TcpServer_Listen_Recb(void *arg, sint8 errType)
 * 功能:连接监听函数
 */
void TcpServer_Listen_recon_cb(void *arg, sint8 errType)
{
    struct espconn *pespconn = (struct espconn *)arg;
      linkConType *linkTemp = (linkConType *)pespconn->reverse;
      Connect_break=0;
}
/*
 * 函数名:void Tcp_Server_Listen_discon_cb(void *arg)
 * 功能:正常断开时监听函数
 */
void Tcp_Server_Listen_discon_cb(void *arg)
{
      struct espconn *pespconn = (struct espconn *) arg;
      linkConType *linkTemp = (linkConType *) pespconn->reverse;
      Connect_break=0;
}
/*
 * 函数名:void Tcp_Server_Listen_sent_cb(void *arg)
 * 功能:发送成功监听函数
 */
void Tcp_Server_Listen_sent_cb(void *arg)
{
      struct espconn *pespconn = (struct espconn *) arg;
     linkConType *linkTemp = (linkConType *) pespconn->reverse;
     Send_Flag=0;
}
/*
 * 函数名:void TcpServer_Listen_PCon(void *arg)
 * 功能:手机连入AP监听函数
 */
void TcpServerListen_PCon(void *arg)
{
    struct espconn *pespconn = (struct espconn *)arg;
      pLink.teToff = FALSE;
      pLink.linkId = 1;
      pLink.teType = teServer;
      pLink.repeaTime = 0;
      pLink.pCon = pespconn;
      pespconn->reverse = &pLink;
      espconn_regist_recvcb(pespconn, TcpServer_Listen_Recv);                           //注册接收监听函数
      espconn_regist_reconcb(pespconn, TcpServer_Listen_recon_cb);                      //注册连接监听函数
      espconn_regist_disconcb(pespconn, Tcp_Server_Listen_discon_cb);                   //注册正常断开时监听函数
      espconn_regist_sentcb(pespconn, Tcp_Server_Listen_sent_cb);                       //注册发送成功监听函数

}
/*
 * 函数名:void WIFI_Server_MODE()
 * 功能:设置服务器模式
 */
void WIFIServerMode(void *arg)
{
        espconn_tcp_set_max_con(1);                                         //设置TCP连接的最大多少
        pTcpServer = (struct espconn *)os_zalloc(sizeof(struct espconn));
        pTcpServer->type = ESPCONN_TCP;                                     //TCP服务
        pTcpServer->state = ESPCONN_NONE;                                   //状态
        pTcpServer->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
        pTcpServer->proto.tcp->local_port = 8000;                           //断开号
        espconn_regist_connectcb(pTcpServer, TcpServerListen_PCon);
        espconn_accept(pTcpServer);
        espconn_regist_time(pTcpServer, 180, 0);                            //设置超时断开时间 单位s


}
/*
 * 函数名:void WIFI_TCP_SendNews(unsigned char *dat)
 * 功能:像TCP服务器发送消息
 */
void WIFI_TCP_SendNews(unsigned char *dat,uint16 len)
{
    espconn_sent(pLink.pCon,dat,len);
}
/*void SendTask(os_event_t *events)
{
    struct station_info *pstation_info=wifi_softap_get_station_info();
        if((pstation_info!=NULL)&&(Connect_break==1)){
            if(Send_Flag==0){
                Send_Flag=1;
            WIFI_TCP_Send(xReceive,xLen);
            }else{

                uart0_sendStr("bus");
            }
        }else{
            Send_Flag=0;
            uart0_sendStr("NO device");
        }
        xLen=0;
}
void Task_Init()
{
//  uart_init(BIT_RATE_9600,BIT_RATE_9600);
     system_os_task(SendTask, procTaskPrio, procTaskQueue, procTaskQueueLen);
}*/
