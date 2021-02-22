/***************************************************************************************************
* FILE: tmc22xx_motor_app.h

* DESCRIPTION:  

* CREATED: 2020/09/21, by taylor
***************************************************************************************************/
#ifndef __TMC22XX_M0TOR_APP_H__
#define __TMC22XX_M0TOR_APP_H__

/*************************************************************************************
* INCLUDE
*************************************************************************************/
#include "tmc22xx.h"
/*************************************************************************************
* DEFINE
*************************************************************************************/

///****************************************************
//* Description: GCONF CONFIG
//****************************************************/
//#define I_SCALE_ANALOG(x)	(x<<0)	// 1: use external Vref; 	0: use internal Vref
//#define INTERNAL_RSENSE		(1<<1)	// 1: use internal; 		0: use external
//#define EN_SPREADCYCLE(x)	(x<<2)	// 1: use spreadcycle; 		0: use stealthchop
//#define SHAFT(x)			(x<<3)	// 1: inverse motor direction
//#define INDEX_OTPW			(0<<4)
//#define INDEX_STEP			(0<<5)	// 1: use internal pulse
//#define PDN_DISABLE			(1<<6)
//#define MSTEP_REG_SELECT	(0<<7)	// 0: Microstep resolution selected by pins MS1, MS2 
//#define MULTISTEP_FILT(x)	(x<<8)	// 0: No filtering of STEP pulses 
//#define TEST_MODE			(0<<9)
//
//#define GCONFIG(x, y, z, m)	TEST_MODE | MULTISTEP_FILT(x) | MSTEP_REG_SELECT | \
//								PDN_DISABLE | INDEX_STEP | INDEX_OTPW | SHAFT(y) | \
//								EN_SPREADCYCLE(z) | INTERNAL_RSENSE | I_SCALE_ANALOG(m)
//
///****************************************************
//* Description: GSTAT CONFIG
//****************************************************/
//#define RESET				(1<<0)
//#define DRV_ERR				(1<<1)
//#define UV_CP				(1<<2)
//
//#define GSTATCONFIG			UV_CP | DRV_ERR | RESET
//
///****************************************************
//* Description: SLAVECONF CONFIG
//****************************************************/
//#define SEND_DELAY_xx8BIT(x)		(x<<8)
//
//#define SLAVECONFIG_SEND_DELAY(x)	SEND_DELAY_xx8BIT(x)
//
///****************************************************
//* Description: OTP_PROG CONFIG
//****************************************************/
//#define OTPBIT(x)			(x<<0)		// x: 0 ~ 7
//#define OTPBYTE(x)			(x<<4)		// x: 0 ~ 2
//#define OTPMAGIC			(0xBD<<8)	// Set to 0xbd to enable programming
//
//#define OTP_PROGCONFIG(x, y)	OTPMAGIC | OTPBYTE(x) | OTPBIT(y)
//
///****************************************************
//* Description: FACTORY_CONF CONFIG
//****************************************************/
//#define FCLKTRIM(x)			(x<<0)
//#define OTTRIM(x)			(x<<8)
//
//#define FACTORY_CONFIG(x, y)	OTTRIM(x) | FCLKTRIM(y)
//
///****************************************************
//* Description: IHOLD_IRUN CONFIG
//****************************************************/
//#define IHOLD(x)			(x<<0)		// x: 0--31  Standstill current  recommend: 16--31
//#define IRUN(x)				(x<<8)		// x: 0--31  Motor run current  recommend: 16--31
//#define IHOLD_DELAY(x)		(x<<16)		// x: 0: instant power down; 1--15: 
//
//#define IHOLD_IRUN_CONFIG(x, y, z)	IHOLD_DELAY(x) | IRUN(y) | IHOLD(z)
//
///****************************************************
//* Description: TPOWER DOWN CONFIG 
//* 停止到电机电流断电的延时时间： x * (2^8-1) * tclk
//****************************************************/
//#define TPOWER_DOWN_CONFIG(x)		(x)			// x: 0--(2^8-1)
//
///****************************************************
//* Description: TPWMTHRS CONFIG
//****************************************************/
//#define TPWMTHRS_CONFIG(x)			(x)			// x: 0--(2^20-1) set upper velocity
//
///****************************************************
//* Description: VACTUAL CONFIG
//****************************************************/
//#define VACTUAL_CONFIG(x)			(x)			// moving the motor by UART control; 0: Normal operation, driver reacts to STEP input
//
///****************************************************
//* Description: CHOPCONF CONFIG
//****************************************************/
//#define DISS2VS				(0)			// 0: Short protection low side is on
//										// 1: Short protection low side is disabled
//#define DISS2G				(0)			// 0: Short to GND protection is on
//										// 1: Short to GND protection is disabled
//#define DEDGE				(0<<29)		//
//#define INTPOL				(1<<28)		// 256 microsteps
//#define MRES(x)				(x<<24)		// x: 0: 256;	1: 128;
//										// 	  2: 64;	3: 32;
//										//	  4: 16; 	5: 8;
//										// 	  6: 4; 	7:2; 	8: fullstep
//#define VSENSE				(0<<17)		// 1: High sensitivity, low sense resistor voltage; 0: low sensitivity, high ...
//#define TBL(x)				(x<<15)		// x: 0--3  recommend 0 or 1
//#define HEND(x)				(x<<7)		// recommend 5
//#define HSTART(x)			(x<<4)		// effective HEND+HSTRT <= 16
//#define TOFF(x)				(x<<0)		// 12MHz:recommend 3; 16MHz:recommend 4 or 5; Driver enable
//
//#define CHOPCONFIG(x, y, z, m, n)	DEDGE | INTPOL | MRES(x) | VSENSE | TBL(y) | HEND(z) | HSTART(m) | TOFF(n)
//
///****************************************************
//* Description: PWMCONF CONFIG
//****************************************************/
//#define PWM_LIM(x)			(x<<28)		// recommend 12  reduce the value to yield a lower current jerk
//#define PWM_REG(x)			(x<<24)		// x: 1--15
//#define FREEWHEEL(x)		(x<<20)		// x: 0--3  0: normal operation
//#define PWM_AUTOGRAD(x)		(x<<19)		// 1: automatic tuning(only pwm_autoscale = 1); 0: fixed 
//#define PWM_AUTOSCALE(x)	(x<<18)		// 1: automatic; 0:
//#define PWM_FREQ(x)			(x<<16)		// x: 0--3
//#define PWM_GRAD(x)			(x<<8)		// 
//#define PWM_OFS(x)			(x<<0)		//
//
//#define PWMCONFIG(x, y, z, m, n, o, p, q)		PWM_LIM(x) | PWM_REG(y) | FREEWHEEL(z) | \
//												PWM_AUTOGRAD(m) | PWM_AUTOSCALE(n) | PWM_FREQ(o) | \
//												PWM_GRAD(p) | PWM_OFS(q)

/*************************************************************************************
* GLOBAL
*************************************************************************************/

/*************************************************************************************
* FUNCTION DECLARATION
*************************************************************************************/
const char * TMC22xx_getVersion(void);

/* 基本控制 */
uint8_t TMC22xx_MotorEnable(uint8_t motor, uint8_t new_state);
void TMC22xx_SetStopPower(uint8_t motor, uint8_t percent);
void TMC22xx_SetRunPower(uint8_t motor, uint8_t percent);
void TMC22xx_SetStopCutOff(uint8_t motor, uint8_t onoff);
void TMC22xx_SetDirInv(uint8_t motor, uint8_t inv);
void TMC22xx_SetFrequency(uint8_t motor, uint32_t freq);

/* 状态 */
int32_t TMC22xx_MotorIsRunning(uint8_t motor);
int32_t TMC22xx_GetMotorCurSpeed(uint8_t motor);
int32_t TMC22xx_GetMotorPosition(uint8_t motor);
int32_t TMC22xx_SetMotorPosition(uint8_t motor, int32_t new_pos);

/* 运行控制 */
int32_t TMC22xx_MotorBrake(uint8_t motor, int16_t dec);
int32_t TMC22xx_CalcuRunPara(struct tmc22xx_motor_para *pmotor);
int32_t TMC22xx_MotorGoto(uint8_t motor, int32_t tar_pos, uint16_t speed, int16_t acc, int16_t dec);

#endif /* __TMC22XX_M0TOR_APP_H__ */
