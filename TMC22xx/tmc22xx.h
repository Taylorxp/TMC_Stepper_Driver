/***************************************************************************************************
* FILE: tmc22xx.h

* DESCRIPTION:  

* CREATED: 2020/09/17, by taylor
***************************************************************************************************/
#ifndef __TMC22XX_H__
#define __TMC22XX_H__

/*************************************************************************************
* INCLUDE
*************************************************************************************/
#include "bsp.h"
#include "system.h"

/*************************************************************************************
* DEFINE
*************************************************************************************/
/* 内外部时钟选择 */
#define USE_EXTERN_CLK

/* 使用完全跳线 */
#define USE_JUMPER_WIRE			1

/* 使用部分跳线 */
#define USE_PARTIAL_JUMPER_WIRE	0

/* 设置占空比，周期6，设置为3，即占空比为50% */
#define PWM_DUTY_CYCLE			3

/* 波特率设置 */
#define TMC22xx_BAUD			460800

/* SYNC */
#define SYNC_DATA				0x05

/* SLAVE ADDR */
#define SLAVE_ADDR				0x00

/* WRITE FIXED REGISTER HIGH */
#define WRITE_FIXED_REGISTER_HIGH		0x80

/* READ REQUSET FIXED REGISTER HIGH */
#define READ_REQ_FIXED_REGISTER_HIGH	0x00

/* READ REPLY FIXED REGISTER HIGH */
#define READ_REPLY_FIXED_REGISTER_HIGH	0x00

/* READ MASTER ADDR */
#define READ_MASTER_ADDR				0xFF

/*********************************************************************************
* GENERAL CONFIGURATION REGISTER 
*********************************************************************************/
#define REG_GCONF					0x00
#define REG_GSTAT					0x01
#define REG_IFCNT					0x02
#define REG_SLAVECONF				0x03
#define REG_OTP_PROG				0x04
#define REG_OTP_READ				0x05
#define REG_IOIN					0x06
#define REG_FACTORY_CONF			0x07

/*********************************************************************************
* VELOCITY DEPENDENT CONTROL REGISTER
*********************************************************************************/
#define REG_IHOLD_IRUN				0x10
#define REG_TPOWERDOWN				0x11
#define REG_TSTEP					0x12
#define REG_TPWMTHRS				0x13
#define REG_VACTUAL					0x22

/*********************************************************************************
* MICROSTEPPING CONTROL REGISTER
*********************************************************************************/
#define REG_MSCNT					0x6A
#define REG_MSCURACT				0x6B

/*********************************************************************************
* DRIVER REGISTER
*********************************************************************************/
#define REG_CHOPCONF				0x6C
#define REG_DRV_STATUS				0x6F
#define REG_PWMCONF					0x70
#define REG_PWM_SCALE				0x71
#define REG_PWM_AUTO				0x72

// GCONF
#define I_SCALE_ANALOG_bp	0
#define INTERNAL_RSENSE_bp	1
#define EN_SPREADCYCLE_bp	2
#define SHAFT_bp			3
#define INDEX_OTPW_bp		4
#define INDEX_STEP_bp		5
#define PDN_DISABLE_bp		6
#define MSTEP_REG_SELECT_bp	7
#define MULTISTEP_FILT_bp	8
#define I_SCALE_ANALOG_bm	(1ul << 0)		//(0b1ul)
#define INTERNAL_RSENSE_bm	(1ul << 1)		//(0b10ul)
#define EN_SPREADCYCLE_bm	(1ul << 2)		//(0b100ul)
#define SHAFT_bm			(1ul << 3)		//(0b1000ul)
#define INDEX_OTPW_bm		(1ul << 4)		//(0b10000ul)
#define INDEX_STEP_bm		(1ul << 5)		//(0b100000ul)
#define PDN_DISABLE_bm		(1ul << 6)		//(0b1000000ul)
#define MSTEP_REG_SELECT_bm	(1ul << 7)		//(0b10000000ul)
#define MULTISTEP_FILT_bm	(1ul << 8)		//(0b100000000ul)
// GSTAT
#define RESET_bp 			0
#define DRV_ERR_bp			1
#define UV_CP_bp			2
#define RESET_bm 			(1ul << 0)		//(0b1ul)
#define DRV_ERR_bm			(1ul << 1)		//(0b10ul)
#define UV_CP_bm			(1ul << 2)		//(0b100ul)
// IFCNT
#define IFCNT_bp			0
#define IFCNT_bm			0xFFul
// SLAVECONF
#define SLAVECONF_bp		0
#define SLAVECONF_bm		0xF00ul
#define SENDDELAY_bp		8
#define SENDDELAY_bm		0xF00ul
// OTP_PROG
#define OTPBIT_bp			0
#define OTPBYTE_bp			4
#define OTPMAGIC_bp			8
#define OTPBIT_bm			(7ul << 0)		//(0b111ul)
#define OTPBYTE_bm			(3ul << 4)		//(0b110000ul)
#define OTPMAGIC_bm			(0xFFul << 8)	//(0b1111111100000000ul)
// OTP_READ
#define OTP0_bp				0
#define OTP1_bp				8
#define OTP2_bp			   16
#define OTP0_bm				0xFFul
#define OTP1_bm				0xFF00ul
#define OTP2_bm			    0xFF0000ul
// IOIN
#define ENN_bp				0
#define MS1_bp				2
#define MS2_bp				3
#define DIAG_bp				4
#define PDN_UART_bp			6
#define STEP_bp 			7
#define SEL_A_bp 			8
#define DIR_bp				9
#define VERSION_bp		   24
#define ENN_bm				(1ul << 0)		//(0b1ul)
#define MS1_bm				(1ul << 2)		//(0b100ul)
#define MS2_bm				(1ul << 3)		//(0b1000ul)
#define DIAG_bm				(1ul << 4)		//(0b10000ul)
#define PDN_UART_bm			(1ul << 6)		//(0b1000000ul)
#define STEP_bm 			(1ul << 7)		//(0b10000000ul)
#define SEL_A_bm 			(1ul << 8)		//(0b100000000ul)
#define DIR_bm				(1ul << 9)		//(0b1000000000ul)
#define VERSION_bm			(0xFF000000ul)
// FACTORY_CONF
#define FCLKTRIM_bp			0
#define OTTRIM_bp			8
#define FCLKTRIM_bm			0x1Ful
#define OTTRIM_bm			0x300ul
// IHOLD_IRUN
#define IHOLD_bp 			0
#define IRUN_bp				8
#define IHOLDDELAY_bp	   16
#define IHOLD_bm 			0x1Ful
#define IRUN_bm				0x1F00ul
#define IHOLDDELAY_bm	    0xF0000ul
// TPOWERDOWN
#define TPOWERDOWN_bp		0
#define TPOWERDOWN_bm		0xFFul
// MSCURACT
#define CUR_A_bp			0
#define CUR_B_bp		   16
#define CUR_A_bm			0x1FFul
#define CUR_B_bm			0x1FF0000ul
// PWM_SCALE
#define PWM_SCALE_SUM_bp	0
#define PWM_SCALE_AUTO_bp  16
#define PWM_SCALE_SUM_bm	0xFFul
#define PWM_SCALE_AUTO_bm 	0x1FF0000ul
// PWM_AUTO
#define PWM_OFS_AUTO_bp		0
#define PWM_GRAD_AUTO_bp   16
#define PWM_OFS_AUTO_bm		0xFFul
#define PWM_GRAD_AUTO_bm 	0xFF0000ul
// OTP_READ
#define OTP_FCLKTRIM_bp		0
#define OTP_OTTRIM_bp		5
#define OTP_INTERNALRSENSE_bp 6
#define OTP_TBL_bp 			7
#define OTP_PWM_GRAD_bp		8
#define OTP_PWM_AUTOGRAD_bp 12
#define OTP_CHOPCONF_bp	   12
#define OTP_TPWMTHRS_bp	   13
#define OTP_PWM_OFS_bp	   16
#define OTP_PWM_REG_bp	   17
#define OTP_OTP_PWM_FREQ_bp 18
#define OTP_IHOLDDELAY_bp  19
#define OTP_IHOLD_bp 	   21
#define OTP_OTP_EN_SPREADCYCLE 23
// CHOPCONF
#define TOFF_bp				0
#define HSTRT_bp			4
#define HEND_bp				7
#define TBL_bp 			   15
#define VSENSE_bp		   17
#define MRES_bp 		   24
#define INTPOL_bp		   28
#define DEDGE_bp		   29
#define DISS2G_bp		   30
#define DISS2VS_bp		   31
#define TOFF_bm			  	0xFul
#define HSTRT_bm		 	0x70ul
#define HEND_bm				0x780ul
#define TBL_bm		  		0x18000ul
#define VSENSE_bm	  		0x20000ul
#define MRES_bm				0xF000000ul
#define INTPOL_bm  			0x10000000ul
#define DEDGE_bm   			0x20000000ul
#define DISS2G_bm  			0x40000000ul
#define DISS2VS_bm 			0x80000000ul
// PWMCONF
#define PWM_OFS_bp 			0
#define PWM_GRAD_bp 		8
#define PWM_FREQ_bp 	   16
#define PWM_AUTOSCALE_bp   18
#define PWM_AUTOGRAD_bp    19
#define FREEWHEEL_bp 	   20
#define PWM_REG_bp 		   24
#define PWM_LIM_bp		   28
#define PWM_OFS_bm 			0xFFul
#define PWM_GRAD_bm 		0xFF00ul
#define PWM_FREQ_bm 	    0x30000ul
#define PWM_AUTOSCALE_bm    0x40000ul
#define PWM_AUTOGRAD_bm     0x80000ul
#define FREEWHEEL_bm 	    0x300000ul
#define PWM_REG_bm 		    0xF000000ul
#define PWM_LIM_bm		    0xF0000000ul
// DRV_STATUS
#define OTPW_bp 			0
#define OT_bp 				1
#define S2GA_bp				2
#define S2GB_bp				3
#define S2VSA_bp			4
#define S2VSB_bp			5
#define OLA_bp 				6
#define OLB_bp 				7
#define T120_bp				8
#define T143_bp				9
#define T150_bp			   10
#define T157_bp			   11
#define CS_ACTUAL_bp	   16
#define STEALTH_bp		   30
#define STST_bp			   31
#define OTPW_bm 			(1ul << 0)		//(0b1ul)
#define OT_bm 				(1ul << 1)		//(0b10ul)
#define S2GA_bm				(1ul << 2)		//(0b100ul)
#define S2GB_bm				(1ul << 3)		//(0b1000ul)
#define S2VSA_bm			(1ul << 4)		//(0b10000ul)
#define S2VSB_bm			(1ul << 5)		//(0b100000ul)
#define OLA_bm 				(1ul << 6)		//(0b1000000ul)
#define OLB_bm 				(1ul << 7)		//(0b10000000ul)
#define T120_bm				(1ul << 8)		//(0b100000000ul)
#define T143_bm				(1ul << 9)		//(0b1000000000ul)
#define T150_bm			    (1ul << 10)		//(0b10000000000ul)
#define T157_bm			    (1ul << 11)		//(0b100000000000ul)
#define CS_ACTUAL_bm	    0x1F0000ul
#define STEALTH_bm		    0x40000000ul
#define STST_bm			    0x80000000ul



/**********************************************************************************
* 定时器频率： freq = 96M / (15 + 1) = 6M
* 步进电机步距角： step_angle = 0.9
* 步进电机运行一圈的步数： steps_one_circle = 360 / 0.9 = 400
* 细分(当前硬件配置)： res = 16
* 细分后一圈运行的步数： micro_steps_one_circle = steps_one_circle * res = 400 * 16 = 6400
* 
**********************************************************************************/
#define __PI						(3.14159f)
#define FREQ						(6 * 1000000ul)
#define PAN_FREQ					FREQ
#define TIL_FREQ					FREQ

#define PAN_STEP_ANGLE				(0.9f)
#define PAN_RES						(16)
#define PAN_STEPS_ONE_CIRCLE		((uint16_t)(360.0f / PAN_STEP_ANGLE))
#define PAN_MICRO_STEPS_ONE_CIRCLE	(PAN_RES * PAN_STEPS_ONE_CIRCLE)

#define TIL_STEP_ANGLE				(0.9f)
#define TIL_RES						(16)
#define TIL_STEPS_ONE_CIRCLE		((uint16_t)(360.0f / TIL_STEP_ANGLE))
#define TIL_MICRO_STEPS_ONE_CIRCLE	(TIL_RES * TIL_STEPS_ONE_CIRCLE)

/**********************************************************************************
* 加速度、减速度、速度扩大10倍，以减小MCU计算负担
**********************************************************************************/
#define PAN_ALPHA					((2 * __PI) / PAN_MICRO_STEPS_ONE_CIRCLE)
#define PAN_ALPHA_FREQ_x10			((uint32_t)(PAN_ALPHA * PAN_FREQ * 10))
#define PAN_FREQ_CONSTANT			((uint32_t)(PAN_FREQ * 0.676f))
#define PAN_2alpha_x1000000			((uint32_t)(2 * PAN_ALPHA))
#define PAN_ALPHA_x200				((uint32_t)(PAN_ALPHA * 2 * 10000))

#define TIL_ALPHA					((2 * __PI) / TIL_MICRO_STEPS_ONE_CIRCLE)
#define TIL_ALPHA_FREQ_x10			((uint32_t)(TIL_ALPHA * TIL_FREQ * 10))
#define TIL_FREQ_CONSTANT			((uint32_t)(TIL_FREQ * 0.676f) / 10)
#define TIL_ALPHA_CONSTANT			((uint64_t)(2 * TIL_ALPHA * 100000))
#define TIL_ALPHA_x200				((uint32_t)(TIL_ALPHA * 2 * 100))

/**********************************************************************************
* high & low
**********************************************************************************/
#define HIGH				1
#define LOW					0

/*********************************************************************************
* 字节数
**********************************************************************************/
#define BYTE_NUM				8
#define WIRTE_BYTE_NUM			BYTE_NUM
#define READ_BYTE_NUM			BYTE_NUM

/*********************************************************************************
* 电机
**********************************************************************************/
#define TMC22XX_MOTOR1			0
#define TMC22XX_MOTOR2			1
#define TMC22XX_PAN				TMC22XX_MOTOR1
#define TMC22XX_TIL				TMC22XX_MOTOR2

/*********************************************************************************
* 正弦表数量
**********************************************************************************/
#define SINEWAVE_NUM			180

/*********************************************************************************
* 重装载值
**********************************************************************************/
#define MAX_PERIOD				3200

/*********************************************************************************
* motor = 1, pan
* motor = 0, til
**********************************************************************************/
#define TMC22xx_SendData(motor, nArr, nSize)	((!motor) ? (UART5_SendArray(nArr, nSize)) : UART4_DmaSend(nArr, nSize))

/*************************************************************************************
* EXTERN
*************************************************************************************/
typedef struct tmc22xx_recv_s
{
	uint8_t pan[8];
	uint8_t til[8];
	uint8_t pan_flag;
	uint8_t til_flag;
} TMC22xx_RECV_ST;

typedef struct tmc22xx_s
{
	uint16_t 	bytesWritten;
	uint16_t 	replyDelay;
	uint16_t 	mA_val;
	float 		Rsense;
	uint8_t 	flag_otpw;
	uint8_t 	write_only;
} TMC22xx_S;

/****************************************************
* Description: Shadow registers
* 			   Default values assume no changes in OTP
****************************************************/
typedef struct shadow_register_s
{
	uint32_t 	GCONF_sr;
	uint32_t	GSTAT_sr;
	uint32_t	SLAVECONF_sr;
	uint32_t	OTP_PROG_sr;
	uint32_t	OTP_READ_sr;
	uint32_t	FACTORY_CONF_sr;
	uint32_t	IHOLD_IRUN_sr;
	uint32_t	TPOWERDOWN_sr;
	uint32_t	TPWMTHRS_sr;
	uint32_t	VACTUAL_sr;
	uint32_t	CHOPCONF_sr;
	uint32_t	PWMCONF_sr;
	uint32_t	tmp_sr;
} SHADOW_REGISTER_S;

typedef struct tmc22xx_motor_shadow_register_s
{
	SHADOW_REGISTER_S stShadowReg_Motor1;	//PAN
	SHADOW_REGISTER_S stShadowReg_Motor2;	//TIL
} TMC22xx_MOTOR_SHADOW_REGISTER_S;

/****************************************************
* Description: 电机参数
****************************************************/
typedef struct tmc22xx_motor_para
{
    int8_t speed_dir;       //运转方向
	int8_t pos_dir;			//位置方向
    
	int32_t psc;			//控制加减速快慢
    int32_t cur_pos;        //当前位置
    int32_t tar_pos;        //目标位置
    int32_t acc_pos;        //加速结束位置
    int32_t dec_pos;        //开始减速位置
	int32_t acc_dist;		//加速距离
	int32_t dec_dist;		//减速距离
	int32_t max_pos;		//最大位置
	int32_t min_pos;		//最小位置
	
	int32_t acc_dist_lim;	//达到减速时的步数
	
	uint32_t min_arr;		//最小的重装载值，即目标最大速度
	uint32_t new_arr;		//最新的重装载值，即当前速度
	
	int32_t acc_count;		//加速或减速计数器
    
    int32_t cur_speed;       //当前速度
    int32_t tar_speed;       //目标速度
    int32_t dec_off;        //减速偏差
    
    int32_t acc;             //加速度1
    int32_t dec;             //加速度2
} TMC22xx_MOTOR_PARA_S;

/* TMC22xx驱动参数 */
typedef struct tmc22xx_motor
{
    TMC22xx_MOTOR_PARA_S para_motor1;    //电机1参数，水平
    TMC22xx_MOTOR_PARA_S para_motor2;    //电机2参数，垂直
} TMC22xx_MOTOR_S;

/* TMC22xx操作接口 */
typedef struct tmc22xx_motor_ops
{
	void (*start_pwm_clk)(void);
	void (*end_pwm_clk)(void);
} TMC22xx_MOTOR_OPS_S;

extern TMC22xx_MOTOR_S stTmc22xxMotor;
extern TMC22xx_MOTOR_OPS_S stTmc22xxMotorOps;

extern SHADOW_REGISTER_S stShadow_Reg;
extern TMC22xx_MOTOR_SHADOW_REGISTER_S stTmc22xxMotorShadowReg;
extern TMC22xx_RECV_ST stTMC22xx_Recv;
extern TMC22xx_S 		stTMC22xx;

extern uint16_t Pan_SinWave_Tab[SINEWAVE_NUM * 2];
extern uint16_t Til_SinWave_Tab[SINEWAVE_NUM * 2];
extern uint16_t speed_lib[144];
extern uint16_t phase_lib[360];

/*************************************************************************************
* FUNCTION DECLARATION
*************************************************************************************/
void TMC22xx_Init(uint8_t motor);
void TMC22xx_Read_ProcessHandler(void);
void TMC22xx_TestRun(void);

void TMC22xx_SetRmsCurrent(uint8_t motor, uint16_t mA, float multiplier, float RS);
uint16_t TMC22xx_GetRmsCurrent(uint8_t motor);
void TMC22xx_SetMicrosteps(uint8_t motor, uint16_t microsteps);
uint16_t TMC22xx_GetMicrosteps(uint8_t motor);
void TMC22xx_SetCurrent(uint8_t motor, uint16_t mA, float multiplier, float RS);
uint16_t TMC22xx_GetCurrent(uint8_t motor);
uint8_t TMC22xx_CheckOT(uint8_t motor);
uint8_t TMC22xx_GetOTPW(uint8_t motor);
void TMC22xx_ClearOtpw(uint8_t motor);
uint8_t TMC22xx_IsEnabled(uint8_t motor);
void TMC22xx_ResetReg_Push(uint8_t motor);
uint8_t TMC22xx_Test_Connection(uint8_t motor);

// RW: GCONF
void TMC22xx_Set_GCONF(uint8_t motor, uint32_t input);
void GCONF_Set_I_scale_analog(uint8_t motor, uint8_t B);
void GCONF_Set_internal_Rsense(uint8_t motor, uint8_t B);
void GCONF_Set_en_spreadCycle(uint8_t motor, uint8_t B);
void GCONF_Set_shaft(uint8_t motor, uint8_t B);
void GCONF_Set_index_otpw(uint8_t motor, uint8_t B);
void GCONF_Set_index_step(uint8_t motor, uint8_t B);
void GCONF_Set_pdn_disable(uint8_t motor, uint8_t B);
void GCONF_Set_mstep_reg_select(uint8_t motor, uint8_t B);
void GCONF_Set_multistep_filt(uint8_t motor, uint8_t B);
uint8_t TMC22xx_Get_GCONF(uint8_t motor, uint32_t *data);
uint8_t GCONF_Get_I_scale_analog(uint8_t motor);
uint8_t GCONF_Get_internal_Rsense(uint8_t motor);
uint8_t GCONF_Get_en_spreadCycle(uint8_t motor);
uint8_t GCONF_Get_shaft(uint8_t motor);
uint8_t GCONF_Get_index_otpw(uint8_t motor);
uint8_t GCONF_Get_index_step(uint8_t motor);
uint8_t GCONF_Get_pdn_disable(uint8_t motor);
uint8_t GCONF_Get_mstep_reg_select(uint8_t motor);
uint8_t GCONF_Get_multistep_filt(uint8_t motor);
// R+WC: GSTAT
void TMC22xx_Set_GSTAT(uint8_t motor, uint32_t input);
void GSTAT_Set_reset(uint8_t motor, uint8_t B);
void GSTAT_Set_drv_err(uint8_t motor, uint8_t B);
void GSTAT_Set_uv_cp(uint8_t motor, uint8_t B);
uint8_t TMC22xx_Get_GSTAT(uint8_t motor, uint32_t *data);
//uint8_t TMC22xx_Get_GSTAT();
uint8_t GSTAT_Get_reset(uint8_t motor);
uint8_t GSTAT_Get_drv_err(uint8_t motor);
uint8_t GSTAT_Get_uv_cp(uint8_t motor);
// R: IFCNT
uint8_t TMC22xx_Get_IFCNT(uint8_t motor, uint32_t *data);
// W: SLAVECONF
void TMC22xx_Set_SLAVECONF(uint8_t motor, uint32_t input);
uint8_t TMC22xx_Get_SLAVECONF(uint8_t motor, uint32_t *data);
void SLAVECONF_Set_senddelay(uint8_t motor, uint8_t B);
uint8_t SLAVECONF_Get_senddelay();
// W: OTP_PROG
void TMC22xx_Set_OTP_PROG(uint8_t motor, uint32_t input);
// R: OTP_READ
uint8_t TMC22xx_Get_OTP_READ(uint8_t motor, uint32_t *data);
// R: IOIN
uint8_t TMC22xx_Get_IOIN(uint8_t motor, uint32_t *data);
uint8_t IOIN_Get_enn(uint8_t motor);
uint8_t IOIN_Get_ms1(uint8_t motor);
uint8_t IOIN_Get_ms2(uint8_t motor);
uint8_t IOIN_Get_diag(uint8_t motor);
uint8_t IOIN_Get_pdn_uart(uint8_t motor);
uint8_t IOIN_Get_step(uint8_t motor);
uint8_t IOIN_Get_sel_a(uint8_t motor);
uint8_t IOIN_Get_dir(uint8_t motor);
uint8_t IOIN_Get_version(uint8_t motor);
// RW: FACTORY_CONF
void TMC22xx_Set_FACTORY_CONF(uint8_t motor, uint32_t input);
uint8_t TMC22xx_Get_FACTORY_CONF(uint8_t motor, uint32_t *data);
void FACTORY_CONF_Set_fclktrim(uint8_t motor, uint8_t B);
void FACTORY_CONF_Set_ottrim(uint8_t motor, uint8_t B);
uint8_t FACTORY_CONF_Get_fclktrim(uint8_t motor);
uint8_t FACTORY_CONF_Get_ottrim(uint8_t motor);
// W: IHOLD_IRUN
void TMC22xx_Set_IHOLD_IRUN(uint8_t motor, uint32_t input);
uint8_t TMC22xx_Get_IHOLD_IRUN(uint8_t motor, uint32_t *data);
void IHOLD_IRUN_Set_ihold(uint8_t motor, uint8_t B);
void IHOLD_IRUN_Set_irun(uint8_t motor, uint8_t B);
void IHOLD_IRUN_Set_iholddelay(uint8_t motor, uint8_t B);
uint8_t IHOLD_IRUN_Get_ihold(uint8_t motor);
uint8_t IHOLD_IRUN_Get_irun(uint8_t motor);
uint8_t IHOLD_IRUN_Get_iholddelay(uint8_t motor);
// W: TPOWERDOWN
void TMC22xx_Set_TPOWERDOWN(uint8_t motor, uint32_t input);
uint8_t TMC22xx_Get_TPOWERDOWN(uint8_t motor, uint32_t *data);
// R: TSTEP
uint8_t TMC22xx_Get_TSTEP(uint8_t motor, uint32_t *data);
// W: TPWMTHRS
void TMC22xx_Set_TPWMTHRS(uint8_t motor, uint32_t input);
uint8_t TMC22xx_Get_TPWMTHRS(uint8_t motor, uint32_t *data);
// W: VACTUAL
void TMC22xx_Set_VACTUAL(uint8_t motor, uint32_t input);
uint8_t TMC22xx_Get_VACTUAL(uint8_t motor, uint32_t *data);
// R: MSCNT
uint8_t TMC22xx_Get_MSCNT(uint8_t motor, uint32_t *data);
// R: MSCURACT
uint8_t TMC22xx_Get_MSCURACT(uint8_t motor, uint32_t *data);
uint16_t MSCURACT_Get_cur_a(uint8_t motor);
uint16_t MSCURACT_Get_cur_b(uint8_t motor);
// RW: CHOPCONF
void TMC22xx_Set_CHOPCONF(uint8_t motor, uint32_t input);
void CHOPCONF_Set_toff(uint8_t motor, uint8_t B);
void CHOPCONF_Set_hstrt(uint8_t motor, uint8_t B);
void CHOPCONF_Set_hysteresis_start(uint8_t motor, uint8_t value);
void CHOPCONF_Set_hend(uint8_t motor, uint8_t B);
void CHOPCONF_Set_hysteresis_end(uint8_t motor, int8_t value);
void CHOPCONF_Set_tbl(uint8_t motor, uint8_t B);
void CHOPCONF_Set_blank_time(uint8_t motor, uint8_t B);
void CHOPCONF_Set_vsense(uint8_t motor, uint8_t B);
void CHOPCONF_Set_mres(uint8_t motor, uint8_t B);
void CHOPCONF_Set_intpol(uint8_t motor, uint8_t B);
void CHOPCONF_Set_dedge(uint8_t motor, uint8_t B);
void CHOPCONF_Set_diss2g(uint8_t motor, uint8_t B);
void CHOPCONF_Set_diss2vs(uint8_t motor, uint8_t B);
uint8_t TMC22xx_Get_CHOPCONF(uint8_t motor, uint32_t *data);
uint8_t CHOPCONF_Get_toff(uint8_t motor);
uint8_t CHOPCONF_Get_hstrt(uint8_t motor);
uint8_t CHOPCONF_Get_hysteresis_start(uint8_t motor);
uint8_t CHOPCONF_Get_hend(uint8_t motor);
int8_t CHOPCONF_Get_hysteresis_end(uint8_t motor);
uint8_t CHOPCONF_Get_tbl(uint8_t motor);
uint8_t CHOPCONF_Get_blank_time(uint8_t motor);
uint8_t CHOPCONF_Get_vsense(uint8_t motor);
uint8_t CHOPCONF_Get_mres(uint8_t motor);
uint8_t CHOPCONF_Get_intpol(uint8_t motor);
uint8_t CHOPCONF_Get_dedge(uint8_t motor);
uint8_t CHOPCONF_Get_diss2g(uint8_t motor);
uint8_t CHOPCONF_Get_diss2vs(uint8_t motor);
// R: DRV_STATUS
uint8_t TMC22xx_Get_DRV_STATUS(uint8_t motor, uint32_t *data);
//uint32_t DRV_STATUS_Get_DRV_STATUS();
uint8_t DRV_STATUS_Get_otpw(uint8_t motor);
uint8_t DRV_STATUS_Get_ot(uint8_t motor);
uint8_t DRV_STATUS_Get_s2ga(uint8_t motor);
uint8_t DRV_STATUS_Get_s2gb(uint8_t motor);
uint8_t DRV_STATUS_Get_s2vsa(uint8_t motor);
uint8_t DRV_STATUS_Get_s2vsb(uint8_t motor);
uint8_t DRV_STATUS_Get_ola(uint8_t motor);
uint8_t DRV_STATUS_Get_olb(uint8_t motor);
uint8_t DRV_STATUS_Get_t120(uint8_t motor);
uint8_t DRV_STATUS_Get_t143(uint8_t motor);
uint8_t DRV_STATUS_Get_t150(uint8_t motor);
uint8_t DRV_STATUS_Get_t157(uint8_t motor);
uint16_t DRV_STATUS_Get_cs_actual(uint8_t motor);
uint8_t DRV_STATUS_Get_stealth(uint8_t motor);
uint8_t DRV_STATUS_Get_stst(uint8_t motor);
// RW: PWMCONF
void TMC22xx_Set_PWMCONF(uint8_t motor, uint32_t input);
void PWMCONF_Set_pwm_ofs(uint8_t motor, uint8_t B);
void PWMCONF_Set_pwm_grad(uint8_t motor, uint8_t B);
void PWMCONF_Set_pwm_freq(uint8_t motor, uint8_t B);
void PWMCONF_Set_pwm_autoscale(uint8_t motor, uint8_t B);
void PWMCONF_Set_pwm_autograd(uint8_t motor, uint8_t B);
void PWMCONF_Set_freewheel(uint8_t motor, uint8_t B);
void PWMCONF_Set_pwm_reg(uint8_t motor, uint8_t B);
void PWMCONF_Set_pwm_lim(uint8_t motor, uint8_t B);
uint8_t TMC22xx_Get_PWMCONF(uint8_t motor, uint32_t *data);
uint8_t PWMCONF_Get_pwm_ofs(uint8_t motor);
uint8_t PWMCONF_Get_pwm_grad(uint8_t motor);
uint8_t PWMCONF_Get_pwm_freq(uint8_t motor);
uint8_t PWMCONF_Get_pwm_autoscale(uint8_t motor);
uint8_t PWMCONF_Get_pwm_autograd(uint8_t motor);
uint8_t PWMCONF_Get_freewheel(uint8_t motor);
uint8_t PWMCONF_Get_pwm_reg(uint8_t motor);
uint8_t PWMCONF_Get_pwm_lim(uint8_t motor);
// R: PWM_SCALE
uint8_t PWM_SCALE_Get_PWM_SCALE(uint8_t motor, uint32_t *data);
uint8_t PWM_SCALE_Get_pwm_scale_sum(uint8_t motor);
int16_t PWM_SCALE_Get_pwm_scale_auto(uint8_t motor);

void TMC22xx_MotorAccTest(void);


#endif /* __TMC22XX_H__ */
