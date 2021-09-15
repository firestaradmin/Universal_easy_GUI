#ifndef __ESP8266EX_H_
#define __ESP8266EX_H_
#include "sys.h"

#define ESP8266_REC_LEN  			200  	//�����������ֽ��� 200



#define ESP8266_AT_SET_STATION_MODE		(u8 *)"AT+CWMODE=1\r\n"
#define ESP8266_AT_RST					(u8 *)"AT+RST\r\n"
#define ESP8266_AT_CONNECT_WIFI			(u8 *)"AT+CWJAP=\"iu\",\"lizhienya\"\r\n"
#define ESP8266_AT_QUERY_IP				(u8 *)"AT+CIFSR\r\n"
#define ESP8266_AT_CONNECT_TCP			(u8 *)"AT+CIPSTART=\"TCP\",\"120.55.45.151\",8888 \r\n"

#define ESP8266_AT_OPEN_SERIAL_MODE		(u8 *)"AT+CIPMODE=1\r\n"
#define ESP8266_AT_BEGIN_TRANS			(u8 *)"AT+CIPSEND\r\n"
#define ESP8266_AT_OVER_TRANS			(u8 *)"+++"
#define ESP8266_AT_CLOSE_TCP			(u8 *)"AT+CIPCLOSE\r\n"

#define ESP8266_AT_TEST_TCP				(u8 *)"\"{\"opreation\":\"close\",\"Note\":\"˧������\"}\""			//�ɹ�����[OK]


extern u8 ESP8266_RX_BUF[ESP8266_REC_LEN];	
extern u16 ESP8266_RX_STA ;       //����״̬���	
extern u16 ESP8266_RX_BUF_P;		//RXBUF ���
extern u8 ESP8266_RX_timeout_10ms_counter;	//������ɳ�ʱ
extern u8 ESP8266_AT_timeout_10ms_counter;
extern u8 ESP8266_AT_wait_flag ;

void ESP8266EX_EN(u8 EN);
void ESP8266EX_RST(void);
void ESP8266EX_Init(void);
void ESP8266_SEND_AT(u8 *str);
void usart3_init(u32 bound);
void ESP8266_wait_AT_RX(void);
void ESP8266_AT_CHOICE(u8 index);



ErrorStatus ESP8266_IF_WIFI_CONNECT(void);
ErrorStatus ESP8266_IF_OK(u8* s, u8 len);
void ESP8266_AT_edit_json_item(char* buf, char *opreation, char *Note);
ErrorStatus ESP8266_AT_AT(void);
ErrorStatus ESP8266_try_send_item(void);
void ESP8266_SHOW_AT(void);
#endif


/*
������ 9600
1�� ���� wifi ģʽ��
AT+CWMODE=3 //����Ϊ softAP+station ����ģʽ
2�� ������Ч
AT+RST
��Ӧ��OK
6 6 /  13  Espressif Systems  June 27, 2014
3�� ����·��
AT+CWJAP="ssid","password" // ����·�ɵ�ssid �� password
��Ӧ��OK
4�� ��ѯ�豸IP
AT+CIFSR
��Ӧ��192.168.3.106 //�����豸�� IP ��ַ
6�� �豸���ӷ�����
AT+CIPSTART="TCP","120.55.45.151",8888 //����Э�顢������ IP���˿ں�
7�� ����͸��ģʽ
AT+CIPMODE=1
��Ӧ��OK
8�� ��ʼ͸��
AT+CIPSEND
��Ӧ�� > // �Ӵ�ʱ��ʼ������������ַ���͸������������
9)
"{"opreation":"close","Note":"˧������"}"
��Ӧ��[OK]
10�� ����͸��
+++
11) �Ͽ�TCP
AT+CIPCLOSE
return��
	OK
	Unlink


*/


