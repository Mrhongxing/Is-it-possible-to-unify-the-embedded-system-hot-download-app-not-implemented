#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include <string.h>
#include <stdio.h>

extern uint16_t recives[1000];
extern GPIO_InitTypeDef GPIOs;
extern int number;
extern int isChanged;
extern uint32_t timer_ms;

int start;
int end;

void USART1_IRQHandler(void){
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET){
		recives[number-1]=USART_ReceiveData(USART1);
		if(recives[number-1]==0){
		}else{
			number++;	
		}
	}
}

void sendString(char *String){
	int j = 0;
	while(String[j]!='\0'){
		USART_SendData(USART1,String[j]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
		j++;
	}
}

int readLong(char *String){
	int i=0;
	int j =0;
	while(String[i]!='\0'){
		j+=1;
		i++;
	}
	return j;
}

int USART1_Init(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USARTs;
	int tryions=1;
	int waitions=0;
	int i =0;
	int O =0;
	int K =0;
	
	start=timer_ms;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_StructInit(&GPIOs);
	GPIOs.GPIO_Pin=GPIO_Pin_9;
	GPIOs.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIOs); 
	GPIOs.GPIO_Pin=GPIO_Pin_10;
	GPIOs.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIOs);
	
	USARTs.USART_BaudRate=115200;
	USARTs.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USARTs.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USARTs.USART_Parity=USART_Parity_No;
	USARTs.USART_StopBits=USART_StopBits_1;
	USARTs.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USARTs);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1,ENABLE);
	
	Delay_ms(100);
	while(tryions){
		if(waitions==0){
			USART_SendData(USART1,0x00);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'A');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'T');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\r');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\n');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\r');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\n');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			waitions=1;
		}
		if(waitions==1){
			i=0;
			while(recives[i]!=0x0000){
				if(recives[i]==0x004F){
					O=1;
				}else if(recives[i]==0x004B){
					K=1;
				}
				i++;
				if(O==1&&K==1){
					tryions=0;
					memset(recives, 0, sizeof(recives));
					number=1;
				}
			}
		}
		end=timer_ms; 
		if(end-start>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 0;
		}
	}
	return 1;
}

int USART1_openHotspot(void){
	int tryions=1;
	int tryConnect=1;
	int waitions=0;
	int i =0;
	int r =0;
	int e =0;
	int a =0;
	int d =0;
	int y=0;
	
	start=timer_ms;
	
	while(tryions){
		if(waitions==0){
			sendString("AT+CWMODE=3\r\n\r\n");
			waitions=1;
		}
		if(waitions==1){
			i=0;
			while(recives[i]!=0x0000){
				if(recives[i]=='O'){
					r=1;
				}else if(recives[i]=='K'){
					e=1;
				}
				i++;
				if(r==1&&e==1){
					tryions=0;
					memset(recives, 0, sizeof(recives));
					number=1;
				}
			}
		}
		end=timer_ms;
		if(end-start>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 2;
		}
	}
	waitions=0;
	r=0;
	e=0;
	while(tryConnect){
		if(waitions==0){
			sendString("AT+CWSAP=\"Device_AP\",\"12345678\",6,3\r\n\r\n");
			waitions=1;
		}
		if(waitions==1){
			i=0;
			while(recives[i]!=0x0000){
				if(recives[i]=='O'){
					r=1;
				}else if(recives[i]=='K'){
					e=1;
				}
				i++;
				if(r==1&&e==1){
					tryConnect=0;
					memset(recives, 0, sizeof(recives));
					number=1;
				}
			}
		}
		start=timer_ms;
		if(start-end>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 0;
		}
	}
	waitions=0;
	tryConnect=1;
	r=0;
	e=0;
	while(tryConnect){
		if(waitions==0){
			sendString("AT+CIPAP=\"192.168.4.1\"\r\n\r\n");
			waitions=1;
		}
		if(waitions==1){
			i=0;
			while(recives[i]!=0x0000){
				if(recives[i]=='O'){
					r=1;
				}else if(recives[i]=='K'){
					e=1;
				}
				i++;
				if(r==1&&e==1){
					tryConnect=0;
					memset(recives, 0, sizeof(recives));
					number=1;
				}
			}
		}
		end=timer_ms;
		if(end-start>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 3;
		}
	}
	waitions=0;
	tryConnect=1;
	r=0;
	e=0;
	while(tryConnect){
		if(waitions==0){
			sendString("AT+CIPMUX=1\r\n\r\n");
			waitions=1;
		}
		if(waitions==1){
			i=0;
			while(recives[i]!=0x0000){
				if(recives[i]=='O'){
					r=1;
				}else if(recives[i]=='K'){
					e=1;
				}
				i++;
				if(r==1&&e==1){
					tryConnect=0;
					memset(recives, 0, sizeof(recives));
					number=1;
				}
			}
		}
		start=timer_ms;
		if(start-end>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 4;
		}
	}
	waitions=0;
	tryConnect=1;
	r=0;
	e=0;
	while(tryConnect){
		if(waitions==0){
			sendString("AT+CIPSERVER=1,80\r\n\r\n");
			waitions=1;
		}
		if(waitions==1){
			i=0;
			while(recives[i]!=0x0000){
				if(recives[i]=='O'){
					r=1;
				}else if(recives[i]=='K'){
					e=1;
				}
				i++;
				if(r==1&&e==1){
					tryConnect=0;
					memset(recives, 0, sizeof(recives));
					number=1;
				}
			}
		}
		end=timer_ms;
		if(end-start>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 3;
		}
	}
	return 1;
}

int ReadConnectIPD(void){
	int i =0;
	int I=0;
	int P=0;
	int D=0;
	int temp=0;
	int plus=0;
	int from=0;
	int dian=0;
	while(recives[i]!=0x0000){
		if(recives[i]=='+'){
			plus=1;
			temp=i;
		}else if(recives[i]=='I'&&i-temp==1&&recives[temp]=='+'){
			I=1;
			temp=i;
		}else if(recives[i]=='P'&&i-temp==1&&recives[temp]=='I'){
			P=1;
			temp=i;
		}else if(recives[i]=='D'&&i-temp==1&&recives[temp]=='P'){
			D=1;
			temp=i;
		}else if(recives[i]==','&&i-temp==1&&recives[temp]=='D'){
			dian=1;
			temp=i;
		}else if(recives[i]=='0'&&i-temp==1&&recives[temp]==','){
			from=2;
			temp=i;
		}else if(recives[i]=='1'&&i-temp==1&&recives[temp]==','){
			from=1;
			temp=i;
		}
		if(plus==1&&I==1&&P==1&&D==1&&from==2&&dian==1){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 1;
		}else if(plus==1&&I==1&&P==1&&D==1&&from==1&&dian==1){
			return 2;
		}
		i++;
	}
	
	return 0;
}

void readHTML(char *name,char *password){
	int i =0;
	int I=0;
	int P=0;
	int D=0;
	int temp=0;
	int plus=0;
	int dian=0;
	int s=0;
	int have=0;
	while(recives[i]!=0x0000){
		if(recives[i]=='s'&&have==0){
			plus=1;
			temp=i;
			have+=1;
		}else if(recives[i]=='s'&&i-temp==1&&recives[temp]=='s'){
			I=1;
			temp=i;
		}else if(recives[i]=='i'&&i-temp==1&&recives[temp]=='s'){
			P=1;
			temp=i;
		}else if(recives[i]=='d'&&i-temp==1&&recives[temp]=='i'){
			D=1;
			temp=i;
		}else if(recives[i]=='='&&i-temp==1&&recives[temp]=='d'){
			dian=1;
			temp=i;
		}
		if(plus==1&&I==1&&P==1&&D==1&&dian==1){
			i+=1;
			while(recives[i]!='&'){
				name[s]=recives[i];
				s++;
				i++;
			}
			name[s]='\0';
			break;
		}
		i++;
	}
	plus=0;
	I=0;
	P=0;
	D=0;
	i=0;
	s=0;
	while(recives[i]!=0x0000){
		if(recives[i]=='p'){
			plus=1;
			temp=i;
		}else if(recives[i]=='w'&&i-temp==1&&recives[temp]=='p'){
			I=1;
			temp=i;
		}else if(recives[i]=='d'&&i-temp==1&&recives[temp]=='w'){
			P=1;
			temp=i;
		}else if(recives[i]=='='&&i-temp==1&&recives[temp]=='d'){
			D=1;
			temp=i;
		}
		if(plus==1&&I==1&&P==1&&D==1){
			i+=1;
			while(recives[i]!=' '){
				password[s]=recives[i];
				s++;
				i++;
			}
			memset(recives, 0, sizeof(recives));
			number=1;
			password[s]='\0';
			break;
		}
		i++;
	}
}

int USART1_sendHTML(char *HTML){
	int tryions=1;
	int waitions=0;
	int i=0;
	int stringLong=readLong(HTML)-2;
	int O=0;
	int K=0;
	char str[100];
	sprintf(str,"AT+CIPSEND=0,%d\r\n\r\n",stringLong);
	start=timer_ms;
	while(tryions){
		if(waitions==0){
			sendString(str);
			waitions=1;
		}
		if(waitions==1){
			i=0;
				while(recives[i]!=0x0000){
					if(recives[i]=='>'){
						tryions=0;
						memset(recives, 0, sizeof(recives));
						number=1;
					}
					i++;
			}
		}
		end=timer_ms;
		if(end-start>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 0;
		}
	}
	tryions=1;
	waitions=0;
	while(tryions){
		if(waitions==0){
			sendString(HTML);
			waitions=1;
		}
		if(waitions==1){
			i=0;
				while(recives[i]!=0x0000){
					if(recives[i]=='O'){
						O=1;
					}else if(recives[i]=='K'){
						K=1;
					}
					if(O==1&&K==1){
						tryions=0;
						memset(recives, 0, sizeof(recives));
						number=1;
					}
					i++;
			}
		}
		start=timer_ms;
		if(start-end>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 2;
		}
	}
	return 1;
}

int USART1_connectToWifi(char *wifiname,char *password){
	int tryions=1;
	int tryConnect=1;
	int waitions=0;
	int i =0;
	int O =0;
	int K =0;
	int j=0;
	
	start=timer_ms;
	
	while(tryions){
		if(waitions==0){
			USART_SendData(USART1,0x00);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'A');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'T');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'+');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'C');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'W');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'M');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'O');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'D');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'E');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'=');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'1');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\r');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\n');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\r');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\n');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			waitions=1;
		}
		if(waitions==1){
			i=0;
			while(recives[i]!=0x0000){
				if(recives[i]==0x004F){
					O=1;
				}else if(recives[i]==0x004B){
					K=1;
				}
				i++;
				if(O==1&&K==1){
					tryions=0;
					memset(recives, 0, sizeof(recives));
					number=1;
				}
			}
		}
		end=timer_ms;
		if(end-start>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 2;
		}
	}
	waitions=0;
	while(tryConnect){
		if(waitions==0){
			USART_SendData(USART1,0x00);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'A');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'T');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'+');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'C');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'W');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'J');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'A');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'P');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'=');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'"');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			while(wifiname[j]!='\0'){
				USART_SendData(USART1,wifiname[j]);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
				j++;
			}
			j=0;
			USART_SendData(USART1,'"');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,',');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'"');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			while(wifiname[j]!='\0'){
				USART_SendData(USART1,password[j]);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
				j++;
			}
			j=0;
			USART_SendData(USART1,'"');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\r');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\n');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\r');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\n');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			waitions=1;
		}
		if(waitions==1){
			i=0;
			while(recives[i]!=0x0000){
				if(recives[i]==0x004F){
					O=1;
				}else if(recives[i]==0x004B){
					K=1;
				}
				i++;
				if(O==1&&K==1){
					tryConnect=0;
					memset(recives, 0, sizeof(recives));
					number=1;
				}
			}
		}
		start=timer_ms;
		if(start-end>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 0;
		}
	}
	return 1;
	}


	
int USART1_connectToServe(char *serveIP){
	int tryions=1;
	int waitions=0;
	int i =0;
	int O =0;
	int K =0;
	int j=0;
	
	start=timer_ms;
	
	while(tryions){
		if(waitions==0){
			USART_SendData(USART1,0x00);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			sendString("AT+CIPSTART=");
			USART_SendData(USART1,'"');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'T');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'C');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'P');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'"');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,',');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'"');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			while(serveIP[j]!='\0'){
				USART_SendData(USART1,serveIP[j]);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
				j++;
			}
			j=0;
			USART_SendData(USART1,'"');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			sendString(",8081");
			USART_SendData(USART1,'\r');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\n');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\r');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			USART_SendData(USART1,'\n');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){};
			waitions=1;
		}
		if(waitions==1){
			i=0;
			while(recives[i]!=0x0000){
				if(recives[i]==0x004F){
					O=1;
				}else if(recives[i]==0x004B){
					K=1;
				}
				i++;
				if(O==1&&K==1){
					tryions=0;
					memset(recives, 0, sizeof(recives));
					number=1;
				}
			}
		}
		end=timer_ms;
		if(end-start>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 0;
		}
	}
	return 1;
}

int USART1_HTTP(int mode,char *content){
	int tryions=1;
	int waitions=0;
	int i =0;
	int K =0;
	int P=0;
	if(mode==1){
	}else if(mode==2){
		end=timer_ms;
		while(tryions){
			if(waitions==0){
				sendString("AT+CIPSEND=43\r\n\r\n");
				waitions=1;
			}
			if(waitions==1){
				i=0;
				while(recives[i]!=0x0000){
					if(recives[i]=='>'){
						tryions=0;
						memset(recives, 0, sizeof(recives));
						number=1;
					}
					i++;
				}
			}
			start=timer_ms;
			if(start-end>=3){
				memset(recives, 0, sizeof(recives));
				number=1;
				return 0;
			}
		}
		tryions=1;
		waitions=0;
		while(tryions){
			if(waitions==0){
				sendString("POST /test HTTP/1.1\r\nHost: 192.168.1.18\r\n\r\n\r\n");
				waitions=1;
			}
			if(waitions==1){
				i=0;
				while(recives[i]!=0x0000){
					if(recives[i]==0x004F){
						P=1;
					}else if(recives[i]==0x004B){
						K=1;
					}
					i++;
					if(P==1&&K==1){
						tryions=0;
						//memset(recives, 0, sizeof(recives));
						number=1;
					}
				}
			}
		}
		end=timer_ms;
		if(end-start>=3){
			memset(recives, 0, sizeof(recives));
			number=1;
			return 0;
		}
	}
	return 1;
}

