
#include "stm8s_spi.h"
#include "stm8s_gpio.h"
#include "spi.h"

void SPI_Configation( void ) //�ȷ���MSB  fMASTER/4   ����״̬ʱSCK���ָߵ�ƽ  ���ݲ����ӵڶ���ʱ�ӱ��ؿ�ʼ  ˫�ߵ�������ģʽ   ��ֹ������豸����
{
    //����ͨ�Ų�����1Mbps,����״̬ʱ��SCK���ֵ͵�ƽ,���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ,
    //ͨ��ʱ��������λ����λ��ǰ����λ�ں�,NSS�����������
    //�ر�flash SPI CS Ƭѡ��
    //�ر�flash SPI CS Ƭѡ��
    GPIO_Init( W25Q_PORT_CS, W25Q_PIN_CS, GPIO_MODE_OUT_PP_HIGH_SLOW ); //C GPIO 2 SPI CS ����
    W25Q_CS_H;
    /* SPI configuration */
    SPI_DeInit();
    SPI_Init( SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW,
              SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07 );
    //�˿�����Ϊ���ٰ���,
    //  GPIO_Init(GPIOC, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);
    CLK_PeripheralClockConfig( CLK_PERIPHERAL_SPI, ENABLE ); //����ʱ��
    SPI_Cmd( ENABLE ); //ʹ��SPI
    W25Q_CS_H;
}


u8 SPI_SendByte( u8 byte )
{
    while( SPI_GetFlagStatus( SPI_FLAG_TXE ) == RESET );
    SPI_SendData( byte );
    while( SPI_GetFlagStatus( SPI_FLAG_RXNE ) == RESET );
    return SPI_ReceiveData();
}
