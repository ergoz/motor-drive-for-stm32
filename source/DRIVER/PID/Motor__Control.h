#ifndef _MOTOR__CONTROL_H_
#define _MOTOR__CONTROL_H_

extern void M_PID_Init(void);
//�ٶ�ʽ����
extern void AM_Control(int target);
extern void BM_Control(int target);
extern void CM_Control(int target);
//��λʽ����
extern void AM_DW_Control(long target);
extern void BM_DW_Control(long target);
extern void CM_DW_Control(long target);

#endif 







