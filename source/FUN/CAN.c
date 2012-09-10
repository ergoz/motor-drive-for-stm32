#include "STD.h"

#define   GPIO_CAN                   GPIOB 
#define   RCC_APB2Periph_GPIO_CAN    RCC_APB2Periph_GPIOB
#define   GPIO_Pin_RX                GPIO_Pin_8
#define   GPIO_Pin_TX                GPIO_Pin_9

#define   SPEED_ONE                  0x01
#define   SPEED_TWO                  0x02
#define   SPEED_THREE                0x03
#define   MY_ID     				 0

void Can_Config_Init(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);
  /* CAN cell init */  // 36M 
  CAN_InitStructure.CAN_TTCM=DISABLE;
  CAN_InitStructure.CAN_ABOM=ENABLE;
  CAN_InitStructure.CAN_AWUM=DISABLE;
  CAN_InitStructure.CAN_NART=DISABLE;
  CAN_InitStructure.CAN_RFLM=DISABLE;
  CAN_InitStructure.CAN_TXFP=DISABLE;
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
  //����ͬ����Ծ���Ϊ1��ʱ�䵥λ
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  //ʱ���1Ϊ8��ʱ�䵥λ
  CAN_InitStructure.CAN_BS1=CAN_BS1_9tq;
  //ʱ���2Ϊ7��ʱ�䵥λ
  CAN_InitStructure.CAN_BS2=CAN_BS2_8tq;
  //pclk/((1 + 9 + 8)*2) =    kbits
  CAN_InitStructure.CAN_Prescaler = 2;
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN filter init */
  //��ʼ��������Ϊ1
  CAN_FilterInitStructure.CAN_FilterNumber=1;
  //������Ϊ��־����λģʽ
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  //������Ϊ32λ
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  /* 
  //������
  //�����趨��������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ����
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  //�����趨��������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���)
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  //�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ��)
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  //�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���)
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  */
  
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=(MY_ID<<5); 
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000; 
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;
  	 
  //ָ��������ָ��FIFO0
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;

  CAN_FilterInit(&CAN_FilterInitStructure);
   
  /* CAN FIFO0 message pending interrupt enable */   
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);   
 }

void Can_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   /* Configure CAN pin: RX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_RX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_TX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
  
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
}
void Can_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Enable CAN1 RX0 interrupt IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
void CAN_RCC_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_CAN,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
}
void CanBus_Init(void)
{  
  CAN_RCC_Configuration();	 
  Can_GPIO_Configuration();	
  Can_Config_Init();
  Can_NVIC_Configuration();		   
}
void Can_Send(CanTxMsg TxMessage)
{
  uint8_t ret = 0;
  uint8_t TransmitMailbox = 0; 
  uint32_t i = 0;
  TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);
  i = 0;
  do
  {
   ret = CAN_TransmitStatus(CAN1, TransmitMailbox);
   switch(ret)
   {
    case CANTXOK:
	 return;
	case CANTXPENDING:
	 ;
	break;
	case CANTXFAILED:case CAN_NO_MB:
	 return;
   } 
   i++;
  }
  while((ret != CANTXOK) && (i != 0xFF));
}
void Can_STD_Send(uint32_t StdId,uint8_t Datalen,uint8_t* pData)
{
  CanTxMsg TxMessage;
  uint32_t i = 0;
  uint8_t ret = 0;
  uint8_t TransmitMailbox = 0;

  /* transmit */
  //���ñ�׼֡ʱӲ���Զ�����������5λ
  TxMessage.StdId = StdId;
  //����֡ 
  TxMessage.RTR = CAN_RTR_DATA;
  //��׼֡
  TxMessage.IDE = CAN_ID_STD;
  //�趨֡�������8�ֽ�
  TxMessage.DLC = Datalen;
  for(i = 0;i < Datalen&&i < 8;i++)
  {
   TxMessage.Data[i] = pData[i];
  }
  TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);
  i = 0;
  do
  {
   ret = CAN_TransmitStatus(CAN1, TransmitMailbox);
   switch(ret)
   {
    case CANTXOK:
	 return;
	case CANTXPENDING:
	 ;
	break;
	case CANTXFAILED:case CAN_NO_MB:
	 return;
   } 
   i++;
  }
  while((ret != CANTXOK) && (i != 0xFF));
}
//�����ж�
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	BYTE byData[2]={0};
   	CanRxMsg RxMessage;
	memset(&RxMessage,0,sizeof(CanRxMsg));
  	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	if(RxMessage.StdId == MY_ID)
	{	
		byData[0] = RxMessage.Data[0];
		byData[1] = RxMessage.Data[1];
		g_Speed.A = *(short*)byData;
		
		byData[0] = RxMessage.Data[2];
		byData[1] = RxMessage.Data[3];
		g_Speed.B = *(short*)byData;
		
		byData[0] = RxMessage.Data[4];
		byData[1] = RxMessage.Data[5];
		g_Speed.C = *(short*)byData;

		g_Speed.Mode = RxMessage.Data[6];
		g_Speed.Elecmd = RxMessage.Data[7];
		g_S = 0;
    }	
}


