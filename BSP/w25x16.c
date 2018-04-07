
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_spi.h"
#include "w25x16.h"
#include "spi.h"
#include "delay_time.h"
#include <string.h> 


extern void WS2812_send_DATA(uint8_t (*color)[3], uint16_t len);



Ws2812b_Config_t Ws2812b_Config_data = {
  .frames_h = 0,                // 1    帧数 高位
  .frames_l = 2,                  // 2    帧数 低位
  .last_time_h = 0,              // 3    每帧持续时间 高位 ms
  .last_time_l = 200,               // 4    每帧持续时间 低位 ms
  .data_offset_h = 0,            // 5    数据相对ox0的偏移地址 高位
  .data_offset_l = 16,            // 6    数据相对ox0的偏移地址 低位
  .led_pixel = 40,                 // 7    led的像素点个数
  .save =66,
};



//每块（Block）的大小为 64KB，每个扇区（Sector）的大小为 4 KB

void SPI_FLASH_SectorErase(uint32_t SectorAddr) //扇区删除
{
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitForWriteEnd();
    W25Q_CS_L;
    SPI_FLASH_SendByte(W25X_SectorErase);
    SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    SPI_FLASH_SendByte(SectorAddr & 0xFF);
    W25Q_CS_H;
    SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BulkErase(void) // 片删除
{
  SPI_FLASH_WriteEnable();
  W25Q_CS_L;
  SPI_FLASH_SendByte(W25X_ChipErase);
  W25Q_CS_H;
  SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) //页写入
{
    SPI_FLASH_WriteEnable();
    
    W25Q_CS_L;
    
    SPI_FLASH_SendByte(W25X_PageProgram);
    SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    SPI_FLASH_SendByte(WriteAddr & 0xFF);
    
    if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
    {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
    }
    
    while (NumByteToWrite--)
    {
        SPI_FLASH_SendByte(*pBuffer);
        pBuffer++;
    }
    
    W25Q_CS_H;
    SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) //数据buf 写入
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

  if (Addr == 0) 
  {
    if (NumOfPage == 0)
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else 
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else 
  {
    if (NumOfPage == 0)
    {
      if (NumOfSingle > count)
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) // 数据buf读出
{

  W25Q_CS_L;
  SPI_FLASH_SendByte(W25X_ReadData);
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) 
  {
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    pBuffer++;
  }
  W25Q_CS_H;
}

uint32_t SPI_FLASH_ReadID(void)  //读取ID
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  W25Q_CS_L;
  SPI_FLASH_SendByte(W25X_JedecDeviceID);
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);
  W25Q_CS_H;
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
  return Temp; // should be 0x00EF4015
}

uint32_t SPI_FLASH_ReadDeviceID(void) // 读取Device ID
{
  uint32_t Temp = 0;

  W25Q_CS_L;
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  Temp = SPI_FLASH_SendByte(Dummy_Byte);
  W25Q_CS_H;
  return Temp;
}

void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{

  W25Q_CS_L;
  SPI_FLASH_SendByte(W25X_ReadData);
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}

uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  while(SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
  SPI_SendData( byte);
  while(SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  return SPI_ReceiveData();
}

void SPI_FLASH_WriteEnable(void)
{
  W25Q_CS_L;
  SPI_FLASH_SendByte(W25X_WriteEnable);
  W25Q_CS_H;
}

void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;
  W25Q_CS_L;
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  do
  {
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
  }
  while ((FLASH_Status & WIP_Flag) == SET); 
  W25Q_CS_H;
}
void SPI_Flash_PowerDown(void)   
{ 
  W25Q_CS_L;
  SPI_FLASH_SendByte(W25X_PowerDown);
  W25Q_CS_H;
}   

void SPI_Flash_WAKEUP(void)   
{
  W25Q_CS_L;
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);
  W25Q_CS_H;
}   

void W25x16_Test(void)
{
    u8 Buffer[8] = "abcdefg";
    if(SPI_FLASH_ReadID() != 0xEF4015)
        return ;
    if(SPI_FLASH_ReadDeviceID() != 0x14) // 14 <---> W25Q16   -->16 @2M
        return ;
//    SPI_FLASH_SectorErase(FLASH_SectorToErase);    
//
//    SPI_FLASH_BufferWrite((u8 *)Buffer,FLASH_SectorToErase,8);	
//    memset(Buffer,0,sizeof(Buffer));
//    delay_ms(20);
//
//    SPI_FLASH_BufferRead((u8 *)Buffer,FLASH_SectorToErase, 8);
//    delay_ms(20);
//    SPI_FLASH_BufferRead((u8 *)Buffer,FLASH_SectorToErase + 2, 6);
//    delay_ms(20);
    
    
    SPI_FLASH_BulkErase();//全片清空
    SPI_FLASH_BufferWrite((u8 *)Buffer,FLASH_SectorToErase,8);	
    SPI_FLASH_BufferWrite((u8 *)Buffer,0X1EFFFF,8);	   
    memset(Buffer,0,sizeof(Buffer));
    delay_ms(20);

    SPI_FLASH_BufferRead((u8 *)Buffer,FLASH_SectorToErase, 8);
    memset(Buffer,0,sizeof(Buffer));

    SPI_FLASH_BufferRead((u8 *)Buffer,0X1EFFFF, 8);
    memset(Buffer,0,sizeof(Buffer));
    
    SPI_FLASH_BulkErase();//全片清空
//    WS2812_send_DATA( Buffer, 16);     
}

void W25x16_Save_Ws2812b_Config(void) // 应该以传参方式 todo
{
    Ws2812b_Config_data.save = 66;
    SPI_FLASH_BulkErase();//全片清空
    
    SPI_FLASH_BufferWrite((u8 *)&Ws2812b_Config_data,FLASH_SectorToErase,sizeof(Ws2812b_Config_t));	

    memset(&Ws2812b_Config_data,0,sizeof(Ws2812b_Config_t));
     
    SPI_FLASH_BufferRead((u8 *)&Ws2812b_Config_data,FLASH_SectorToErase, sizeof(Ws2812b_Config_t));
    Ws2812b_Config_data.save = 67;
}


