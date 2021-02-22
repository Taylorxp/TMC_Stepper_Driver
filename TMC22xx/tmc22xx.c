/***************************************************************************************************
* FILE: tmc22xx.c

* DESCRIPTION:  

* CREATED: 2020/09/17, by taylor
***************************************************************************************************/

/*************************************************************************************
* INCLUDE
*************************************************************************************/
#include "tmc22xx.h"
#include "tmc22xx_motor_app.h"

/*************************************************************************************
* DEFINE
*************************************************************************************/
#define TMC22XX_UART_DELAY(ms)		delay_ms(ms)

/*************************************************************************************
* EXTERN GLOBAL VARIABLE
*************************************************************************************/
TMC22xx_MOTOR_S stTmc22xxMotor;
TMC22xx_MOTOR_OPS_S stTmc22xxMotorOps;
TMC22xx_RECV_ST stTMC22xx_Recv = { 0 };
uint16_t Pan_SinWave_Tab[SINEWAVE_NUM * 2] = { 0 };
uint16_t Til_SinWave_Tab[SINEWAVE_NUM * 2] = { 0 };
TMC22xx_S stTMC22xx = 
{
	.bytesWritten 	= 0,
	.replyDelay 	= 5,
	.mA_val 		= 0,
	.Rsense 		= 0.11,
	.flag_otpw 		= 0,
	.write_only 	= 0,
};

typedef struct{
	SHADOW_REGISTER_S *ptr;
} SHADOW_REGISTERConetxt;
	

SHADOW_REGISTER_S stShadow_Reg = 
{
	/**
	 * i_scale_analog = 1; 
	 * pdn_disable = 1; 
	 * multistep_filt = 1 
	 */
	.GCONF_sr = 			0x00000141UL, // Added default: pdn_disable = 1;
	.GSTAT_sr = 			0x00000000UL,
	.SLAVECONF_sr = 		0x00000000UL,
	.OTP_PROG_sr = 			0x00000000UL,
	.OTP_READ_sr = 			0x00000000UL,
	.FACTORY_CONF_sr = 		0x00000000UL,
	/* iholddelay = 1 */
	.IHOLD_IRUN_sr = 		0x00010000UL, // Reset default would be IRUN=31 IHOLD=16
	.TPOWERDOWN_sr = 		0x00000014UL,
	.TPWMTHRS_sr = 			0x00000000UL,
	.VACTUAL_sr = 			0x00000000UL,
	/**
	 * intpol = 1; 
	 * toff = 3; 
	 */
	.CHOPCONF_sr = 			0x10000053UL,
	/**
	 * pwm_reg = 1; 
	 * pwm_autograd = 1; 
	 * pwm_autoscale = 1; 
	 * pwm_freq = 01; fpwm = 2/683fclk
	 * pwm_ofs = 36;
	 */
	.PWMCONF_sr = 			0xC10D0024UL,
	.tmp_sr = 				0x00000000UL,
};

TMC22xx_MOTOR_SHADOW_REGISTER_S stTmc22xxMotorShadowReg = 
{
	.stShadowReg_Motor1 = {
		/**
		* i_scale_analog = 1; 
		* pdn_disable = 1; 
		* multistep_filt = 1 
		*/
		.GCONF_sr = 			0x00000141UL, // Added default: pdn_disable = 1;
		.GSTAT_sr = 			0x00000000UL,
		.SLAVECONF_sr = 		0x00000000UL,
		.OTP_PROG_sr = 			0x00000000UL,
		.OTP_READ_sr = 			0x00000000UL,
		.FACTORY_CONF_sr = 		0x00000000UL,
		/* iholddelay = 1 */
		.IHOLD_IRUN_sr = 		0x00010000UL, // Reset default would be IRUN=31 IHOLD=16
		.TPOWERDOWN_sr = 		0x00000014UL,
		.TPWMTHRS_sr = 			0x00000000UL,
		.VACTUAL_sr = 			0x00000000UL,
		/**
		* intpol = 1; 
		* toff = 3; 
		*/
		.CHOPCONF_sr = 			0x10000053UL,
		/**
		* pwm_reg = 1; 
		* pwm_autograd = 1; 
		* pwm_autoscale = 1; 
		* pwm_freq = 01; fpwm = 2/683fclk
		* pwm_ofs = 36;
		*/
		.PWMCONF_sr = 			0xC10D0024UL,
		.tmp_sr = 				0x00000000UL,
	},
	
	.stShadowReg_Motor2 = {
		/**
		* i_scale_analog = 1; 
		* pdn_disable = 1; 
		* multistep_filt = 1 
		*/
		.GCONF_sr = 			0x00000141UL, // Added default: pdn_disable = 1;
		.GSTAT_sr = 			0x00000000UL,
		.SLAVECONF_sr = 		0x00000000UL,
		.OTP_PROG_sr = 			0x00000000UL,
		.OTP_READ_sr = 			0x00000000UL,
		.FACTORY_CONF_sr = 		0x00000000UL,
		/* iholddelay = 1 */
		.IHOLD_IRUN_sr = 		0x00010000UL, // Reset default would be IRUN=31 IHOLD=16
		.TPOWERDOWN_sr = 		0x00000014UL,
		.TPWMTHRS_sr = 			0x00000000UL,
		.VACTUAL_sr = 			0x00000000UL,
		/**
		* intpol = 1; 
		* toff = 3; 
		*/
		.CHOPCONF_sr = 			0x10000053UL,
		/**
		* pwm_reg = 1; 
		* pwm_autograd = 1; 
		* pwm_autoscale = 1; 
		* pwm_freq = 01; fpwm = 2/683fclk
		* pwm_ofs = 36;
		*/
		.PWMCONF_sr = 			0xC10D0024UL,
		.tmp_sr = 				0x00000000UL,
	}
};

/* 速度表 */
//uint16_t speed_lib[220] = {
//	0xF000, 0xE000, 0xC000, 0xA000, 0x9000, 0x8000, 0x7000, 0x6000, 0x5000, 0x4000,
//	0x3000, 0x3000, 0x2E00, 0x2C00, 0x2A00, 0x2800, 0x2600, 0x2400, 0x2200, 0x2000,
//	0x1F00, 0x1E00, 0x1D00, 0x1C00, 0x1B00, 0x1A00, 0x1900, 0x1800, 0x1700, 0x1600,
//	0x1500, 0x1400, 0x1300, 0x1200, 0x1100, 0x1000, 0x0FF0, 0x0FD0, 0x0FB0, 0x0F90,
//	0x0F70, 0x0F50, 0x0F30, 0x0F10, 0x0EF0, 0x0ED0, 0x0EB0, 0x0E90, 0x0E70, 0x0E50,
//	0x0E30, 0x0E10, 0x0DF0, 0x0DD0, 0x0DB0, 0x0D90, 0x0D70, 0x0D50, 0x0D30, 0x0D10,
//	0x0CF0, 0x0CD0, 0x0CB0, 0x0C90, 0x0C70, 0x0C50, 0x0C30, 0x0C10, 0x0BF0, 0x0BD0,
//	0x0BB0, 0x0B90, 0x0B70, 0x0B50, 0x0B30, 0x0B10, 0x0AF0, 0x0AD0, 0x0AB0, 0x0A80,
//	0x0A50, 0x0A30, 0x0A10, 0x09F0, 0x09D0, 0x09B0, 0x0990, 0x0970, 0x0950, 0x0930,
//	0x0910, 0x08F0, 0x08D0, 0x08B0, 0x0890, 0x0870, 0x0850, 0x0830, 0x0810, 0x07F0,
//	0x07D0, 0x07B0, 0x0790, 0x0770, 0x0750, 0x0730, 0x0710, 0x06F0, 0x06E0, 0x06D0,
//	0x06C0, 0x06C0, 0x06B0, 0x06B0, 0x06B0, 0x06A0, 0x06A0, 0x0690, 0x0690, 0x0680,
//	0x0680, 0x0670, 0x0670, 0x0660, 0x0660, 0x0650, 0x0650, 0x0640, 0x0640, 0x0630,
//	0x0630, 0x0620, 0x0620, 0x0610, 0x0610, 0x0600, 0x0600, 0x05F0, 0x05F0,	0x05F0,
//	0x05D0, 0x05D0, 0x05C0, 0x05C0, 0x05C0, 0x05A0, 0x05A0, 0x05A0, 0x0580,	0x0580,
//	0x0580, 0x0580, 0x0560, 0x0550, 0x0540, 0x0530, 0x0520, 0x0510, 0x0500,	0x04F0,
//	0x04E0, 0x04D0, 0x04C0, 0x04B0, 0x04A0, 0x0490, 0x0480, 0x0470, 0x0460,	0x0450,
//	0x0450, 0x0450, 0x0450, 0x0440, 0x0430, 0x0420, 0x0410, 0x0400, 0x03F0, 0x03E0,
//	0x04D0, 0x03C0, 0x03B0, 0x03B0, 0x03A0, 0x03A0, 0x0390, 0x0390, 0x0380, 0x0380,
//	0x0370, 0x0370, 0x0360, 0x0360, 0x0350, 0x0350, 0x0340, 0x0340, 0x0330, 0x0330,
//	0x0320, 0x0320, 0x0310, 0x0310, 0x0300, 0x0300, 0x02F0, 0x02F0,	0x02E0, 0x02E0,
//	0x02D0, 0x02D0, 0x02C0, 0x02C0, 0x02B0, 0x02B0, 0x02A0, 0x02A0, 0x0290, 0x0290,
//};
///**
// * 等差数列：
// * 第一段： y = 0xf000 - 0x2000 * x
// * 第二段： y = 0x3000 - 0x200  * x
// * 第三段： y = 0x1000 - 0x30   * x
// * 第四段： y = 0x580  - 0x10   * x
// * 第五段： y = 0x280
// */
//uint16_t speed_lib[127] = {
//	/* 第一段 */
//	0xf000, 0xd000, 0xb000, 0x9000, 0x7000, 0x5000, 0x3000,
//	0x2e00, 0x2c00, 0x2a00, 0x2800, 0x2600, 0x2400, 0x2200,
//	/* 第二段 */
//	0x2000, 0x1e00, 0x1c00, 0x1a00, 0x1800, 0x1600, 0x1400, 
//	0x1200, 0x1000,
//	/* 第三段 */
//	0xfd0,  0xfa0,  0xf70,  0xf40,  0xf10,  0xee0,  0xeb0, 0xe80,
//	0xe50,  0xe20,  0xdf0,  0xdc0,  0xd90,  0xd60,  0xd30, 0xd00,
//	0xcd0,  0xca0,  0xc70,  0xc40,  0xc10,  0xbe0,  0xbb0, 0xb80,
//	0xb50,  0xb20,  0xaf0,  0xac0,  0xa90,  0xa60,  0xa30, 0xa00,
//	0x9d0,  0x9a0,  0x970,  0x940,  0x910,  0x8e0,  0x8b0, 0x880,
//	0x850,  0x820,  0x7f0,  0x7c0,  0x790,  0x760,  0x730, 0x700,
//	0x6d0,  0x6a0,  0x670,  0x640,  0x610,  0x5e0,  0x5b0, 0x580,
//	/* 第四段 */
//	0x570,  0x560,  0x550,  0x540,  0x530,  0x520,  0x510, 0x500, 
//	0x4f0,  0x4e0,  0x4d0,  0x4c0,  0x4b0,  0x4a0,  0x490, 0x480, 
//	0x470,  0x460,  0x450,  0x440,  0x430,  0x420,  0x410, 0x400, 
//	0x3f0,  0x3e0,  0x3d0,  0x3c0,  0x3b0,  0x3a0,  0x390, 0x380, 
//	0x370,  0x360,  0x350,  0x340,  0x330,  0x320,  0x310, 0x300, 
//	0x2f0,  0x2e0,  0x2d0,  0x2c0,  0x2b0,  0x2a0,  0x290, 0x280,
//};
/**
 * 等差数列：
 * 第一段： y = 0xf000 - 0x2000 * x
 * 第二段： y = 0x3000 - 0x200  * x
 * 第三段： y = 0x1000 - 0x30   * x
 * 第四段： y = 0x580  - 0x10   * x
 * 第五段： y = 0x380  - 0x8    * x
 * 第六段： y = 0x280
 */
uint16_t speed_lib[144] = {
	0xffff,
	/* 第一段 */
	0xf000, 0xd000, 0xb000, 0x9000, 0x7000, 0x5000, 0x3000,
	/* 第二段 */
	0x2e00, 0x2c00, 0x2a00, 0x2800, 0x2600, 0x2400, 0x2200,
	0x2000, 0x1e00, 0x1c00, 0x1a00, 0x1800, 0x1600, 0x1400,
	0x1200, 0x1000,
	/* 第三段 */
	0xfd0,  0xfa0,  0xf70,  0xf40,  0xf10,  0xee0,  0xeb0, 0xe80,
	0xe50,  0xe20,  0xdf0,  0xdc0,  0xd90,  0xd60,  0xd30, 0xd00,
	0xcd0,  0xca0,  0xc70,  0xc40,  0xc10,  0xbe0,  0xbb0, 0xb80,
	0xb50,  0xb20,  0xaf0,  0xac0,  0xa90,  0xa60,  0xa30, 0xa00,
	0x9d0,  0x9a0,  0x970,  0x940,  0x910,  0x8e0,  0x8b0, 0x880,
	0x850,  0x820,  0x7f0,  0x7c0,  0x790,  0x760,  0x730, 0x700,
	0x6d0,  0x6a0,  0x670,  0x640,  0x610,  0x5e0,  0x5b0, 0x580,
	/* 第四段 */
	0x570,  0x560,  0x550,  0x540,  0x530,  0x520,  0x510, 0x500,
	0x4f0,  0x4e0,  0x4d0,  0x4c0,  0x4b0,  0x4a0,  0x490, 0x480,
	0x470,  0x460,  0x450,  0x440,  0x430,  0x420,  0x410, 0x400,
	0x3f0,  0x3e0,  0x3d0,  0x3c0,  0x3b0,  0x3a0,  0x390, 0x380,
	/* 第五段 */
	0x378,  0x370,  0x368,  0x360,  0x358,  0x350,  0x348, 0x340,
	0x338,  0x330,  0x328,  0x320,  0x318,  0x310,  0x308, 0x300,
	0x2f8,  0x2f0,  0x2e8,  0x2e0,  0x2d8,  0x2d0,  0x2c8, 0x2c0,
	0x2b8,  0x2b0,  0x2a8,  0x2a0,  0x298,  0x290,  0x288, 0x280,
};

uint16_t phase_lib[360] = {
	0, 55, 111, 167, 223, 278, 334, 389, 445, 500, 
	555, 610, 665, 719, 774, 828, 882, 935, 988, 1041, 
	1094, 1146, 1198, 1250, 1301, 1352, 1402, 1452, 1502, 1551, 
	1599, 1648, 1695, 1742, 1789, 1835, 1880, 1925, 1970, 2013, 
	2056, 2099, 2141, 2182, 2222, 2262, 2301, 2340, 2378, 2415, 
	2451, 2486, 2521, 2555, 2588, 2621, 2652, 2683, 2713, 2742, 
	2771, 2798, 2825, 2851, 2876, 2900, 2923, 2945, 2966, 2987,
	3007, 3025, 3043, 3060, 3076, 3090, 3104, 3117, 3130, 3141, 
	3151, 3160, 3168, 3176, 3182, 3187, 3192, 3195, 3198, 3199, 
	3200, 3199, 3198, 3195, 3192, 3187, 3182, 3176, 3168, 3160, 
	3151, 3141, 3130, 3117, 3104, 3090, 3076, 3060, 3043, 3025, 
	3007, 2987, 2966, 2945, 2923, 2900, 2876, 2851, 2825, 2798, 
	2771, 2742, 2713, 2683, 2652, 2621, 2588, 2555, 2521, 2486, 
	2451, 2415, 2378, 2340, 2301, 2262, 2222, 2182, 2141, 2099, 
	2056, 2013, 1970, 1925, 1880, 1835, 1789, 1742, 1695, 1648, 
	1599, 1551, 1502, 1452, 1402, 1352, 1301, 1250, 1198, 1146, 
	1094, 1041, 988, 935, 882, 828, 774, 719, 665, 610, 
	555, 500, 445, 389, 334, 278, 223, 167, 111, 55,
	0, 55, 111, 167, 223, 278, 334, 389, 445, 500, 
	555, 610, 665, 719, 774, 828, 882, 935, 988, 1041, 
	1094, 1146, 1198, 1250, 1301, 1352, 1402, 1452, 1502, 1551, 
	1599, 1648, 1695, 1742, 1789, 1835, 1880, 1925, 1970, 2013, 
	2056, 2099, 2141, 2182, 2222, 2262, 2301, 2340, 2378, 2415, 
	2451, 2486, 2521, 2555, 2588, 2621, 2652, 2683, 2713, 2742, 
	2771, 2798, 2825, 2851, 2876, 2900, 2923, 2945, 2966, 2987,
	3007, 3025, 3043, 3060, 3076, 3090, 3104, 3117, 3130, 3141, 
	3151, 3160, 3168, 3176, 3182, 3187, 3192, 3195, 3198, 3199, 
	3200, 3199, 3198, 3195, 3192, 3187, 3182, 3176, 3168, 3160, 
	3151, 3141, 3130, 3117, 3104, 3090, 3076, 3060, 3043, 3025, 
	3007, 2987, 2966, 2945, 2923, 2900, 2876, 2851, 2825, 2798, 
	2771, 2742, 2713, 2683, 2652, 2621, 2588, 2555, 2521, 2486, 
	2451, 2415, 2378, 2340, 2301, 2262, 2222, 2182, 2141, 2099, 
	2056, 2013, 1970, 1925, 1880, 1835, 1789, 1742, 1695, 1648, 
	1599, 1551, 1502, 1452, 1402, 1352, 1301, 1250, 1198, 1146, 
	1094, 1041, 988, 935, 882, 828, 774, 719, 665, 610, 
	555, 500, 445, 389, 334, 278, 223, 167, 111, 55,
};

void TMC22xx_Uart_Write(uint8_t motor, uint8_t reg, int32_t dat);

/*********************************************************************************
* Function Declaration: 正弦表
* Discreable:
*********************************************************************************/
void SinWave_Generator_Init(void)
{
#define PI		3.1415926
	
	for (uint16_t i = 0; i < SINEWAVE_NUM * 2; i++) {
//		SinWave_Tab[i] = (uint16_t)(MAX_PERIOD * sin(i * (PI / SINEWAVE_NUM)));
		Pan_SinWave_Tab[i] = (uint16_t)((MAX_PERIOD / 2) * sin(i * PI / SINEWAVE_NUM) + (MAX_PERIOD / 2));
		Til_SinWave_Tab[i] = (uint16_t)((MAX_PERIOD / 2) * sin(i * PI / SINEWAVE_NUM) + (MAX_PERIOD / 2));
	}
}

/* 内外部时钟选择 */
#ifdef USE_EXTERN_CLK

/*********************************************************************************
* Function Declaration: 电机外部时钟初始化
* Discreable: 使用外部时钟时，配置时钟频率10MHz ~ 16MHz；当时钟占空比为50%时，
*			  最高使用18MHz
*********************************************************************************/
static void TMC22xx_Clk_Init(void)
{
#if USE_JUMPER_WIRE
	
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	/* PA11 & PA8 for PAN & TIL CLK TIM1_CH4 & TIM1_CH1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_DeInit(TIM1);
	
	TIM_BaseInitStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_BaseInitStructure.TIM_Prescaler		 	= 0;   //96M
    TIM_BaseInitStructure.TIM_Period 			= 8 - 1;  //
    TIM_BaseInitStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel 					 = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
    NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse		= PWM_DUTY_CYCLE;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
    TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
#elif USE_PARTIAL_JUMPER_WIRE
	
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	/* PB6 for PAN CLK TIM4_CH1 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	    
    TIM_BaseInitStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_BaseInitStructure.TIM_Prescaler		 	= 0;   //96M
    TIM_BaseInitStructure.TIM_Period 			= 8 - 1;  //
    TIM_BaseInitStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel 					 = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
    NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse		= PWM_DUTY_CYCLE;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_Cmd(TIM4, ENABLE);
	
	/* PA11 or PA8 for TIL CLK TIM1_CH4 or TIM1_CH1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_BaseInitStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_BaseInitStructure.TIM_Prescaler		 	= 0;   //96M
    TIM_BaseInitStructure.TIM_Period 			= 8 - 1;  //
    TIM_BaseInitStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel 					 = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
    NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse		= PWM_DUTY_CYCLE;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
# if 0
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
# else
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
# endif
	
    TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

#endif
}

/*********************************************************************************
* Function Declaration: 电机参考引脚初始化
* Discreable: 使用外部时钟时，使用MCU生成的SPWM控制，产生30KHz
*********************************************************************************/
static void TMC22xx_Vref_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	/* PA1 for PAN VREF SPWM TIM2_CH2 or TIM5_CH2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_DeInit(TIM2);
	    
    TIM_BaseInitStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_BaseInitStructure.TIM_Prescaler		 	= 0;   //96M
    TIM_BaseInitStructure.TIM_Period 			= MAX_PERIOD - 1;  //
    TIM_BaseInitStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel 					 = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;
    NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse 		= 0;
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_Low;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
	
	/* PB5 for TIL VREF SPWM TIM3_CH2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_DeInit(TIM3);

    TIM_BaseInitStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_BaseInitStructure.TIM_Prescaler		 	= 0;   //96M
    TIM_BaseInitStructure.TIM_Period 			= MAX_PERIOD - 1;  //
    TIM_BaseInitStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel 					 = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
    NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse 		= 0;
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

#else

/*********************************************************************************
* Function Declaration: 电机内部时钟初始化
* Discreable: 使用内部时钟时，CLK引脚直接接GND
*********************************************************************************/
static void TMC22xx_Clk_Init(void)
{
	/* PAN TIL CLK TO GND */
	PAN_CLK_TO_GND();
	TIL_CLK_TO_GND();
}

/*********************************************************************************
* Function Declaration: 电机参考引脚初始化
* Discreable: 使用内部时钟时，Vref引脚直接接VCC_IO
*********************************************************************************/
static void TMC22xx_Vref_Init(void)
{
	/* PAN TIL VREF TO VCC_IO */
	PAN_VREF_TO_VCC_IO();
	TIL_VREF_TO_VCC_IO();
}

#endif

/*********************************************************************************
* Function Declaration: 电机分辨率初始化
* Discreable: MS0  MS1	细分
*			   0	0	 8
*			   0	1	 2
*			   1	0	 4
*			   1	1	16
*********************************************************************************/
static void TMC22xx_Res_Init(void)
{
	PAN_MS1 = 1;
	PAN_MS2 = 1;
	TIL_MS1 = 1;
	TIL_MS2 = 1;
}

/*********************************************************************************
* Function Declaration: 电机串口初始化
* Discreable: 波特率设置 9600 ~ 500Kbps
*********************************************************************************/
static void TMC22xx_Uart_Init(void)
{
	/* TIL */
	UART4_Init(TMC22xx_BAUD);
	/* PAN */
	UART5_Init(TMC22xx_BAUD);
}

/*********************************************************************************
* Function Declaration: STEP脉冲
*********************************************************************************/
static void TMC22xx_StepFreqConfig_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	    
    TIM_BaseInitStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;
    TIM_BaseInitStructure.TIM_Prescaler		 	= 15;   //
    TIM_BaseInitStructure.TIM_Period 			= 0xFFFF;  //
    TIM_BaseInitStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_BaseInitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel 					 = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
    NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
    TIM_Cmd(TIM4, DISABLE);
}

/*********************************************************************************
* Function Declaration: 串口数据校验
*********************************************************************************/
static void swuart_calcCRC(uint8_t* datagram, uint8_t datagramLength)
{
	int i,j;
	
	uint8_t* crc = datagram + (datagramLength-1); // CRC located in last byte of message
	uint8_t currentByte;
	
	*crc = 0;
	
	for (i=0; i<(datagramLength-1); i++)
	{ // Execute for all bytes of a message
		currentByte = datagram[i]; // Retrieve a byte to be sent from Array
		for (j=0; j<8; j++)
		{
			if ((*crc >> 7) ^ (currentByte&0x01)) // update CRC based result of XOR operation
			{
				*crc = (*crc << 1) ^ 0x07;
			}
			else
			{
				*crc = (*crc << 1);
			}
			currentByte = currentByte >> 1;
		} // for CRC bit
	} // for message byte
}

/*********************************************************************************
* Function Declaration: TMC22xx串口写数据
*********************************************************************************/
void TMC22xx_Uart_Write(uint8_t motor, uint8_t reg, int32_t dat)
{
	static uint8_t writeArr[8] = { 0 };
	
	writeArr[0] = SYNC_DATA;
	writeArr[1] = SLAVE_ADDR;
	writeArr[2] = reg | WRITE_FIXED_REGISTER_HIGH;
	writeArr[3] = (uint8_t)(dat >> 24);
	writeArr[4] = (uint8_t)(dat >> 16);
	writeArr[5] = (uint8_t)(dat >> 8);
	writeArr[6] = (uint8_t)(dat >> 0);
	
	swuart_calcCRC(writeArr, sizeof(writeArr));
	
	TMC22xx_SendData(motor, writeArr, sizeof(writeArr));
}

/*********************************************************************************
* Function Declaration: TMC22xx串口读请求
*********************************************************************************/
void TMC22xx_Uart_Read_Req(uint8_t motor, uint8_t reg)
{
	uint8_t read_req[4] = { 0 };
	
	read_req[0] = SYNC_DATA;
	read_req[1] = SLAVE_ADDR;
	read_req[2] = reg | READ_REQ_FIXED_REGISTER_HIGH;
	
	swuart_calcCRC(read_req, sizeof(read_req));
	
	TMC22xx_SendData(motor, read_req, sizeof(read_req));
}

/*********************************************************************************
* Function Declaration: 
*********************************************************************************/
static int32_t TMC22xx_Uart_Read(uint8_t motor, uint8_t reg)
{
	TMC22xx_Uart_Read_Req(motor, reg);
	
	return 0;
}

/*********************************************************************************
* Function Declaration: 电流设置
* mA: 电流大小
* multiplier: 倍数
* RS: 感测电阻值
*********************************************************************************/
//static void TMC22xx_CurrentConfig_Init(void)
//{
//	static uint8_t ihold = 16, irun = 31, ihold_delay = 15;
//	
//	TMC22xx_Uart_Write(TMC22XX_TIL, REG_IHOLD_IRUN, 
//					   IHOLD_IRUN_CONFIG(ihold_delay, irun, ihold));	//断电时间为 1 * 2^18 * tclk
//	
//	static uint8_t tpowerdown = 128;
//	TMC22xx_Uart_Write(TMC22XX_TIL, REG_TPOWER_DOWN, 
//					   TPOWER_DOWN_CONFIG(tpowerdown));
//}

/*********************************************************************************
* Function Declaration: StealthChop模式配置
*********************************************************************************/
//static void TMC22xx_StealthChopConfig_Init(void)
//{
////	TMC22xx_Uart_Write(TMC22XX_TIL, REG_PWMCONF, 
////					   PWMCONFIG(12, 8, 0, 1, 0, 0, 255, 255));
//	/* OTP: pwm_ofs = 36 or 0 */
//	static uint8_t pwm_lim = 12, pwm_reg = 8, freewheel = 0, pwm_autograd = 1, 
//					pwm_autoscale = 1, pwm_freq = 1, pwm_grad = 14, pwm_ofs = 36;
//	
//	TMC22xx_Uart_Write(TMC22XX_TIL, REG_PWMCONF, 
//					   PWMCONFIG(pwm_lim, pwm_reg, freewheel, pwm_autograd, 
//								 pwm_autoscale, pwm_freq, pwm_grad, pwm_ofs));
//
//	/* OTP: StealthChop mode: SpreadCycle settings: HEND = 0; HSTRAT = 5; TOFF = 0 */
//	static uint8_t mres = 0, tbl = 1, hend = 0, hstart = 4, toff = 5;
//	
//	TMC22xx_Uart_Write(TMC22XX_TIL, REG_CHOPCONF, 
//					   CHOPCONFIG(mres, tbl, hend, hstart, toff));
//}

/*********************************************************************************
* Function Declaration: 速度配置
*********************************************************************************/
static void TMC22xx_VelocityConfig_Init(void)
{
	
}

/*********************************************************************************
* Function Declaration: 电机初始化
*********************************************************************************/
//static void TMC22xx_RegConfig_Init(void)
//{
//	/* 先进行状态复位 */
////	TMC22xx_Uart_Write(TMC22XX_TIL, REG_GSTAT, 
////					   GSTATCONFIG);
//	
//	static uint8_t i_scale_analog = 1, en_spreadcycle = 0, dir_shaft = 0, mutistep_filt = 0;
////	TMC22xx_Uart_Write(TMC22XX_PAN, GCONF, GCONFIG(0, 0));
//	TMC22xx_Uart_Write(TMC22XX_TIL, REG_GCONF, 
//					   GCONFIG(mutistep_filt, dir_shaft, en_spreadcycle, i_scale_analog));
//	
////	TMC22xx_Uart_Write(TMC22XX_PAN, REG_SLAVECONF, SLAVECONFIG_SEND_DELAY(1));
//	TMC22xx_Uart_Write(TMC22XX_TIL, REG_SLAVECONF, 
//					   SLAVECONFIG_SEND_DELAY(1));
////	TMC22xx_Uart_Write(TMC22XX_PAN, REG_IHOLD_IRUN, IHOLD_IRUN_CONFIG(1, 16, 16));
////	TMC22xx_Uart_Write(TMC22XX_TIL, REG_IHOLD_IRUN, IHOLD_IRUN_CONFIG(1, 16, 16));	//断电时间为 1 * 2^18 * tclk
////	TMC22xx_Uart_Write(TMC22XX_PAN, REG_TPOWER_DOWN, TPOWER_DOWN_CONFIG(5));		//停止到电机电流断电的延时时间
////	TMC22xx_Uart_Write(TMC22XX_TIL, REG_TPOWER_DOWN, TPOWER_DOWN_CONFIG(5));
////	TMC22xx_Uart_Write(TMC22XX_PAN, REG_TPWMTHRS, TPWMTHRS_CONFIG(255));
//	
//	static uint8_t fclktrim = 15, ottrim = 0;
//	TMC22xx_Uart_Write(TMC22XX_TIL, REG_FACTORY_CONF, 
//					   FACTORY_CONFIG(ottrim, fclktrim));
//	
//	TMC22xx_CurrentConfig_Init();
//	TMC22xx_StealthChopConfig_Init();
//	
//	static uint32_t tpwmthrs = 0;
//	TMC22xx_Uart_Write(TMC22XX_TIL, REG_TPWMTHRS, 
//					   TPWMTHRS_CONFIG(tpwmthrs));
////	TMC22xx_Uart_Write(TMC22XX_PAN, REG_VACTUAL, VACTUAL_CONFIG(0));
//	static uint32_t vactual = 0;
//	TMC22xx_Uart_Write(TMC22XX_TIL, REG_VACTUAL, 
//					   VACTUAL_CONFIG(vactual));
////	TMC22xx_Uart_Write(TMC22XX_PAN, REG_CHOPCONF, CHOPCONFIG(0, 1, 5, 10, 5));
////	TMC22xx_Uart_Write(TMC22XX_TIL, REG_CHOPCONF, CHOPCONFIG(0, 1, 5, 10, 5));
////	TMC22xx_Uart_Write(TMC22XX_PAN, REG_PWMCONF, PWMCONFIG(12, 0, 0, 1, 0, 0, 0, 36));
////	TMC22xx_Uart_Write(TMC22XX_TIL, REG_PWMCONF, PWMCONFIG(12, 8, 0, 1, 0, 0, 0, 36));
//	
//	TMC22xx_Uart_Write(TMC22XX_TIL, REG_GSTAT, 
//					   GSTATCONFIG);
//	
//	delay_ms(100);
//	
////	PAN_FORWARD_DIR();
//	TIL_FORWARD_DIR();
//}

/*********************************************************************************
* Function Declaration: 电机初始化
*********************************************************************************/
void TMC22xx_Init(uint8_t motor)
{
	SinWave_Generator_Init();
	TMC22xx_Clk_Init();
	TMC22xx_Res_Init();
	TMC22xx_Uart_Init();
	TMC22xx_Vref_Init();
//	TMC22xx_RegConfig_Init();
	TMC22xx_StepFreqConfig_Init();
	
	TMC22xx_ResetReg_Push(motor);
	static uint8_t i_scale_analog = 1;
	GCONF_Set_I_scale_analog(motor, i_scale_analog);
	TMC22XX_UART_DELAY(2);
	static uint8_t internal_rsense = 1;
	GCONF_Set_internal_Rsense(motor, internal_rsense);
	TMC22XX_UART_DELAY(2);
	static uint8_t multistep_filt = 0;
	GCONF_Set_multistep_filt(motor, multistep_filt);
	TMC22XX_UART_DELAY(2);
	static uint8_t shaft = 0;
	/* shaft = 1 : inverse motor direction */
	GCONF_Set_shaft(motor, shaft);
	TMC22XX_UART_DELAY(2);
	static float multiplier = 0.6, rs = 0.75; 
	static uint16_t mA = 200;
	TMC22xx_SetRmsCurrent(motor, mA, multiplier, rs);
	TMC22XX_UART_DELAY(2);
	static uint8_t pwm_reg = 8;
	PWMCONF_Set_pwm_reg(motor, pwm_reg);
	TMC22XX_UART_DELAY(2);
	static uint8_t pwm_freq = 1;
	PWMCONF_Set_pwm_freq(motor, pwm_freq);
	TMC22XX_UART_DELAY(2);
	static uint8_t pwm_grad = 14;
	PWMCONF_Set_pwm_grad(motor, pwm_grad);
	TMC22XX_UART_DELAY(2);
	static uint8_t pwm_ofs = 36;
	PWMCONF_Set_pwm_ofs(motor, pwm_ofs);
	TMC22XX_UART_DELAY(2);
	static uint8_t tbl = 1;
	CHOPCONF_Set_tbl(motor, tbl);
	TMC22XX_UART_DELAY(2);
	static uint8_t hend = 5;
	CHOPCONF_Set_hend(motor, hend);
	TMC22XX_UART_DELAY(2);
	static uint8_t hstrt = 0;
	CHOPCONF_Set_hstrt(motor, hstrt);
	TMC22XX_UART_DELAY(2);
	static uint8_t dedge = 1;
	CHOPCONF_Set_dedge(motor, dedge);
	TMC22XX_UART_DELAY(2);
	
	TMC22XX_UART_DELAY(200);

	memset(&stTmc22xxMotor, 0, sizeof(stTmc22xxMotor));
}

/*********************************************************************************
* Function Declaration: TMC22xx串口读处理
*********************************************************************************/
//void TMC22xx_Read_ProcessHandler(void)
//{
//	if ((stTMC22xx_Recv.til_flag == 1) || (stTMC22xx_Recv.pan_flag == 1)) {
//		if (stTMC22xx_Recv.til_flag == 1) {
//			stTMC22xx_Recv.til_flag = 0;
//		} else if (stTMC22xx_Recv.pan_flag == 1) {
//			stTMC22xx_Recv.pan_flag = 0;
//		}
//	}
//}

/*********************************************************************************
* Function Declaration: TMC22xx串口读处理
*********************************************************************************/
static inline SHADOW_REGISTER_S* TMC22xx_Motor_ShadowRegisterSelect(uint8_t motor)
{
	if (motor) {
		return &stTmc22xxMotorShadowReg.stShadowReg_Motor2;
	} else {
		return &stTmc22xxMotorShadowReg.stShadowReg_Motor1;
	}
}

/*	
	Requested current = mA = I_rms/1000
	Equation for current:
	I_rms = (CS+1)/32 * V_fs/(R_sense+0.02ohm) * 1/sqrt(2)
	Solve for CS ->
	CS = 32*sqrt(2)*I_rms*(R_sense+0.02)/V_fs - 1
	
	Example:
	vsense = 0b0 -> V_fs = 0.325V
	mA = 1640mA = I_rms/1000 = 1.64A
	R_sense = 0.10 Ohm
	->
	CS = 32*sqrt(2)*1.64*(0.10+0.02)/0.325 - 1 = 26.4
	CS = 26
*/
/*********************************************************************************
* Function Declaration: 电流RMS设置
* mA: 电流大小
* multiplier: 倍数
* RS: 感测电阻值
*********************************************************************************/
void TMC22xx_SetRmsCurrent(uint8_t motor, uint16_t mA, float multiplier, float RS)
{
	stTMC22xx.Rsense = RS;
	uint8_t CS = (uint8_t)(32.0 * 1.41421 * mA / 1000.0 * (stTMC22xx.Rsense + 0.02) / 0.325 - 1);
	// If Current Scale is too low, turn on high sensitivity R_sense and calculate again
	if (CS < 16) {
		CHOPCONF_Set_vsense(motor, HIGH);
		CS = (uint8_t)(32.0 * 1.41421 * mA / 1000.0 * (stTMC22xx.Rsense + 0.02) / 0.180 - 1);
	} else { // If CS >= 16, turn off high_sense_r if it's currently ON
		CHOPCONF_Set_vsense(motor, LOW);
	}
	
	if (CS > 31)	CS = 31;
	
	IHOLD_IRUN_Set_irun(motor, CS);
	IHOLD_IRUN_Set_ihold(motor, (uint8_t)(CS * multiplier));
	stTMC22xx.mA_val = mA;
}

/*********************************************************************************
* Function Declaration: 获取电流RMS
*********************************************************************************/
uint16_t TMC22xx_GetRmsCurrent(uint8_t motor)
{
	return (uint16_t)((float)(IHOLD_IRUN_Get_irun(motor) + 1) / 32.0 * (CHOPCONF_Get_vsense(motor) ? 0.180 : 0.325) / (stTMC22xx.Rsense + 0.02) / 1.41421 * 1000);
}

/*********************************************************************************
* Function Declaration: 微步设置
* microsteps: 微步
*********************************************************************************/
void TMC22xx_SetMicrosteps(uint8_t motor, uint16_t ms)
{
	switch(ms) {
		case 256: CHOPCONF_Set_mres(motor, 0); break;
		case 128: CHOPCONF_Set_mres(motor, 1); break;
		case  64: CHOPCONF_Set_mres(motor, 2); break;
		case  32: CHOPCONF_Set_mres(motor, 3); break;
		case  16: CHOPCONF_Set_mres(motor, 4); break;
		case   8: CHOPCONF_Set_mres(motor, 5); break;
		case   4: CHOPCONF_Set_mres(motor, 6); break;
		case   2: CHOPCONF_Set_mres(motor, 7); break;
		case   0: CHOPCONF_Set_mres(motor, 8); break;
		default: break;
	}
}

/*********************************************************************************
* Function Declaration: 获取微步
*********************************************************************************/
uint16_t TMC22xx_GetMicrosteps(uint8_t motor)
{
	switch(CHOPCONF_Get_mres(motor)) {
		case 0: return 256;
		case 1: return 128;
		case 2: return  64;
		case 3: return  32;
		case 4: return  16;
		case 5: return   8;
		case 6: return   4;
		case 7: return   2;
		case 8: return   0;
	}
	return 0;
}

/*********************************************************************************
* Function Declaration: 电流设置
* mA: 电流大小
* multiplier: 倍数
* RS: 感测电阻值
*********************************************************************************/
void TMC22xx_SetCurrent(uint8_t motor, uint16_t mA, float multiplier, float RS)
{
	TMC22xx_SetRmsCurrent(motor, mA, multiplier, stTMC22xx.Rsense);
}

/*********************************************************************************
* Function Declaration: 获取电流
*********************************************************************************/
uint16_t TMC22xx_GetCurrent(uint8_t motor)
{
	return stTMC22xx.mA_val;
}

/*********************************************************************************
* Function Declaration: 温度
*********************************************************************************/
uint8_t TMC22xx_CheckOT(uint8_t motor)
{
	uint32_t response;
	
	TMC22xx_Get_DRV_STATUS(motor, &response);
	
	if (response & OTPW_bm) {
		stTMC22xx.flag_otpw = HIGH;
		return 1; // bit 26 for overtemperature warning flag
	}
	return 0;
}
uint8_t TMC22xx_GetOTPW(uint8_t motor)
{
	return stTMC22xx.flag_otpw;
}
void TMC22xx_ClearOtpw(uint8_t motor)
{
	stTMC22xx.flag_otpw = 0;
}

uint8_t TMC22xx_IsEnabled(uint8_t motor)
{
	return IOIN_Get_enn(motor) && CHOPCONF_Get_toff(motor);
}

/*********************************************************************************
* Function Declaration: 复位寄存器
* Push at the start of setting up the driver resets the register to default
*********************************************************************************/
void TMC22xx_ResetReg_Push(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	/* i_scale_analog = 1; pdn_disable = 1;  multistep_filt = 1 */
	TMC22xx_Set_GCONF(motor, pstShadowReg->GCONF_sr);
	TMC22XX_UART_DELAY(2);
	/* iholddelay = 1 */
	TMC22xx_Set_IHOLD_IRUN(motor, pstShadowReg->IHOLD_IRUN_sr);
	TMC22XX_UART_DELAY(2);
	/*  */
	TMC22xx_Set_SLAVECONF(motor, pstShadowReg->SLAVECONF_sr);
	TMC22XX_UART_DELAY(2);
	/*  */
	TMC22xx_Set_FACTORY_CONF(motor, pstShadowReg->FACTORY_CONF_sr);
	TMC22XX_UART_DELAY(2);
	/*  */
	TMC22xx_Set_TPOWERDOWN(motor, pstShadowReg->TPOWERDOWN_sr);
	TMC22XX_UART_DELAY(2);
	/*  */
	TMC22xx_Set_TPWMTHRS(motor, pstShadowReg->TPWMTHRS_sr);
	TMC22XX_UART_DELAY(2);
	/*  */
	TMC22xx_Set_VACTUAL(motor, pstShadowReg->VACTUAL_sr);
	TMC22XX_UART_DELAY(2);
	/* intpol = 1; toff = 3 */
	TMC22xx_Set_CHOPCONF(motor, pstShadowReg->CHOPCONF_sr);
	TMC22XX_UART_DELAY(2);
	/* pwm_reg = 1; pwm_autograd = 1; pwm_autoscale = 1; pwm_freq = 01 */
	TMC22xx_Set_PWMCONF(motor, pstShadowReg->PWMCONF_sr);
	TMC22XX_UART_DELAY(200);
}

uint8_t TMC22xx_Test_Connection(uint8_t motor)
{
	return 0;
}

/*********************************************************************************
* Function Declaration: GCONF设置
*********************************************************************************/
void TMC22xx_Set_GCONF(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GCONF_sr = input;
	TMC22xx_Uart_Write(motor, REG_GCONF, pstShadowReg->GCONF_sr);
}
void GCONF_Set_I_scale_analog(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GCONF_sr &= ~I_SCALE_ANALOG_bm;
	pstShadowReg->GCONF_sr |= ((uint32_t)B << I_SCALE_ANALOG_bp) & I_SCALE_ANALOG_bm;
	TMC22xx_Uart_Write(motor, REG_GCONF, pstShadowReg->GCONF_sr);
}
void GCONF_Set_internal_Rsense(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GCONF_sr &= ~INTERNAL_RSENSE_bm;
	pstShadowReg->GCONF_sr |= ((uint32_t)B << INTERNAL_RSENSE_bp) & INTERNAL_RSENSE_bm;
	TMC22xx_Uart_Write(motor, REG_GCONF, pstShadowReg->GCONF_sr);
}
void GCONF_Set_en_spreadCycle(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GCONF_sr &= ~EN_SPREADCYCLE_bm;
	pstShadowReg->GCONF_sr |= ((uint32_t)B << EN_SPREADCYCLE_bp) & EN_SPREADCYCLE_bm;
	TMC22xx_Uart_Write(motor, REG_GCONF, pstShadowReg->GCONF_sr);
}
void GCONF_Set_shaft(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GCONF_sr &= ~SHAFT_bm;
	pstShadowReg->GCONF_sr |= ((uint32_t)B << SHAFT_bp) & SHAFT_bm;
	TMC22xx_Uart_Write(motor, REG_GCONF, pstShadowReg->GCONF_sr);
}
void GCONF_Set_index_otpw(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GCONF_sr &= ~INDEX_OTPW_bm;
	pstShadowReg->GCONF_sr |= ((uint32_t)B << INDEX_OTPW_bp) & INDEX_OTPW_bm;
	TMC22xx_Uart_Write(motor, REG_GCONF, pstShadowReg->GCONF_sr);
}
void GCONF_Set_index_step(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GCONF_sr &= ~INDEX_STEP_bm;
	pstShadowReg->GCONF_sr |= ((uint32_t)B << INDEX_STEP_bp) & INDEX_STEP_bm;
	TMC22xx_Uart_Write(motor, REG_GCONF, pstShadowReg->GCONF_sr);
}
void GCONF_Set_pdn_disable(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GCONF_sr &= ~PDN_DISABLE_bm;
	pstShadowReg->GCONF_sr |= ((uint32_t)B << PDN_DISABLE_bp) & PDN_DISABLE_bm;
	TMC22xx_Uart_Write(motor, REG_GCONF, pstShadowReg->GCONF_sr);
}
void GCONF_Set_mstep_reg_select(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GCONF_sr &= ~MSTEP_REG_SELECT_bm;
	pstShadowReg->GCONF_sr |= ((uint32_t)B << MSTEP_REG_SELECT_bp) & MSTEP_REG_SELECT_bm;
	TMC22xx_Uart_Write(motor, REG_GCONF, pstShadowReg->GCONF_sr);
}
void GCONF_Set_multistep_filt(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GCONF_sr &= ~MULTISTEP_FILT_bm;
	pstShadowReg->GCONF_sr |= ((uint32_t)B << MULTISTEP_FILT_bp) & MULTISTEP_FILT_bm;
	TMC22xx_Uart_Write(motor, REG_GCONF, pstShadowReg->GCONF_sr);
}

/*********************************************************************************
* Function Declaration: GCONF获取
*********************************************************************************/
uint8_t TMC22xx_Get_GCONF(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	if (stTMC22xx.write_only) {
		*data = pstShadowReg->GCONF_sr;
		return 0;
	}

	TMC22xx_Uart_Read(motor, REG_GCONF);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;
	
	pstShadowReg->GCONF_sr = *data;
	
	return 1;
}
uint8_t GCONF_Get_I_scale_analog(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GCONF_sr & I_SCALE_ANALOG_bm) >> I_SCALE_ANALOG_bp;
}
uint8_t GCONF_Get_internal_Rsense(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GCONF_sr & INTERNAL_RSENSE_bm) >> INTERNAL_RSENSE_bp;
}
uint8_t GCONF_Get_en_spreadCycle(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GCONF_sr & EN_SPREADCYCLE_bm) >> EN_SPREADCYCLE_bp;
}
uint8_t GCONF_Get_shaft(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GCONF_sr & SHAFT_bm) >> SHAFT_bp;
}
uint8_t GCONF_Get_index_otpw(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GCONF_sr & INDEX_OTPW_bm) >> INDEX_OTPW_bp;
}
uint8_t GCONF_Get_index_step(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GCONF_sr & INDEX_STEP_bm) >> INDEX_STEP_bp;
}
uint8_t GCONF_Get_pdn_disable(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GCONF_sr & PDN_DISABLE_bm) >> PDN_DISABLE_bp;
}
uint8_t GCONF_Get_mstep_reg_select(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GCONF_sr & MSTEP_REG_SELECT_bm) >> MSTEP_REG_SELECT_bp;
}
uint8_t GCONF_Get_multistep_filt(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GCONF_sr & MULTISTEP_FILT_bm) >> MULTISTEP_FILT_bp;
}

/*********************************************************************************
* Function Declaration: GSTAT设置
*********************************************************************************/
void TMC22xx_Set_GSTAT(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GSTAT_sr = input;
	TMC22xx_Uart_Write(motor, REG_GSTAT, pstShadowReg->GSTAT_sr);
}
void GSTAT_Set_reset(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GSTAT_sr &= ~RESET_bm;
	pstShadowReg->GSTAT_sr |= ((uint32_t)B << RESET_bp) & RESET_bm;
	TMC22xx_Uart_Write(motor, REG_GSTAT, pstShadowReg->GSTAT_sr);
}
void GSTAT_Set_drv_err(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GSTAT_sr &= ~DRV_ERR_bm;
	pstShadowReg->GSTAT_sr |= ((uint32_t)B << DRV_ERR_bp) & DRV_ERR_bm;
	TMC22xx_Uart_Write(motor, REG_GSTAT, pstShadowReg->GSTAT_sr);
}
void GSTAT_Set_uv_cp(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->GSTAT_sr &= ~UV_CP_bm;
	pstShadowReg->GSTAT_sr |= ((uint32_t)B << UV_CP_bp) & UV_CP_bm;
	TMC22xx_Uart_Write(motor, REG_GSTAT, pstShadowReg->GSTAT_sr);
}

/*********************************************************************************
* Function Declaration: GSTAT获取
*********************************************************************************/
uint8_t TMC22xx_Get_GSTAT(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	if (stTMC22xx.write_only) {
		*data = pstShadowReg->GSTAT_sr;
		return 0;
	}
	
	TMC22xx_Uart_Read(motor, REG_GSTAT);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;
	
	pstShadowReg->GSTAT_sr = *data;
	
	return 1;
}
//uint8_t TMC22xx_Get_GSTAT()
//{
//	
//}
uint8_t GSTAT_Get_reset(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GSTAT_sr & RESET_bm) >> RESET_bp;
}
uint8_t GSTAT_Get_drv_err(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GSTAT_sr & DRV_ERR_bm) >> DRV_ERR_bp;
}
uint8_t GSTAT_Get_uv_cp(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->GSTAT_sr & UV_CP_bm) >> UV_CP_bp;
}

/*********************************************************************************
* Function Declaration: IFCNT获取
*********************************************************************************/
uint8_t TMC22xx_Get_IFCNT(uint8_t motor, uint32_t *data)
{
	TMC22xx_Uart_Read(motor, REG_IFCNT);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;

	return 0;
}

/*********************************************************************************
* Function Declaration: SLAVECONF设置
*********************************************************************************/
void TMC22xx_Set_SLAVECONF(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->SLAVECONF_sr = input&SLAVECONF_bm;
	TMC22xx_Uart_Write(motor, REG_SLAVECONF, pstShadowReg->SLAVECONF_sr);
}
void SLAVECONF_Set_senddelay(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->SLAVECONF_sr &= ~SENDDELAY_bm;
	pstShadowReg->SLAVECONF_sr |= ((uint32_t)B << SENDDELAY_bp) & SENDDELAY_bm;
	TMC22xx_Uart_Write(motor, REG_SLAVECONF, pstShadowReg->SLAVECONF_sr);
}

/*********************************************************************************
* Function Declaration: SLAVECONF获取
*********************************************************************************/
uint8_t TMC22xx_Get_SLAVECONF(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	*data = pstShadowReg->SLAVECONF_sr;
	return 0;
}
uint8_t SLAVECONF_Get_senddelay(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->SLAVECONF_sr & SENDDELAY_bm) >> SENDDELAY_bp;
}

/*********************************************************************************
* Function Declaration: OTP_PROG设置
*********************************************************************************/
void TMC22xx_Set_OTP_PROG(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->OTP_PROG_sr = input;
	TMC22xx_Uart_Write(motor, REG_OTP_PROG, pstShadowReg->OTP_PROG_sr);
}

/*********************************************************************************
* Function Declaration: OTP_READ获取
*********************************************************************************/
uint8_t TMC22xx_Get_OTP_READ(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	TMC22xx_Uart_Read(motor, REG_OTP_READ);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;
	
	pstShadowReg->OTP_READ_sr = *data;
	
	return 1;
}

/*********************************************************************************
* Function Declaration: IOIN获取
*********************************************************************************/
uint8_t TMC22xx_Get_IOIN(uint8_t motor, uint32_t *data)
{
	TMC22xx_Uart_Read(motor, REG_IOIN);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;

	return 0;
}
uint8_t IOIN_Get_enn(uint8_t motor)
{
	return (ENN_bm >> ENN_bp);
}
uint8_t IOIN_Get_ms1(uint8_t motor)
{
	return (MS1_bm >> MS1_bp);
}
uint8_t IOIN_Get_ms2(uint8_t motor)
{
	return (MS2_bm >> MS2_bp);
}
uint8_t IOIN_Get_diag(uint8_t motor)
{
	return (DIAG_bm >> DIAG_bp);
}
uint8_t IOIN_Get_pdn_uart(uint8_t motor)
{
	return (PDN_UART_bm >> PDN_UART_bp);
}
uint8_t IOIN_Get_step(uint8_t motor)
{
	return (STEP_bm >> STEP_bp);
}
uint8_t IOIN_Get_sel_a(uint8_t motor)
{
	return (SEL_A_bm >> SEL_A_bp);
}
uint8_t IOIN_Get_dir(uint8_t motor)
{
	return (DIR_bm >> DIR_bp);
}
uint8_t IOIN_Get_version(uint8_t motor)
{
	return (VERSION_bm >> VERSION_bp);
}

/*********************************************************************************
* Function Declaration: FACTORY_CONF设置
*********************************************************************************/
void TMC22xx_Set_FACTORY_CONF(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->FACTORY_CONF_sr = input;
	TMC22xx_Uart_Write(motor, REG_FACTORY_CONF, pstShadowReg->FACTORY_CONF_sr);
}
void FACTORY_CONF_Set_fclktrim(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->FACTORY_CONF_sr &= ~FCLKTRIM_bm;
	pstShadowReg->FACTORY_CONF_sr |= ((uint32_t)B << FCLKTRIM_bp) & FCLKTRIM_bm;
	TMC22xx_Uart_Write(motor, REG_FACTORY_CONF, pstShadowReg->FACTORY_CONF_sr);
}
void FACTORY_CONF_Set_ottrim(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->FACTORY_CONF_sr &= ~OTTRIM_bm;
	pstShadowReg->FACTORY_CONF_sr |= ((uint32_t)B << OTTRIM_bp) & OTTRIM_bm;
	TMC22xx_Uart_Write(motor, REG_FACTORY_CONF, pstShadowReg->FACTORY_CONF_sr);
}

/*********************************************************************************
* Function Declaration: FACTORY_CONF获取
*********************************************************************************/
uint8_t TMC22xx_Get_FACTORY_CONF(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	if (stTMC22xx.write_only) {
		*data = pstShadowReg->FACTORY_CONF_sr;
		return 0;
	}
	
	TMC22xx_Uart_Read(motor, REG_FACTORY_CONF);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;
	
	pstShadowReg->FACTORY_CONF_sr = *data;
	
	return 1;
}
uint8_t FACTORY_CONF_Get_fclktrim(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->FACTORY_CONF_sr & FCLKTRIM_bm) >> FCLKTRIM_bp;
}
uint8_t FACTORY_CONF_Get_ottrim(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->FACTORY_CONF_sr & OTTRIM_bm) >> OTTRIM_bp;
}

/*********************************************************************************
* Function Declaration: IHOLD_IRUN设置
*********************************************************************************/
void TMC22xx_Set_IHOLD_IRUN(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->IHOLD_IRUN_sr = input;
	TMC22xx_Uart_Write(motor, REG_IHOLD_IRUN, pstShadowReg->IHOLD_IRUN_sr);
}
void IHOLD_IRUN_Set_ihold(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->IHOLD_IRUN_sr &= ~IHOLD_bm;
	pstShadowReg->IHOLD_IRUN_sr |= ((uint32_t)B << IHOLD_bp) & IHOLD_bm;
	TMC22xx_Uart_Write(motor, REG_IHOLD_IRUN, pstShadowReg->IHOLD_IRUN_sr);
}
void IHOLD_IRUN_Set_irun(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->IHOLD_IRUN_sr &= ~IRUN_bm;
	pstShadowReg->IHOLD_IRUN_sr |= ((uint32_t)B << IRUN_bp) & IRUN_bm;
	TMC22xx_Uart_Write(motor, REG_IHOLD_IRUN, pstShadowReg->IHOLD_IRUN_sr);
}
void IHOLD_IRUN_Set_iholddelay(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->IHOLD_IRUN_sr &= ~IHOLDDELAY_bm;
	pstShadowReg->IHOLD_IRUN_sr |= ((uint32_t)B << IHOLDDELAY_bp) & IHOLDDELAY_bm;
	TMC22xx_Uart_Write(motor, REG_IHOLD_IRUN, pstShadowReg->IHOLD_IRUN_sr);
}

/*********************************************************************************
* Function Declaration: IHOLD_IRUN获取
*********************************************************************************/
uint8_t TMC22xx_Get_IHOLD_IRUN(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	*data = pstShadowReg->IHOLD_IRUN_sr;
	return 0;
}
uint8_t IHOLD_IRUN_Get_ihold(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->IHOLD_IRUN_sr & IHOLD_bm) >> IHOLD_bp;
}
uint8_t IHOLD_IRUN_Get_irun(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->IHOLD_IRUN_sr & IRUN_bm) >> IRUN_bp;
}
uint8_t IHOLD_IRUN_Get_iholddelay(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->IHOLD_IRUN_sr & IHOLDDELAY_bm) >> IHOLDDELAY_bp;
}

/*********************************************************************************
* Function Declaration: TPOWERDOWN设置
*********************************************************************************/
void TMC22xx_Set_TPOWERDOWN(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->TPOWERDOWN_sr = input;
	TMC22xx_Uart_Write(motor, REG_TPOWERDOWN, pstShadowReg->TPOWERDOWN_sr);
}

/*********************************************************************************
* Function Declaration: TPOWERDOWN获取
*********************************************************************************/
uint8_t TMC22xx_Get_TPOWERDOWN(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	*data = pstShadowReg->TPOWERDOWN_sr;
	return 0;
}

/*********************************************************************************
* Function Declaration: TSTEP获取
*********************************************************************************/
uint8_t TMC22xx_Get_TSTEP(uint8_t motor, uint32_t *data)
{
	TMC22xx_Uart_Read(motor, REG_TSTEP);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;

	return 0;
}

/*********************************************************************************
* Function Declaration: TPWMTHRS设置
*********************************************************************************/
void TMC22xx_Set_TPWMTHRS(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->TPWMTHRS_sr = input;
	TMC22xx_Uart_Write(motor, REG_TPWMTHRS, pstShadowReg->TPWMTHRS_sr);
}

/*********************************************************************************
* Function Declaration: TPWMTHRS获取
*********************************************************************************/
uint8_t TMC22xx_Get_TPWMTHRS(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	*data = pstShadowReg->TPWMTHRS_sr;
	return 0;
}

/*********************************************************************************
* Function Declaration: VACTUAL设置
*********************************************************************************/
void TMC22xx_Set_VACTUAL(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->VACTUAL_sr = input;
	TMC22xx_Uart_Write(motor, REG_VACTUAL, pstShadowReg->VACTUAL_sr);
}

/*********************************************************************************
* Function Declaration: TVACTUAL获取
*********************************************************************************/
uint8_t TMC22xx_Get_VACTUAL(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	*data = pstShadowReg->VACTUAL_sr;
	return 0;
}

/*********************************************************************************
* Function Declaration: MSCNT获取
*********************************************************************************/
uint8_t TMC22xx_Get_MSCNT(uint8_t motor, uint32_t *data)
{
	TMC22xx_Uart_Read(motor, REG_MSCNT);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;
		
	return 0;
}

/*********************************************************************************
* Function Declaration: MSCURACT获取
*********************************************************************************/
uint8_t TMC22xx_Get_MSCURACT(uint8_t motor, uint32_t *data)
{
	TMC22xx_Uart_Read(motor, REG_MSCURACT);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;

	return 0;
}
uint16_t MSCURACT_Get_cur_a(uint8_t motor)
{
	return (CUR_A_bm >> CUR_A_bp);
}
uint16_t MSCURACT_Get_cur_b(uint8_t motor)
{
	return (CUR_B_bm >> CUR_B_bp);
}

/*********************************************************************************
* Function Declaration: CHOPCONF设置
*********************************************************************************/
void TMC22xx_Set_CHOPCONF(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr = input;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}
void CHOPCONF_Set_toff(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr &= ~TOFF_bm;
	pstShadowReg->CHOPCONF_sr |= ((uint32_t)B << TOFF_bp) & TOFF_bm;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}
void CHOPCONF_Set_hstrt(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr &= ~HSTRT_bm;
	pstShadowReg->CHOPCONF_sr |= ((uint32_t)B << HSTRT_bp) & HSTRT_bm;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}
void CHOPCONF_Set_hysteresis_start(uint8_t motor, uint8_t value)
{
	CHOPCONF_Set_hstrt(motor, value-1);
}
void CHOPCONF_Set_hend(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr &= ~HEND_bm;
	pstShadowReg->CHOPCONF_sr |= ((uint32_t)B << HEND_bp) & HEND_bm;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}
void CHOPCONF_Set_hysteresis_end(uint8_t motor, int8_t value)
{
	CHOPCONF_Set_hend(motor, value+3);
}
void CHOPCONF_Set_tbl(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr &= ~TBL_bm;
	pstShadowReg->CHOPCONF_sr |= ((uint32_t)B << TBL_bp) & TBL_bm;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}
void CHOPCONF_Set_blank_time(uint8_t motor, uint8_t B)
{
	switch (B) {
		case 16: CHOPCONF_Set_tbl(motor, 0); break;
		case 24: CHOPCONF_Set_tbl(motor, 1); break;
		case 36: CHOPCONF_Set_tbl(motor, 2); break;
		case 54: CHOPCONF_Set_tbl(motor, 3); break;
	}
}
void CHOPCONF_Set_vsense(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr &= ~VSENSE_bm;
	pstShadowReg->CHOPCONF_sr |= ((uint32_t)B << VSENSE_bp) & VSENSE_bm;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}
void CHOPCONF_Set_mres(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr &= ~MRES_bm;
	pstShadowReg->CHOPCONF_sr |= ((uint32_t)B << MRES_bp) & MRES_bm;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}
void CHOPCONF_Set_intpol(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr &= ~INTPOL_bm;
	pstShadowReg->CHOPCONF_sr |= ((uint32_t)B << INTPOL_bp) & INTPOL_bm;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}
void CHOPCONF_Set_dedge(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr &= ~DEDGE_bm;
	pstShadowReg->CHOPCONF_sr |= ((uint32_t)B << DEDGE_bp) & DEDGE_bm;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}
void CHOPCONF_Set_diss2g(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr &= ~DISS2G_bm;
	pstShadowReg->CHOPCONF_sr |= ((uint32_t)B << DISS2G_bp) & DISS2G_bm;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}
void CHOPCONF_Set_diss2vs(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->CHOPCONF_sr &= ~DISS2VS_bm;
	pstShadowReg->CHOPCONF_sr |= ((uint32_t)B << DISS2VS_bp) & DISS2VS_bm;
	TMC22xx_Uart_Write(motor, REG_CHOPCONF, pstShadowReg->CHOPCONF_sr);
}

/*********************************************************************************
* Function Declaration: CHOPCONF获取
*********************************************************************************/
uint8_t TMC22xx_Get_CHOPCONF(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	if (stTMC22xx.write_only) {
		*data = pstShadowReg->CHOPCONF_sr;
		return 0;
	}
	
	TMC22xx_Uart_Read(motor, REG_CHOPCONF);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;
	
	pstShadowReg->CHOPCONF_sr = *data;
	
	return 1;
}
uint8_t CHOPCONF_Get_toff(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->CHOPCONF_sr & TOFF_bm) >> TOFF_bp;
}
uint8_t CHOPCONF_Get_hstrt(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->CHOPCONF_sr & HSTRT_bm) >> HSTRT_bp;
}
uint8_t CHOPCONF_Get_hysteresis_start(uint8_t motor)
{
	return CHOPCONF_Get_hstrt(motor)+1;
}
uint8_t CHOPCONF_Get_hend(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->CHOPCONF_sr & HEND_bm) >> HEND_bp;
}
int8_t CHOPCONF_Get_hysteresis_end(uint8_t motor)
{
	return CHOPCONF_Get_hend(motor)-3;
}
uint8_t CHOPCONF_Get_tbl(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->CHOPCONF_sr & TBL_bm) >> TBL_bp;
}
uint8_t CHOPCONF_Get_blank_time(uint8_t motor)
{
	switch (CHOPCONF_Get_tbl(motor)) {
		case 0: return 16;
		case 1: return 24;
		case 2: return 36;
		case 3: return 54;
	}
	return 0;
}
uint8_t CHOPCONF_Get_vsense(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->CHOPCONF_sr & VSENSE_bm) >> VSENSE_bp;
}
uint8_t CHOPCONF_Get_mres(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->CHOPCONF_sr & MRES_bm) >> MRES_bp;
}
uint8_t CHOPCONF_Get_intpol(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->CHOPCONF_sr & INTPOL_bm) >> INTPOL_bp;
}
uint8_t CHOPCONF_Get_dedge(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->CHOPCONF_sr & IRUN_bm) >> IRUN_bp;
}
uint8_t CHOPCONF_Get_diss2g(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->CHOPCONF_sr & DISS2G_bm) >> DISS2G_bp;
}
uint8_t CHOPCONF_Get_diss2vs(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->CHOPCONF_sr & DISS2VS_bm) >> DISS2VS_bp;
}

/*********************************************************************************
* Function Declaration: DRV_STATUS获取
*********************************************************************************/
uint8_t TMC22xx_Get_DRV_STATUS(uint8_t motor, uint32_t *data)
{
	if (stTMC22xx.write_only) return 1;
	TMC22xx_Uart_Read(motor, REG_DRV_STATUS);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;


	return 0;
}
//uint32_t DRV_STATUS_Get_DRV_STATUS()
//{
//	
//}
uint8_t DRV_STATUS_Get_otpw(uint8_t motor)
{
	return (OTPW_bm >> OTPW_bp);
}
uint8_t DRV_STATUS_Get_ot(uint8_t motor)
{
	return (OT_bm >> OT_bp);
}
uint8_t DRV_STATUS_Get_s2ga(uint8_t motor)
{
	return (S2GA_bm >> S2GA_bp);
}
uint8_t DRV_STATUS_Get_s2gb(uint8_t motor)
{
	return (S2GB_bm >> S2GB_bp);
}
uint8_t DRV_STATUS_Get_s2vsa(uint8_t motor)
{
	return (S2VSA_bm >> S2VSA_bp);
}
uint8_t DRV_STATUS_Get_s2vsb(uint8_t motor)
{
	return (S2VSB_bm >> S2VSB_bp);
}
uint8_t DRV_STATUS_Get_ola(uint8_t motor)
{
	return (OLA_bm >> OLA_bp);
}
uint8_t DRV_STATUS_Get_olb(uint8_t motor)
{
	return (OLB_bm >> OLB_bp);
}
uint8_t DRV_STATUS_Get_t120(uint8_t motor)
{
	return (T120_bm >> T120_bp);
}
uint8_t DRV_STATUS_Get_t143(uint8_t motor)
{
	return (T143_bm >> T143_bp);
}
uint8_t DRV_STATUS_Get_t150(uint8_t motor)
{
	return (T150_bm >> T150_bp);
}
uint8_t DRV_STATUS_Get_t157(uint8_t motor)
{
	return (T157_bm >> T157_bp);
}
uint16_t DRV_STATUS_Get_cs_actual(uint8_t motor)
{
	return (CS_ACTUAL_bm >> CS_ACTUAL_bp);
}
uint8_t DRV_STATUS_Get_stealth(uint8_t motor)
{
	return (STEALTH_bm >> STEALTH_bp);
}
uint8_t DRV_STATUS_Get_stst(uint8_t motor)
{
	return (STST_bm >> STST_bp);
}

/*********************************************************************************
* Function Declaration: PWMCONF设置
*********************************************************************************/
void TMC22xx_Set_PWMCONF(uint8_t motor, uint32_t input)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->PWMCONF_sr = input;
	TMC22xx_Uart_Write(motor, REG_PWMCONF, pstShadowReg->PWMCONF_sr);
}
void PWMCONF_Set_pwm_ofs(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->PWMCONF_sr &= ~PWM_OFS_bm;
	pstShadowReg->PWMCONF_sr |= ((uint32_t)B << PWM_OFS_bp) & PWM_OFS_bm;
	TMC22xx_Uart_Write(motor, REG_PWMCONF, pstShadowReg->PWMCONF_sr);
}
void PWMCONF_Set_pwm_grad(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->PWMCONF_sr &= ~PWM_GRAD_bm;
	pstShadowReg->PWMCONF_sr |= ((uint32_t)B << PWM_GRAD_bp) & PWM_GRAD_bm;
	TMC22xx_Uart_Write(motor, REG_PWMCONF, pstShadowReg->PWMCONF_sr);
}
void PWMCONF_Set_pwm_freq(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->PWMCONF_sr &= ~PWM_FREQ_bm;
	pstShadowReg->PWMCONF_sr |= ((uint32_t)B << PWM_FREQ_bp) & PWM_FREQ_bm;
	TMC22xx_Uart_Write(motor, REG_PWMCONF, pstShadowReg->PWMCONF_sr);
}
void PWMCONF_Set_pwm_autoscale(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->PWMCONF_sr &= ~PWM_AUTOSCALE_bm;
	pstShadowReg->PWMCONF_sr |= ((uint32_t)B << PWM_AUTOSCALE_bp) & PWM_AUTOSCALE_bm;
	TMC22xx_Uart_Write(motor, REG_PWMCONF, pstShadowReg->PWMCONF_sr);
}
void PWMCONF_Set_pwm_autograd(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->PWMCONF_sr &= ~PWM_AUTOGRAD_bm;
	pstShadowReg->PWMCONF_sr |= ((uint32_t)B << PWM_AUTOGRAD_bp) & PWM_AUTOGRAD_bm;
	TMC22xx_Uart_Write(motor, REG_PWMCONF, pstShadowReg->PWMCONF_sr);
}
void PWMCONF_Set_freewheel(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->PWMCONF_sr &= ~FREEWHEEL_bm;
	pstShadowReg->PWMCONF_sr |= ((uint32_t)B << TOFF_bp) & FREEWHEEL_bm;
	TMC22xx_Uart_Write(motor, REG_PWMCONF, pstShadowReg->PWMCONF_sr);
}
void PWMCONF_Set_pwm_reg(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->PWMCONF_sr &= ~PWM_REG_bm;
	pstShadowReg->PWMCONF_sr |= ((uint32_t)B << PWM_REG_bp) & PWM_REG_bm;
	TMC22xx_Uart_Write(motor, REG_PWMCONF, pstShadowReg->PWMCONF_sr);
}
void PWMCONF_Set_pwm_lim(uint8_t motor, uint8_t B)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	pstShadowReg->PWMCONF_sr &= ~PWM_LIM_bm;
	pstShadowReg->PWMCONF_sr |= ((uint32_t)B << PWM_LIM_bp) & PWM_LIM_bm;
	TMC22xx_Uart_Write(motor, REG_PWMCONF, pstShadowReg->PWMCONF_sr);
}

/*********************************************************************************
* Function Declaration: PWMCONF获取
*********************************************************************************/
uint8_t TMC22xx_Get_PWMCONF(uint8_t motor, uint32_t *data)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	if (stTMC22xx.write_only) {
		*data = pstShadowReg->PWMCONF_sr;
		return 0;
	}
	
	TMC22xx_Uart_Read(motor, REG_PWMCONF);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;
		
	pstShadowReg->PWMCONF_sr = *data;
	
	return 1;
}
uint8_t PWMCONF_Get_pwm_ofs(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->PWMCONF_sr & PWM_OFS_bm) >> PWM_OFS_bp;
}
uint8_t PWMCONF_Get_pwm_grad(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->PWMCONF_sr & PWM_GRAD_bm) >> PWM_GRAD_bp;
}
uint8_t PWMCONF_Get_pwm_freq(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->PWMCONF_sr & PWM_FREQ_bm) >> PWM_FREQ_bp;
}
uint8_t PWMCONF_Get_pwm_autoscale(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->PWMCONF_sr & PWM_AUTOSCALE_bm) >> PWM_AUTOSCALE_bp;
}
uint8_t PWMCONF_Get_pwm_autograd(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->PWMCONF_sr & PWM_AUTOGRAD_bm) >> PWM_AUTOGRAD_bp;
}
uint8_t PWMCONF_Get_freewheel(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->PWMCONF_sr & FREEWHEEL_bm) >> FREEWHEEL_bp;
}
uint8_t PWMCONF_Get_pwm_reg(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->PWMCONF_sr & PWM_REG_bm) >> PWM_REG_bp;
}
uint8_t PWMCONF_Get_pwm_lim(uint8_t motor)
{
	SHADOW_REGISTER_S *pstShadowReg;
	
	pstShadowReg = TMC22xx_Motor_ShadowRegisterSelect(motor);
	
	return (pstShadowReg->PWMCONF_sr & PWM_LIM_bm) >> PWM_LIM_bp;
}

/*********************************************************************************
* Function Declaration: PWM_SCALE获取
*********************************************************************************/
uint8_t PWM_SCALE_Get_PWM_SCALE(uint8_t motor, uint32_t *data)
{
	TMC22xx_Uart_Read(motor, REG_PWM_SCALE);
	delay_ms(50);
	if (motor)
		*data = stTMC22xx_Recv.til[3] << 24 | stTMC22xx_Recv.til[4] << 16 | stTMC22xx_Recv.til[5] << 8 | stTMC22xx_Recv.til[6] << 0;
	else
		*data = stTMC22xx_Recv.pan[3] << 24 | stTMC22xx_Recv.pan[4] << 16 | stTMC22xx_Recv.pan[5] << 8 | stTMC22xx_Recv.pan[6] << 0;
		
	
	return 0;
}
uint8_t PWM_SCALE_Get_pwm_scale_sum(uint8_t motor)
{
	return (PWM_SCALE_SUM_bm >> PWM_SCALE_SUM_bp);
}
int16_t PWM_SCALE_Get_pwm_scale_auto(uint8_t motor)
{
	// Not two's complement? 9nth bit determines sign
	uint32_t d;
	PWM_SCALE_Get_PWM_SCALE(motor, &d);
	int16_t response = (d>>PWM_SCALE_AUTO_bp)&0xFF;
	if (((d&PWM_SCALE_AUTO_bm) >> 24) & 0x1) return -response;
	else return response;
}
