#ifndef __HTTP_H
#define __HTTP_H

int USART1_Init(void);
int USART1_connectToWifi(char *wifiname,char *password);
int USART1_connectToServe(char *serveIP);
int USART1_HTTP(int mode,char *content);
int USART1_openHotspot(void);
int ReadConnectIPD(void);
int USART1_sendHTML(char *HTML);
void readHTML(char *name,char *password);

#endif
