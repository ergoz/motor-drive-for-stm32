/*=========================================================*
*   ������˵����SP027 ����Һ��ģ�������⺯��               *
*   �汾��1.0                                                      *
*   ���ߣ�w.j                                                      *
*   �������ڣ�2009/1/18                                    *
* -------------------------------------------------------- *
*  [Ӳ��˵��]                                              *
*     			VSS     CLK      DI    VDD                 *
*              ��Դ��  ʱ���ź�  ����  ��Դ��              *
*  ע��: Ӧ��д��һ�����ݺ���ĩβ����һ��ʱ��������(Paulse)*
*   ��������                                                            *
*   ϵͳʱ�ӣ�                                                        *
* ---------------------------------------------------------------- *
*  [ʹ��˵��]                                              *
*=========================================================*/

/*==================*
* ͷ �� �� �� �� �� *
*==================*/
#include "stm32f10x.h" 
#include "stm32f10x_gpio.h"
#include "math.h"
#include "Hard_ware.h"
/*==================*
*   �� �� �� �� ��  *
*==================*/
#define   DI0          GPIO_WriteBit(GPIOD,GPIO_Pin_3,Bit_RESET); //P2
#define   DI1          GPIO_WriteBit(GPIOD,GPIO_Pin_3,Bit_SET);
#define   CLK0         GPIO_WriteBit(GPIOD, GPIO_Pin_2,Bit_RESET);//P3
#define   CLK1         GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_SET);

#define   Wrt1         {CLK0;Delay(4);CLK1;Delay(4);DI1;Delay(4);DI0;Delay(4);}              //д����"1"
#define   Wrt0         {CLK0;Delay(4);CLK1;Delay(4);DI0;Delay(4);DI1;Delay(4);}              //д����"0" 
#define   Paulse       {CLK0;Delay(3);CLK1;Delay(3);}                      //һ��������
#define   GO           0x01
#define   Ready        0x02

/*==================*
*  ģ��ṹ�嶨���� *
*==================*/

/*==================*
*   �� �� �� �� ��  *
*==================*/
void Display_Onechar(uint8_t Data);

/*==================*
*   ģ�麯��������  *
*==================*/
void CLS(void);
void DisplayINTNum_SP027(int32_t Number);
void DisplayFloatNum_SP027(float Num);

/*==================*
*   ģ�����������  *
*==================*/
const uint8_t SP027DisplayCode[] = {0x09,0xcf,0x91,0x85,0x47,0x25,0x21,0x8f,0x01,0x05,
                         	 0x03,0x61,0x39,0xc1,0x31,0x33,0x29,0x43,0x7b,0xc9,
						 	 0x79,0xe1,0x13,0xf3,0x49,0xf7,0xfd,0xff};
							 /*{0~F,G,H,I,J,L,O,P,r,U,_,-}*/ 
						 
/*==================*
*   ȫ�ֱ���������  *
*==================*/

/*=========================================================*
*   �������ܣ���������                                     *
*   ��    �룺    ��                                       *
*   ��    ����    ��                                       *
* -------------------------------------------------------- *
*  [ʹ��˵��]                                              *
*=========================================================*/
void CLS(void)
{
   uint8_t k = 0; 
   for(k=0;k<41;k++)
   {
      Wrt0;
   }	   
}                       

/*=========================================================*
*   �������ܣ�ʮ����������ʾ����                           *
*   ��    �룺Ҫ��ʾ����(long)                             *
*   ��    ����    ��                                       *
* -------------------------------------------------------- *
*  [ʹ��˵��]                                              *
*=========================================================*/
void DisplayINTNum_SP027(int32_t Number)
{	
	s8 lcv_Counter = 0;
    //uint8_t cNonce_BIT_Number = 0;
	uint8_t tmpNumber[5] = {0};
	uint8_t isNegative = FALSE;

	if(Number<0)
	{
		isNegative = TRUE;
	}
	Number = fabs(Number);
	CLS();	//����
	
	do
	{   
		if(lcv_Counter+isNegative>=5)
		{
			tmpNumber[0] &=~(1<<0);	//�����
			break;
		}
		tmpNumber[lcv_Counter] = SP027DisplayCode[Number%10];   //���㵱ǰλ�ϵ�����Number%10
		Number=Number/10;    //����ȵ�ǰλ����λ�ϵ�����
		lcv_Counter ++;
	}while(Number);
	if(isNegative)
	{
		tmpNumber[lcv_Counter]=0xF7;	//����
		lcv_Counter++;
	}
	while(lcv_Counter--)
	{
		Display_Onechar(tmpNumber[lcv_Counter]);	//��ʾ
	}
    Paulse;   
}    

/*=========================================================*
*   �������ܣ�Float������ʾ����                            *
*   ��    �룺Ҫ��ʾ����(float)                            *
*   ��    ����    ��                                       *
* -------------------------------------------------------- *
*  [ʹ��˵��]                                              *
*=========================================================*/
void DisplayFloatNum_SP027(float Num)
{
    s8 lcv_Counter = 0;
    //uint8_t cNonce_BIT_Number = 0;
	uint8_t tmpNumber[5] = {0};
	s32 Number = 0;
	uint8_t isNegative = FALSE;
	
	if(Num<0)
	{
		isNegative = TRUE;
	}
	Number =fabs((s32)(Num*100));
	CLS();	//����
	
	do
	{   
		if(lcv_Counter+isNegative>=5)
		{
			tmpNumber[0] &=~(1<<0);	//�����
			break;
		}
		tmpNumber[lcv_Counter] = SP027DisplayCode[Number%10];   //���㵱ǰλ�ϵ�����Number%10
		Number=Number/10;    //����ȵ�ǰλ����λ�ϵ�����
		lcv_Counter ++;
	}while(Number||(lcv_Counter<=2));
	tmpNumber[2] &=~(1<<0);	//С����
	if(isNegative)
	{
		tmpNumber[lcv_Counter]=0xF7;	//����
		lcv_Counter++;
	}
	while(lcv_Counter--)
	{
		Display_Onechar(tmpNumber[lcv_Counter]);	//��ʾ
	}
    Paulse;
}    

/*=========================================================*
*   �������ܣ�д��������                                   *
*   ��    �룺Ҫ��ʾ����(UINT8)                            *
*   ��    ����    ��                                       *
* -------------------------------------------------------- *
*  [ʹ��˵��]                                              *
*=========================================================*/
void Display_Onechar(uint8_t Data)
{
   uint8_t CharLength = 0;
   uint8_t NumTemp = 0;
   
   for (CharLength=0;CharLength<8;CharLength++)
   {
	  NumTemp = ((Data>>(7-CharLength))&0x01);
	  if (NumTemp == 0)
	  {
		 Wrt1;
	  }
	  else
	  {
	     Wrt0;
	  }	  
   }         
}

