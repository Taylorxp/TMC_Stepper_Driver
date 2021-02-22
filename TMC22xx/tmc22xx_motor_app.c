/***************************************************************************************************
* FILE: tmc22xx_motor_app.c

* DESCRIPTION:  

* CREATED: 2020/09/21, by taylor
***************************************************************************************************/

/*************************************************************************************
* INCLUDE
*************************************************************************************/
#include "tmc22xx_motor_app.h"
#include <math.h>
#include <stdio.h>

/*************************************************************************************
* DEFINE
*************************************************************************************/
/* ��������ʱɲ���ٶ� */
#define TMC22xx_INV_BRAKE           5

/* ����������� */
#define MOTOR_IS_RUNNING(m)     (!((m)->cur_pos == (m)->tar_pos))
/* ������з��� */
#define GET_RUN_DIR(cur, tar)   (((tar)-(cur)) > 0 ? 1 : -1)
/* λ��ƫ�� */
#define GET_POS_DIFF(a, b)      (m_abs(b-a))

/***************************************************************************************************
* Description:  
***************************************************************************************************/
static inline int32_t m_acc_sum(int32_t v0, int32_t v1, int32_t acc)
{
    int32_t sum;

	sum = ((v1 * v1) - (v0 * v0)) / (2 * acc) - (acc * (v0 + v1) / 2);

    return sum;
}

static inline int32_t m_dec_sum(int32_t v0, int32_t v1, int32_t dec)
{
    int32_t sum;

	sum = ((v1 * v1) - (v0 * v0)) / (2 * dec) - (dec * (v0 + v1) / 2);

    return sum;
}

/***************************************************************************************************
* Description:  �����״̬
***************************************************************************************************/
int32_t TMC22xx_MotorIsRunning(uint8_t motor)
{
	if (motor) {
		return MOTOR_IS_RUNNING(&stTmc22xxMotor.para_motor2);
	} else {
		return MOTOR_IS_RUNNING(&stTmc22xxMotor.para_motor1);
	}
}

/***************************************************************************************************
* Description:  ��ȡ�����ǰ���ٶ�
***************************************************************************************************/
int32_t TMC22xx_GetMotorCurSpeed(uint8_t motor)
{
	if (motor) {
		return stTmc22xxMotor.para_motor2.cur_speed;
	} else {
		return stTmc22xxMotor.para_motor1.cur_speed;
	}
}

/***************************************************************************************************
* Description:  ��ȡ�����ǰλ��
***************************************************************************************************/
int32_t TMC22xx_GetMotorPosition(uint8_t motor)
{
	if (motor) {
		return stTmc22xxMotor.para_motor2.cur_pos;
	} else {
		return stTmc22xxMotor.para_motor1.cur_pos;
	}
}

/***************************************************************************************************
* Description:  �޸ĵ����ǰλ��
***************************************************************************************************/
int32_t TMC22xx_SetMotorPosition(uint8_t motor, int32_t new_pos)
{
	if (motor) {
		stTmc22xxMotor.para_motor2.cur_pos = new_pos;
	} else {
		stTmc22xxMotor.para_motor1.cur_pos = new_pos;
	}
	
	return 0;
}

/***************************************************************************************************
* Description:  �������в���
***************************************************************************************************/
int32_t TMC22xx_CalcuRunPara(struct tmc22xx_motor_para *pmotor)
{
//	pmotor->cur_pos += pmotor->pos_dir;
//	/* ��ת */
//	if (pmotor->pos_dir == 1) {
//		/* ���������ٹ��� */
//		if (pmotor->cur_pos < pmotor->dec_pos) {
//			if (pmotor->cur_speed != pmotor->tar_speed) {
//				if (++pmotor->psc >= pmotor->cur_speed * pmotor->acc) {
//					pmotor->psc = 0;
//					pmotor->cur_speed ++;
//					if (pmotor->cur_speed >= pmotor->tar_speed) {
//						pmotor->cur_speed = pmotor->tar_speed;
//					}
//				}
//			}
//		}
//		/* ���� */
//		else if (pmotor->cur_pos < pmotor->tar_pos) {
//			if (pmotor->cur_speed > 1) {
//				if (++pmotor->psc >= pmotor->cur_speed) {
//					pmotor->psc = 0;
//					pmotor->cur_speed -= pmotor->dec;
//					if (pmotor->cur_speed <= 0) {
//						pmotor->cur_speed = 1;
//					}
//				}
//			}
//		}
//		/* ͣ�� */
//		else {
//			pmotor->cur_speed = 0;
//			pmotor->pos_dir = 0;
//			pmotor->psc = 0;
//			TIM_Cmd(TIM4, DISABLE);
////			stTmc22xxMotorOps.end_step();
//		}
//	}
//	/* ��ת */
//	else if (pmotor->pos_dir == -1) {
//		/* ���������ٹ��� */
//		if (pmotor->cur_pos > pmotor->dec_pos) {
//			if (pmotor->cur_speed != pmotor->tar_speed) {
//				if (++pmotor->psc >= pmotor->cur_speed * pmotor->acc) {
//					pmotor->psc = 0;
//					pmotor->cur_speed ++;
//					if (pmotor->cur_speed >= pmotor->tar_speed) {
//						pmotor->cur_speed = pmotor->tar_speed;
//					}
//				}
//			}
//		}
//		/* ���� */
//		else if (pmotor->cur_pos > pmotor->tar_pos) {
//			if (pmotor->cur_speed > 1) {
//				if (++pmotor->psc >= pmotor->cur_speed) {
//					pmotor->psc = 0;
//					pmotor->cur_speed -= pmotor->dec;
//					if (pmotor->cur_speed <= 0) {
//						pmotor->cur_speed = 1;
//					}
//				}
//			}
//		}
//		/* ͣ�� */
//		else {
//			pmotor->cur_speed = 0;
//			pmotor->pos_dir = 0;
//			pmotor->psc = 0;
//			TIM_Cmd(TIM4, DISABLE);
////			stTmc22xxMotorOps.end_step();
//		}
//	}
//	else {
//		pmotor->cur_speed = 0;
//		pmotor->pos_dir = 0;
//		TIM_Cmd(TIM4, DISABLE);
////		stTmc22xxMotorOps.end_step();
//	}
//	
////	static uint32_t phase = 0;
////	phase = pmotor->cur_pos % (SINEWAVE_NUM);
//	
////	TIM3->CCR2 = phase_lib[phase];
//	TIM4->ARR = speed_lib[pmotor->cur_speed];
//
//	TIL_STEP = !TIL_STEP;
	
	/* ������һ����ʱ���� */
	uint32_t new_step_delay;
	/* ���ٹ��������һ����ʱ */
	static int32_t last_accel_delay;
	/* ��¼new_step_delay�е������������һ������ľ��� */
	static int32_t rest = 0;
	/* �ƶ����������� */
	static uint32_t step_count = 0;
	
	pmotor->cur_pos += pmotor->pos_dir;
	
	TIM4->ARR = pmotor->new_arr;
	TIL_STEP = !TIL_STEP;

	/* ��ת */
	if (pmotor->pos_dir == 1) {
		/* ���� */
		if (pmotor->cur_pos <= pmotor->acc_pos) {
			pmotor->acc_count++;
			new_step_delay = pmotor->new_arr - ((2 * pmotor->new_arr) + rest) / (4 * pmotor->acc_count + 1);
			rest = (2 * pmotor->new_arr + rest) % (4 * pmotor->acc_count + 1);
			last_accel_delay = new_step_delay;
		}
		/* ���� */
		else if (pmotor->cur_pos < pmotor->dec_pos) {
			new_step_delay = pmotor->min_arr;
			rest = 0;
		}
		/* ���� */
		else if (pmotor->cur_pos == pmotor->dec_pos) {
			new_step_delay = last_accel_delay;
			pmotor->acc_count = -1 * pmotor->dec_dist; 
		} else {
			pmotor->acc_count++;
			new_step_delay = pmotor->new_arr - ((2 * pmotor->new_arr) + rest) / (4 * pmotor->acc_count + 1);
			rest = (2 * pmotor->new_arr + rest) % (4 * pmotor->acc_count + 1);
		}
		
		if (pmotor->cur_pos == pmotor->tar_pos) {
			pmotor->new_arr = 0xFFFF;
			TIM_Cmd(TIM4, DISABLE);
		}
		
		pmotor->new_arr = new_step_delay;
	}
	/* ��ת */
	else if (pmotor->pos_dir == -1) {
		
	}
	
	
	
	return 0;
}

/* ���п��� */
int32_t TMC22xx_MotorBrake(uint8_t motor, int16_t dec)
{
//    TMC22xx_MOTOR_PARA_S *pmotor;
//    int32_t dec_dist;
//    
//    if (motor) pmotor = &stTmc22xxMotor.para_motor2;
//    else pmotor = &stTmc22xxMotor.para_motor1;
//    
//	if (pmotor->pos_dir != 0) {
//	    TIM_Cmd(TIM4, DISABLE);
////		stTmc22xxMotorOps.end_step();
//	
//		pmotor->dec = dec;
//		pmotor->tar_speed = 0;
//		pmotor->dec_pos = pmotor->cur_pos;
//		dec_dist = m_dec_sum(0, pmotor->cur_speed, dec);
//		if (pmotor->pos_dir == 1) {
//			/* ��ֹ����ɲ��ʱ�������ٽ�ֵ */
//			if (pmotor->max_pos <= (pmotor->cur_pos + dec_dist)) {
//				pmotor->tar_pos = pmotor->max_pos;
//			} else {
//				pmotor->tar_pos = pmotor->cur_pos + dec_dist;
//			}
//		} else if (pmotor->pos_dir == -1) {
//			/* ��ֹ����ɲ��ʱ�������ٽ�ֵ */
//			if (pmotor->min_pos >= (pmotor->cur_pos - dec_dist)) {
//				pmotor->tar_pos = pmotor->min_pos;
//			} else {
//				pmotor->tar_pos = pmotor->cur_pos - dec_dist;
//			}
//		}
//		
//		TIM_Cmd(TIM4, ENABLE);
////		stTmc22xxMotorOps.start_step();
//	}
//    
//	/* ɲ������ */
//	if (pmotor->pos_dir == 1) {
//		if (dec_dist >= (pmotor->tar_pos - pmotor->cur_pos)) {
//			return (pmotor->tar_pos - pmotor->cur_pos);
//		}
//	} else if (pmotor->pos_dir == -1) {
//		if (dec_dist >= (pmotor->cur_pos - pmotor->tar_pos)) {
//			return (pmotor->cur_pos - pmotor->tar_pos);
//		}
//	}
//	
//	return dec_dist;
}

int32_t TMC22xx_MotorGoto(uint8_t motor, int32_t tar_pos, uint16_t speed, int16_t acc, int16_t dec)
{
	int32_t tarPos = 0, curPos = 0, tarSpeed = 0, curSpeed = 0, acc_dist = 0, dec_dist = 0, delta_speed = 0, delta_pos = 0;
	/* ��ʼ���ٵĲ��� */
	static int32_t acc_dist_lim = 0;
	
    TMC22xx_MOTOR_PARA_S *pmotor;
    
    if (speed == 0 || acc == 0 || dec == 0)
        return 1;
	
//	if (speed >= 144) speed = 143;
	if (speed >= 100) speed = 100;
    
    if (motor) pmotor = &stTmc22xxMotor.para_motor2;
    else pmotor = &stTmc22xxMotor.para_motor1;
    
    if (tar_pos == pmotor->cur_pos)
        return 1;
    
    if ((tar_pos == pmotor->tar_pos) && (speed == pmotor->tar_speed))
        return 1;
    
    TIM_Cmd(TIM4, DISABLE);
//	stTmc22xxMotorOps.end_step();
    
//	/* �����෴������ɲ�� */
//	if ((int32_t)((tar_pos - pmotor->cur_pos) * pmotor->pos_dir) < 0) {
//		TMC22xx_MotorBrake(motor, TMC22xx_INV_BRAKE);
//		
//		while(1) {
//			if (pmotor->pos_dir == 0) {
//				break;
//			}
//		}
//	}
//	
//	tarPos 		= tar_pos;
//	tarSpeed 	= speed;
//	curPos 		= pmotor->cur_pos;
//	curSpeed 	= pmotor->cur_speed;
//	delta_pos 	= abs(tarPos - curPos);
//	delta_speed = tarSpeed - curSpeed;
//	
//	pmotor->acc = acc;
//	pmotor->dec = dec;
//	
//	/* Ŀ���ٶȴ��ڵ�ǰ�ٶ� */
//	if (delta_speed > 0) {
//		/* ���پ��� */
//		acc_dist = m_acc_sum(curSpeed, tarSpeed, acc);
//		/* ���پ��� */
//		dec_dist = m_dec_sum(1, tarSpeed, dec);
//		/* ���ٵ�����ֵ�����벻�� */
//		while (delta_pos < (acc_dist + dec_dist)) {
//			tarSpeed -= 2;
//			if (tarSpeed < 1) {
//				tarSpeed = 1;
//			}
//			acc_dist = m_acc_sum(curSpeed, tarSpeed, acc);
//			dec_dist = m_dec_sum(1, tarSpeed, dec);
//		}
//
//		pmotor->acc *= 1;
//	}
////	/* Ŀ���ٶ�С�ڵ�ǰ�ٶ� */
////	else if (delta_speed < 0) {
////		pmotor->acc *= -1;
////	}
////	/* Ŀ���ٶȵ��ڵ�ǰ�ٶ� */
////	else {
////		pmotor->acc = 0;
////	}
//	
//	dec_dist = m_dec_sum(0, tarSpeed, dec);
//	
//	if (tarPos > curPos) {
//		TIL_FORWARD_DIR();
//		pmotor->pos_dir = 1;
////		pmotor->acc_pos = pmotor->cur_pos + acc_dist;
//		pmotor->dec_pos = tarPos - dec_dist;
//		pmotor->max_pos = tarPos;
//	} else {
//		TIL_REVERSE_DIR();
//		pmotor->pos_dir = -1;
////		pmotor->acc_pos = pmotor->cur_pos - acc_dist;
//		pmotor->dec_pos = tarPos + dec_dist;
//		pmotor->min_pos = tarPos;
//	}
//	
//	pmotor->cur_speed = curSpeed;
//	pmotor->tar_speed = tarSpeed;
//	pmotor->cur_pos   = curPos;
//	pmotor->tar_pos   = tarPos;
//	
//    TIM_Cmd(TIM4, ENABLE);
//	stTmc22xxMotorOps.start_step();
//    
//	pmotor->tar_speed = (int32_t)(PAN_ALPHA_FREQ_x100 / speed);
//	pmotor->cur_speed = (int32_t)((PAN_FREQ_CONSTANT * sqrt(PAN_ALPHA_CONSTANT / acc)) / 100);
//	
//	acc_dist = (uint32_t)(speed * speed) / (PAN_ALPHA_x20000 * acc / 100);
//	if (acc_dist == 0)	acc_dist = 1;
//	
//	acc_dist_lim = (uint32_t)((tar_pos * dec) / (acc + dec));
//	if (acc_dist_lim == 0)	acc_dist_lim = 1;
//	
//	if (acc_dist_lim <= acc_dist) {
//		pmotor->dec_dist = acc_dist_lim - tar_pos;
//	} else {
//		pmotor->dec_dist = -(acc_dist * acc / dec);
//	}
//	
//	if (pmotor->dec_dist == 0) {
//		pmotor->dec_dist = -1;
//	}
//	
//	pmotor->dec_pos = tar_pos + pmotor->dec_dist;
//	
//	if (pmotor->cur_speed <= pmotor->tar_speed) {
//		pmotor->cur_speed = pmotor->tar_speed;
//	}
//	
//	TIM_Cmd(TIM4, ENABLE);
//	
//	delta_pos = abs(tar_pos - pmotor->cur_pos);
	
	/* ����������� */
	if (MOTOR_IS_RUNNING(pmotor)) {
		
	}
	/* ��ֹ */
	else {
		/* Ŀ���ٶ� */
		pmotor->tar_speed = speed;
		pmotor->min_arr = PAN_ALPHA_FREQ_x10 / speed;
		
		/* Ŀ��λ�� */
		pmotor->tar_pos = tar_pos;
		
		/* ��ʼ�ٶ� */
		pmotor->cur_speed = 0;
		pmotor->new_arr = (uint32_t)((PAN_FREQ_CONSTANT * sqrt((double)(PAN_2alpha_x1000000 / acc))));
		
		if (pmotor->new_arr >= 0xFFFF)
			pmotor->new_arr = 0xffff;
		
		/* ���ٵ�Ŀ���ٶ�ʱ�ľ��� */
		pmotor->acc_dist = (speed * speed) / ((PAN_ALPHA_x200 * acc));
		/* ����ﵽ����ٶ�С��0.5�������ǽ���������Ϊ0
		   ��ʵ�����Ǳ����ƶ�����һ�����ܴﵽ��Ҫ���ٶ� */
		if (pmotor->acc_dist == 0) {
			pmotor->acc_dist = 1;
		}
		
		/* �ﵽ��Ҫ����ʱ�Ĳ��� */
		pmotor->acc_dist_lim = (tar_pos * dec) / (acc + dec);
		/* ���Ǳ����������1�����ܲ��ܿ�ʼ���� */
		if (pmotor->acc_dist_lim == 0) {
			pmotor->acc_dist_lim = 1;
		}
		
		/* δ�ﵽ����ٶ� */
		if (pmotor->acc_dist_lim <= pmotor->acc_dist) {
			/* �����˶� */
			if (tar_pos > pmotor->cur_pos) {
				TIL_FORWARD_DIR();
				pmotor->pos_dir = 1;
			}
			/* �����˶� */
			else {
				TIL_REVERSE_DIR();
				pmotor->pos_dir = -1;
			}
			
			pmotor->acc_pos = pmotor->cur_pos + pmotor->pos_dir * pmotor->acc_dist_lim;
			pmotor->dec_dist = tar_pos - pmotor->acc_dist_lim * pmotor->pos_dir;
		}
		/* �ﵽ����ٶ� */
		else {
			/* �����˶� */
			if (tar_pos > pmotor->cur_pos) {
				TIL_FORWARD_DIR();
				pmotor->pos_dir = 1;
			}
			/* �����˶� */
			else {
				TIL_REVERSE_DIR();
				pmotor->pos_dir = -1;
			}
			
			pmotor->acc_pos = pmotor->cur_pos + pmotor->pos_dir * pmotor->acc_dist;
			pmotor->dec_dist = (pmotor->pos_dir * pmotor->acc_dist * acc) / dec;
		}
		
		/* ��ֻʣ��һ�����Ǳ������ */
		if (pmotor->dec_dist == 0) {
			pmotor->dec_dist = 1;
		}
		
		/* ��ʼ����ʱ��λ�� */
		pmotor->dec_pos = tar_pos - pmotor->dec_dist;
		
		/* �������ٶȺ��������ǾͲ���Ҫ���м����˶� */
		if (pmotor->new_arr <= pmotor->min_arr) {
			pmotor->new_arr = pmotor->min_arr;
		}
		
		pmotor->acc_count = 0;
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM4, ENABLE);
	}
	
    return 0;
}

