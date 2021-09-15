/**
  ******************************************************************************
  * @file    bsp_mpu6050.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief    mpu6050驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 霸道 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

//#include "./HARDWARE/MPU6050/mpu6050.h"
#include "mpu6050.h"
//#include "./usart/bsp_usart.h"
//#include "./i2c/bsp_i2c.h"

/**
  * @brief   写数据到MPU6050寄存器
  * @param   
  * @retval  
  */
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	MPU_I2C_ByteWrite(reg_dat,reg_add); 
}

/**
  * @brief   从MPU6050寄存器读取数据
  * @param   
  * @retval  
  */
void MPU6050_ReadData(u8 reg_add,unsigned char* Read,u8 num)
{
	MPU_I2C_BufferRead(Read,reg_add,num);
}


/**
  * @brief   初始化MPU6050芯片
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
  int i=0,j=0;
  //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //解除休眠状态
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //配置加速度传感器工作在2G模式
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
}

/**
  * @brief   读取MPU6050的ID
  * @param   
  * @retval  正常返回1，异常返回0
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //读器件地址
	if(Re != 0x68)
	{
		MPU_ERROR("MPU6050 dectected error!\r\n检测不到MPU6050模块，请检查模块与开发板的接线");
		return 0;
	}
	else
	{
		MPU_INFO("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}

/**
  * @brief   读取MPU6050的加速度数据
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
  * @brief   读取MPU6050的角加速度数据
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
  * @brief   读取MPU6050的原始温度数据
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}



//MPU_iic定义


/**
  * @brief   从I2C设备里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从slave读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的从设备的地址
  *     @arg NumByteToWrite:要从从设备读取的字节数
  * @retval  正常返回1，异常返回0
  */
uint8_t MPU_I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
	//u8 res;

    IIC_Start();

    IIC_Send_Byte(MPU6050_SLAVE_ADDRESS|0);//发送器件地址+写命令 

    IIC_Wait_Ack();//等待应答

    IIC_Send_Byte(ReadAddr);//写寄存器地址

    IIC_Wait_Ack();//等待应答

    IIC_Start();

    IIC_Send_Byte(MPU6050_SLAVE_ADDRESS|1);//发送期间地址+读命令

    IIC_Wait_Ack();//等待应答

    *pBuffer = IIC_Read_Byte(0);//读取数据，发送nACK

    IIC_Stop();//产生一个停止条件

    return 1;
}

/**
  * @brief   写一个字节到I2C设备中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  正常返回1，异常返回0
  */
uint8_t MPU_I2C_ByteWrite(u8 pBuffer, u8 WriteAddr)
{
	IIC_Start();

    IIC_Send_Byte(MPU6050_SLAVE_ADDRESS|0);//发送器件地址+写命令 

    if(IIC_Wait_Ack())  //等待应答

    {

        IIC_Stop(); 

        return 1;       

    }

    IIC_Send_Byte(WriteAddr); //写寄存器地址

    IIC_Wait_Ack();     //等待应答 

        IIC_Send_Byte(pBuffer);//发送数据

    if(IIC_Wait_Ack())  //等待ACK

    {

        IIC_Stop();  

        return 1;        

    }        

    IIC_Stop();  

    return 0;
}















