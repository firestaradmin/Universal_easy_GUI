/************************************************************************************
*  Copyright (c), 2020, LXG.
*
* FileName		:
* Author		:firestaradmin
* Version		:1.0
* Date			:2020.7.21
* Description	:���ڽ���BIN�ļ�������¼���ⲿ������
* History		:
*
*
*************************************************************************************
֡Э���ʽ:
		Byte0	Byte1	Byte2		Byte3		Byte4		Byte5	��		last but two Byte	last but one Byte	Last Byte
		0xC5	0x5C	XX			XX			XX			XX		XX		XX					0x5C				0xC5
		֡ͷ	֡ͷ	����		���ȸ��ֽ�	���ȵ��ֽ�	����	����	BCCУ����			֡β				֡β
		
		�緢�����ݣ�C5 5C 00 00 04 22 33 44 FF AE 5C C5
		BCC = ����֡ͷ֡β��bcc��������
		BCC = 00 ^ 00 ^ 04 ^ 22 ^ 33 ^ 44 ^ FF = AE

����:	0x00	0x01			0x02			0xFF
��ע:	����	���ݴ��俪ʼ	���ݴ������	Ӧ��

*************************************************************************************/


#include "stm32f10x.h"
#include "../bsp/transfer_BIN_to_EX_FLASH/transfer_BIN_to_EX_FLASH.h"
#include "../bsp/w25qxx/w25qxx.h" 
#include "../BSP/usart/usart.h"

static u32 bytesStored = 0;	//�Ѿ�������ֽ�
static u32 stroage_StartAddress = 0;	//Ƭ�ⴢ�����ʼ��ַ
static u8 transfer_started_flag = 0;	//�Ƿ��Ѿ���ʼ�������ݱ�־

static u8 TBEF_recvBuf[1000];	//����buf����󵥴δ����ֽ���Ӧ������buf��С-8
static u16 TBEF_recvBuf_tail = 0;
u8 TBEF_uart_recv_finish = 0, TBEF_uart_recving_flag = 0, TBEF_uart_recv_tim_cnt = 0;


//�û��ص��������˴��޸���Ҫ��¼�ĺ���
void TBEF_data_CallBack(u8 *dataBuf, u16 len)
{
	//���������޸Ĵ˴�
	W25QXX_Write(dataBuf, stroage_StartAddress + bytesStored, len);//д��flash
	
	bytesStored += len;
}

//���ڴ��������ڴ����ж��е��ã������յ����ֽڴ���data
void TBEF_uart_receive_process(u8 data)
{  	
	if(TBEF_uart_recv_finish == 0){
		TBEF_recvBuf[TBEF_recvBuf_tail++] = data;		// ���뻺������
		TBEF_uart_recving_flag = 1;                     // ���� ���ձ�־
		TBEF_uart_recv_tim_cnt = 0;	                    // ���ڽ��ն�ʱ����������	
	}
	if(TBEF_recvBuf_tail >= sizeof(TBEF_recvBuf))
	{
		 TBEF_recvBuf_tail = 0;                               	// ��ֹ����������
	}		
	
}

//��ʱ�����������ڶ�ʱ���ж��е��ã�1Msһ��
void TBEF_tim_process(void)		//1MS����һ��
{
	/* ���ڽ�������жϴ��� */
	if(TBEF_uart_recving_flag)                        	// ��� usart�������ݱ�־Ϊ1
	{
		TBEF_uart_recv_tim_cnt++;             // usart ���ռ���	
		if(TBEF_uart_recv_tim_cnt > 10)       // ������ 3 ms δ���յ����ݣ�����Ϊ���ݽ�����ɡ�
		{
			TBEF_uart_recv_finish = 1;
			TBEF_uart_recving_flag = 0;
			TBEF_uart_recv_tim_cnt = 0;
		}
	}
	
}

//���������е��ã���Ҫһֱѭ�����ã��˺���Ϊ��������
void TBEF_mainFun(void)
{
	u8 ret = TBEF_frameVerify();
	if(ret)
		TBEF_Err_Processing(ret);
	else
		TBEF_CMD_Precesing();
	
	TBEF_SendACK(ret);
	TBEF_recvBuf_tail = 0;
	TBEF_uart_recv_finish = 0;
}

void TBEF_CMD_Precesing(void)
{
	u8 cmd = TBEF_recvBuf[2];
	
	if(cmd == 0x00)
	{
		u16 length = TBEF_recvBuf[3] * 0xFF + TBEF_recvBuf[4];;
		TBEF_data_CallBack(TBEF_recvBuf + 5, length);
		
	}
	else if(cmd == 0x01)
	{
		transfer_started_flag = 1;
		bytesStored = 0;
		stroage_StartAddress = (uint32_t)TBEF_recvBuf[5] * 0x1000000 + (uint32_t)TBEF_recvBuf[6] * 0x10000 + (uint32_t)TBEF_recvBuf[7] * 0x100 + (uint32_t)TBEF_recvBuf[8] ; 
		//stroage_StartAddress = ((uint32_t)TBEF_recvBuf[5] << 24) & ((uint32_t)TBEF_recvBuf[6] << 16) & ((uint32_t)TBEF_recvBuf[7] << 8) & (uint32_t)TBEF_recvBuf[8];
	}
	else if(cmd == 0x02)
	{
		transfer_started_flag = 0;
	}else {
		//TODO:
	}
}

void TBEF_Err_Processing(u8 err)
{
	switch(err){
		case 0:	/* no err */
			
			break;
		case 1:
			UartSendStr(USART1, (u8*)"Frame Head err!\r\n");
			break;
		case 2:
			UartSendStr(USART1, (u8*)"Frame Tail err!\r\n");
			break;
		case 3:
			UartSendStr(USART1, (u8*)"BCC err!\r\n");
			break;
		default:
			UartSendStr(USART1, (u8*)"Unknow err!\r\n");
	};
}
	
u8 TBEF_frameVerify(void)
{
	u16 length = 0;
	u8 bcc = 0x00;
	while(TBEF_uart_recv_finish != 1);	//wait receive finish
	
	
	if(TBEF_recvBuf[0] != 0xC5)
		return 1;	//֡ͷ����
	if(TBEF_recvBuf[1] != 0x5C)
		return 1;	//֡ͷ����
	length = (u16)TBEF_recvBuf[3] * 0xFF + (u16)TBEF_recvBuf[4];
	
	for(u16 i = 2; i < 5 + length; i ++){
		bcc ^= TBEF_recvBuf[i];
	}
	
	if(bcc != TBEF_recvBuf[5 + length])
		return 2;	//bccУ�������
	
	if(TBEF_recvBuf[6 + length] != 0x5C)
		return 3;	//֡β����
	if(TBEF_recvBuf[7 + length] != 0xC5)
		return 3;	//֡β����
	
	return 0;
	
}

//�磺C5 5C FF 00 01 00 FE 5C C5	��ʾû�д���
void TBEF_SendACK(u8 ERROR)
{
	u8 sendBuf[9] = {0xC5, 0x5C, 0x00, 0x00, 0x01, ERROR, 0x00, 0x5C, 0xC5};
	sendBuf[2] = TBEF_recvBuf[2];
	sendBuf[6] = sendBuf[2] ^ sendBuf[3] ^ sendBuf[4] ^ sendBuf[5] ; 
	
	UartSendMultByte(USART1, sendBuf, 9);
}

void TBEF_clearRecvBuf(void)
{
	while(TBEF_recvBuf_tail--)
	{
		TBEF_recvBuf[TBEF_recvBuf_tail] = 0;
	}
	
}




/*
//W25Q64
//����Ϊ8M�ֽ�,����128��Block,2048��Sector 
//4KbytesΪһ��Sector
//16������Ϊ1��Block

//���±�ʾ��ַΪW25QXX�ĵ�һ������ĵ�0�������ĵ�0����ַ
#define W25QXX_STORAGE_Block	1
#define W25QXX_STORAGE_Sector	0
#define W25QXX_STORAGE_Sector_OFFSET	0
//static u32 TBEF_W25QXX_StorageAddress = W25QXX_STORAGE_Block * 4 * 1024 * 16 + 4 * 1024 * W25QXX_STORAGE_Sector + W25QXX_STORAGE_Sector_OFFSET;	//Ҫ��¼�ľ����ַ
*/






