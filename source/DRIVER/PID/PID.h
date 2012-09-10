#ifndef _PID_H
#define _PID_H

//PID�������						
typedef struct tagPID
{				
	  long nSum_Error;         //  ����ۼ�
	  long nSet_Point;         //  �趨Ŀ�� 
	  long nOutput;			   //  PID�������ֵ	
	 					
      double Proportion;       //  �������� 	 
	  double Integral;         //  ���ֳ��� 	 		
      double Derivative;       //  ΢�ֳ���  
	 
	  long nPrev_Error;        //  ��ǰ���			
      long nLast_Error;        //  �ϴ����	
}PID,*pPID;
//PID������ʼ��
extern void   PID_Init(PID *pPIDDATA,double P,double I,double D);
//����ʽPID���� 
extern long   IncPIDCalc(PID *pPIDDATA,long goal,long FeedBack);
//λ��ʽPID������� 
extern long   LocPIDCalc(PID *pPIDDATA,long goal,long FeedBack); 
//��������
extern short  Motor_LimitValue(long nLimitValue);
extern short  LimitValue(long nLimitValue,long nMax);

extern void   PID_Output(int Output,int nID);
extern void   PWM_TESTA(unsigned short u1,unsigned short u2,unsigned short u3);
extern void   PWM_TESTB(unsigned short u1,unsigned short u2,unsigned short u3);
#endif

