/*
 * tcp.h
 *
 *  Created on: 2017��3��23��
 *      Author: yuanlifu
 */

#ifndef APP_USER_TCP_H_
#define APP_USER_TCP_H_

void WIFIAPInit();
void WIFIServerMode(void *arg);
void TcpServerListen_PCon(void *arg);
void WIFI_TCP_SendNews(unsigned char *dat,uint16 len);
//void Task_Init();

#endif /* APP_USER_TCP_H_ */
