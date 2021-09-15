/**
  ******************************************************************************
  * @file    bsp_mpu6050.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief    mpu6050����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� �Ե� ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

//#include "./HARDWARE/MPU6050/mpu6050.h"
#include "mpu6050.h"
//#include "./usart/bsp_usart.h"
//#include "./i2c/bsp_i2c.h"

/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   
  * @retval  
  */
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	MPU_I2C_ByteWrite(reg_dat,reg_add); 
}

/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param   
  * @retval  
  */
void MPU6050_ReadData(u8 reg_add,unsigned char* Read,u8 num)
{
	MPU_I2C_BufferRead(Read,reg_add,num);
}


/**
  * @brief   ��ʼ��MPU6050оƬ
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
  int i=0,j=0;
  //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //�������״̬
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ�����
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //���ü��ٶȴ�����������2Gģʽ
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}

/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  ��������1���쳣����0
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
	if(Re != 0x68)
	{
		MPU_ERROR("MPU6050 dectected error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		return 0;
	}
	else
	{
		MPU_INFO("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}

/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}



//MPU_iic����


/**
  * @brief   ��I2C�豸�����ȡһ������ 
  * @param   
  *		@arg pBuffer:��Ŵ�slave��ȡ�����ݵĻ�����ָ��
  *		@arg WriteAddr:�������ݵĴ��豸�ĵ�ַ
  *     @arg NumByteToWrite:Ҫ�Ӵ��豸��ȡ���ֽ���
  * @retval  ��������1���쳣����0
  */
uint8_t MPU_I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
	//u8 res;

    IIC_Start();

    IIC_Send_Byte(MPU6050_SLAVE_ADDRESS|0);//����������ַ+д���� 

    IIC_Wait_Ack();//�ȴ�Ӧ��

    IIC_Send_Byte(ReadAddr);//д�Ĵ�����ַ

    IIC_Wait_Ack();//�ȴ�Ӧ��

    IIC_Start();

    IIC_Send_Byte(MPU6050_SLAVE_ADDRESS|1);//�����ڼ��ַ+������

    IIC_Wait_Ack();//�ȴ�Ӧ��

    *pBuffer = IIC_Read_Byte(0);//��ȡ���ݣ�����nACK

    IIC_Stop();//����һ��ֹͣ����

    return 1;
}

/**
  * @brief   дһ���ֽڵ�I2C�豸��
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ 
  * @retval  ��������1���쳣����0
  */
uint8_t MPU_I2C_ByteWrite(u8 pBuffer, u8 WriteAddr)
{
	IIC_Start();

    IIC_Send_Byte(MPU6050_SLAVE_ADDRESS|0);//����������ַ+д���� 

    if(IIC_Wait_Ack())  //�ȴ�Ӧ��

    {

        IIC_Stop(); 

        return 1;       

    }

    IIC_Send_Byte(WriteAddr); //д�Ĵ�����ַ

    IIC_Wait_Ack();     //�ȴ�Ӧ�� 

        IIC_Send_Byte(pBuffer);//��������

    if(IIC_Wait_Ack())  //�ȴ�ACK

    {

        IIC_Stop();  

        return 1;        

    }        

    IIC_Stop();  

    return 0;
}














