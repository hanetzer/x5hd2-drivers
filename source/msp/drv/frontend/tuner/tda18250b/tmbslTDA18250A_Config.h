/*
Copyright (C) 2010 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmbslTDA18250A_Config.h
*
* \date          %modify_time%
*
* \author        Alexandre TANT
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS :
*                TDA18250A_Driver_User_Guide.pdf
*
* \section info Change Information
*
*/

#ifndef _TDA18250A_CONFIG_H
#define _TDA18250A_CONFIG_H


#ifdef __cplusplus
extern "C"
{
#endif

    /*============================================================================*/
    /* Types and defines:                                                         */
    /*============================================================================*/

    /* Driver settings version definition */
#define TDA18250A_SETTINGS_CUSTOMER_NUM      (0)                     /* SW Settings Customer Number */
#define TDA18250A_SETTINGS_PROJECT_NUM       (0)                     /* SW Settings Project Number  */
#define TDA18250A_SETTINGS_MAJOR_VER         (0)                     /* SW Settings Major Version   */
#define TDA18250A_SETTINGS_MINOR_VER         (27)                     /* SW Settings Minor Version   */

#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX 19
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX 12
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX 14
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX 8

#define TDA18250A_RF_MAX         1200000000 

#define TDA18250A_BP_FILTER_1      39424000 
#define TDA18250A_BP_FILTER_2      61952000
#define TDA18250A_BP_FILTER_3      86528000
#define TDA18250A_BP_FILTER_4     123392000
#define TDA18250A_BP_FILTER_5     172544000
#define TDA18250A_BP_FILTER_6     244222000
#define TDA18250A_BP_FILTER_7     320000000

/* {    X     ,    X    ,   X  ,           R           },         */
/* {    0     ,    1    ,   2  ,           F           },         */
/* {          ,         ,      ,                       },         */
/* {    x     ,    x    ,   x  ,                       },         */
/* {    1     ,    1    ,   1  ,           M           },         */
/* {    0     ,    0    ,   0  ,           H           },         */
/* {    0     ,    0    ,   0  ,           z           },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {          ,         ,   0  ,                       },         */
/* {          ,         ,      ,                       },         */
/* {          ,         ,      ,                       },         */
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX_NB    \
{                                                              \
   { 50544946,   314452,  -25806,     TDA18250A_BP_FILTER_1  }, \
   { 50544946,   314452,  -25806,     TDA18250A_BP_FILTER_2  }, \
   { 51239049,   232294,  -13936,     TDA18250A_BP_FILTER_3  }, \
   { 52388115,   156738,   -7029,     TDA18250A_BP_FILTER_4  }, \
   { 53961378,    99065,   -3425,     TDA18250A_BP_FILTER_5  }, \
   { 59788149,     6642,    -150,     TDA18250A_BP_FILTER_6  }, \
   { 53121988,    47726,    -805,     TDA18250A_BP_FILTER_7  }, \
   { 60101688,      140,    -154,     335360000              }, \
   { 65022437,   -32531,     334,     390656000              }, \
   { 54268908,    22541,    -372,     430592000              }, \
   { 73524942,   -64465,     641,     473500000              }, \
   { 52585046,    24369,    -326,     535040000              }, \
   { 68451433,   -35142,     254,     562688000              }, \
   { 69310100,   -38493,     264,     600000000              }, \
   { 50755238,    18868,    -176,     654848000              }, \
   { 42143765,    45877,    -383,     720384000              }, \
   { 92511767,   -93070,     592,     793088000              }, \
   { 56275795,     4022,     -69,     865792000              }, \
   { 47648303,    24175,    -174,     TDA18250A_RF_MAX}         \
}

/* {    X     ,    X    ,   X  ,           A           },                */
/* {    0     ,    1    ,   2  ,           G           },                */
/* {          ,         ,      ,           C           },                */
/* {    x     ,    x    ,   x  ,           1           },                */
/* {    1     ,    1    ,   1  ,           N           },                */
/* {    0     ,    0    ,   0  ,           B           },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {          ,         ,   0  ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB						                   \
{                                                                                \
   { -351128740,      10662,   -123,      TDA18250AAGC1_GAIN_Minus_11dB   }, \
   { -325410065,       5753,    -28,      TDA18250AAGC1_GAIN_Minus_8dB    }, \
   { -294228073,     -14490,    220,      TDA18250AAGC1_GAIN_Minus_5dB    }, \
   { -264405950,      -7071,    134,      TDA18250AAGC1_GAIN_Minus_2dB    }, \
   { -229138590,       6509,    -20,      TDA18250AAGC1_GAIN_Plus_1dB         }, \
   { -202637443,     -13125,    208,      TDA18250AAGC1_GAIN_Plus_4dB         }, \
   { -175582583,      20477,   -175,      TDA18250AAGC1_GAIN_Plus_7dB         }, \
   { -143582193,      19323,   -155,      TDA18250AAGC1_GAIN_Plus_10dB        }, \
   { -112577197,      17126,   -108,      TDA18250AAGC1_GAIN_Plus_13dB        }, \
   { -78464937,       13220,    -99,      TDA18250AAGC1_GAIN_Plus_16dB        }, \
   { -44181980,        8145,    -24,      TDA18250AAGC1_GAIN_Plus_19dB       }, \
   {         0,           0,      0,      TDA18250AAGC1_GAIN_Plus_22dB       }  \
}

/* {    X     ,    X    ,   X  ,           A           },                */
/* {    0     ,    1    ,   2  ,           G           },                */
/* {          ,         ,      ,           C           },                */
/* {    x     ,    x    ,   x  ,           2           },                */
/* {    1     ,    1    ,   1  ,           N           },                */
/* {    0     ,    0    ,   0  ,           B           },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {          ,         ,   0  ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC2NB  \
{                                                    \
   { -262186,    2085,    -120,       10    },       \
   { -264194,    2079,    -120,       21    },       \
   { -266973,    2079,    -119,       32    },       \
   { -269833,    2078,    -116,       43    },       \
   { -272289,    2076,    -114,       54    },       \
   { -274733,    2070,    -99,        65    },       \
   { -277588,    2067,    -81,        76    },       \
   { -280562,    2063,    -62,        87    },       \
   { -282784,    2054,    -43,        98    },       \
   { -284701,    2043,    -30,       109    },       \
   { -284809,    2027,    -22,       120    },       \
   { -283553,    2007,    -14,       131    },       \
   {  -281681,    1984,     0,       142    }        \
}

/* {    X     ,    X    ,   X  ,           A           },                */
/* {    0     ,    1    ,   2  ,           G           },                */
/* {          ,         ,      ,           C           },                */
/* {    x     ,    x    ,   x  ,           3           },                */
/* {    1     ,    1    ,   1  ,           N           },                */
/* {    0     ,    0    ,   0  ,           B           },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {          ,         ,   0  ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC3NB						        \
{                                                                               \
   { -208116100,   -688,     2,    TDA18250AAGC3_GAIN_Minus_6dB      }, \
   { -178917030,   -732,     3,    TDA18250AAGC3_GAIN_Minus_3dB      }, \
   { -148438740,   -798,     3,    TDA18250AAGC3_GAIN_0dB            }, \
   { -118542815,   -777,     3,    TDA18250AAGC3_GAIN_Plus_3dB      }, \
   {  -88269478,   -778,     2,    TDA18250AAGC3_GAIN_Plus_6dB       }, \
   {  -58950548,   -606,     1,    TDA18250AAGC3_GAIN_Plus_9dB       }, \
   {  -29234591,   -457,     1,    TDA18250AAGC3_GAIN_Plus_12dB      }, \
   {          0,      0,     0,    TDA18250AAGC3_GAIN_Plus_15dB      }  \
}

    /* Standard Preset Definitions: */
#define TDA18250A_CONFIG_STD_QAM_6MHZ \
    {                                                                        /* QAM 6MHz */            \
    /****************************************************************/                            \
    /* IF Settings                                                  */                            \
    /****************************************************************/                            \
    3500000,                                                            /* IF */                  \
    0,                                                                  /* CF_Offset */           \
    \
    /****************************************************************/                            \
    /* IF SELECTIVITY Settings                                      */                            \
    /****************************************************************/                            \
    TDA18250A_LPF_6MHz,                                                 /* LPF */                 \
    TDA18250A_LPFOffset_min_4pc,                                        /* LPF_Offset */          \
    TDA18250A_DC_Notch_IF_PPF_Disabled,                                 /* DC notch IF PPF */     \
    TDA18250A_HPF_0_5MHz,                                               /* Hi Pass */             \
    TDA18250A_HPFOffset_plus_12pc,                                            /* HPF Offset */          \
    TDA18250A_IF_Notch_Enabled,                                        /* IF notch */            \
    TDA18250A_IF_Notch_Freq_6_25MHz,                                    /* IF Notch Freq */       \
    TDA18250A_IF_Notch_Offset_plus_8pc,                                      /* IF Notch Offset */     \
    TDA18250A_IFnotchToRSSI_Disabled,                                    /* IF notch To RSSI */    \
    \
    /****************************************************************/                            \
    /* AGC Settings                                                 */                            \
    /****************************************************************/                            \
    TDA18250AAGC1_GAIN_Free_Frozen,                                     /* AGC1 GAIN */           \
    TDA18250AAGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d102_u96dBuV,                          /* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_0,                                      /* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_62_5KHz,                                   /* AGC1 Det Speed */      \
    TDA18250A_AGC1_SMOOTH_T_CST_51_2ms,                                 /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_S11,                                              /* LNA_Zin */             \
    False,                                                              /* AGC2 Gain Control En */ \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)102,                                                                                  \
            (UInt8)101                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                                   \
            (UInt8)104,                                                                                   \
            (UInt8)103                                                                                    \
        }                                                                                          \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_62_5KHz,                                   /* AGC2 Det Speed */      \
    False,                                                              /* AGC2 Adapt TOP23 Enable */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    TDA18250A_AGC2_Do_Step_1048ms,                                      /* AGC2 Do Step  */                \
    TDA18250AAGC2_Up_Udld_Step_65ms,                                    /* AGC2 Up Udld Step */ \
    -7,                                                                 /* AGC2 TOP Up Udld */ \
    6,                                                                  /* AGC2 Fast Auto Delta */ \
    TDA18250A_DET12_CINT_200fF,                                         /* Det12 Cint */          \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,                          /* IF Output Level */     \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_6dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                         \
    },                                                                                            \
    \
    /****************************************************************/                            \
    /* GSK Settings                                                 */                            \
    /****************************************************************/                            \
    TDA18250A_AGCK_Time_Constant_8_192ms,                              /* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,                                          /* RSSI HP FC */          \
    \
    /****************************************************************/                            \
    /* H3H5 Settings                                                */                            \
    /****************************************************************/                            \
    TDA18250A_VHF_III_Mode_Disabled,                                    /* VHF III Mode */        \
    \
    /****************************************************************/                            \
    /*RSSI Settings                                                 */                            \
    /****************************************************************/                            \
    TDA18250A_RSSI_CAP_VAL_3pF,                                         /* RSSI_Ck_speed */       \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,                                   /* RSSI_Cap_Val */        \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
}

#define TDA18250A_CONFIG_STD_QAM_8MHZ \
    {																		/* QAM 8MHz */			  \
    /****************************************************************/							  \
    /* IF Settings                                                  */							  \
    /****************************************************************/							  \
    5000000,															/* IF */				  \
    0,																	/* CF_Offset */			  \
    \
    /****************************************************************/							  \
    /* IF SELECTIVITY Settings                                      */							  \
    /****************************************************************/							  \
    TDA18250A_LPF_9MHz,													/* LPF */				  \
    TDA18250A_LPFOffset_min_4pc,										/* LPF_Offset */		  \
    TDA18250A_DC_Notch_IF_PPF_Disabled,                                 /* DC notch IF PPF */     \
    TDA18250A_HPF_1MHz,                                                 /* Hi Pass */			  \
    TDA18250A_HPFOffset_plus_4pc,                                       /* HPF Offset */		  \
    TDA18250A_IF_Notch_Enabled,                                         /* IF notch */			  \
    TDA18250A_IF_Notch_Freq_9_25MHz,									/* IF Notch Freq */		  \
    TDA18250A_IF_Notch_Offset_0pc,										/* IF Notch Offset */     \
    TDA18250A_IFnotchToRSSI_Disabled,									/* IF notch To RSSI */    \
    \
    /****************************************************************/							  \
    /* AGC Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250AAGC1_GAIN_Free_Frozen,                                     /* AGC1 GAIN */           \
    TDA18250AAGC1_GAIN_SMOOTH_ALGO_Enabled,                             /* AGC1 GAIN SMOOTH ALGO */ \
    TDA18250A_AGC1_TOP_I2C_DN_UP_d102_u96dBuV,							/* AGC1 TOP I2C DN/UP */  \
    TDA18250A_AGC1_TOP_STRATEGY_0,										/* AGC1 TOP STRATEGY 0 */ \
    TDA18250A_AGC1_DET_SPEED_62_5KHz,									/* AGC1 Det Speed */	  \
    TDA18250A_AGC1_SMOOTH_T_CST_51_2ms,                                 /* AGC1 Smooth T Cst */   \
    TDA18250A_LNA_ZIN_S11,												/* LNA_Zin */			  \
    False,                                                              /* AGC2 Gain Control En */ \
    {                                                                   /* AGC2 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)102,                                                                                  \
            (UInt8)101                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)320000000,                                                                                   \
            (UInt8)104,                                                                                   \
            (UInt8)103                                                                                    \
        }                                                                                          \
    },                                                                                            \
    TDA18250A_AGC2_DET_SPEED_62_5KHz,									/* AGC2 Det Speed */	  \
    False,                                                              /* AGC2 Adapt TOP23 Enable */     \
    0,                                                                  /* AGC2 Adapt TOP23 Delta (in dB) */     \
    True,                                                               /* AGC2 Gain Control Speed (False 1ms ; True 0.5ms)   */     \
    TDA18250A_AGC2_Do_Step_1048ms,                                      /* AGC2 Do Step  */                \
    TDA18250AAGC2_Up_Udld_Step_65ms,                                    /* AGC2 Up Udld Step */ \
    -7,                                                                 /* AGC2 TOP Up Udld */ \
    6,                                                                  /* AGC2 Fast Auto Delta */ \
    TDA18250A_DET12_CINT_200fF,											/* Det12 Cint */		  \
    {                                                                   /* AGC3 TOP */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            (UInt8)110,                                                                                  \
            (UInt8)105                                                                                   \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            (UInt8)-1,                                                                                   \
            (UInt8)-1                                                                                    \
        }                                                                                         \
    },                                                                                            \
    TDA18250A_IF_Output_Level_1Vpp_min_6_24dB,							/* IF Output Level */     \
    {                                                                   /* S2D Gain */            \
        {                                                                                         \
            (UInt32)0,                                                                                    \
            TDA18250A_S2D_Gain_6dB                                                                \
        },                                                                                        \
        {                                                                                         \
            (UInt32)-1,                                                                                   \
            TDA18250A_S2D_Gain_6dB                                                                \
        }                                                                                         \
    },                                                                                            \
    \
    /****************************************************************/							  \
    /* GSK Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_AGCK_Time_Constant_8_192ms,								/* AGCK Time Constant */  \
    TDA18250A_RSSI_HP_FC_0_3M,											/* RSSI HP FC */		  \
    \
    /****************************************************************/							  \
    /* H3H5 Settings                                                */							  \
    /****************************************************************/							  \
    TDA18250A_VHF_III_Mode_Disabled,									/* VHF III Mode */		  \
    \
    /****************************************************************/							  \
    /*RSSI Settings                                                 */							  \
    /****************************************************************/							  \
    TDA18250A_RSSI_CAP_VAL_3pF,											/* RSSI_Ck_speed */		  \
    TDA18250A_RSSI_CK_SPEED_31_25kHz,									/* RSSI_Cap_Val */		  \
    \
    0x44,                                                               /* ES1 Power Saving Byte 1 */ \
    0x06,                                                               /* ES1 Power Saving Byte 2 */ \
    0x07,                                                               /* ES1 Power Saving Byte 3 */ \
    0x88,                                                               /* ES2 Power Saving Byte 1 */ \
    0x42,                                                               /* ES2 Power Saving Byte 2 */ \
    0x0B,                                                               /* ES2 Power Saving Byte 3 */ \
}


    /* Default configuration */
#define TDA18250A_CONFIG_CURPOWERSTATE_DEF tmPowerMax
#define TDA18250A_CONFIG_CURLLPOWERSTATE_DEF TDA18250A_PowerMax
#define TDA18250A_CONFIG_RF_DEF 0
#define TDA18250A_CONFIG_PROG_RF_DEF 0
#define TDA18250A_CONFIG_STANDARDMODE_DEF TDA18250A_StandardMode_Max

    /* Power settings */
#define TDA18250A_CONFIG_POWER_DEF_MASTER \
    TDA18250A_CONFIG_CURPOWERSTATE_DEF,                     /* curPowerState */       \
    TDA18250A_CONFIG_CURLLPOWERSTATE_DEF,                   /* curLLPowerState */     \
    {                                                       /* mapLLPowerState */     \
    TDA18250A_PowerNormalMode,                          /* tmPowerOn (D0) */      \
    TDA18250A_PowerStandbyWithLtOnWithXtalOn,           /* tmPowerStandby (D1) */ \
    TDA18250A_PowerStandbyWithLtOnWithXtalOn,           /* tmPowerSuspend (D2) */ \
    TDA18250A_PowerStandbyWithLtOnWithXtalOn            /* tmPowerOff (D3) */     \
}

#define TDA18250A_CONFIG_POWER_DEF_SLAVE \
    TDA18250A_CONFIG_CURPOWERSTATE_DEF,                     /* curPowerState */       \
    TDA18250A_CONFIG_CURLLPOWERSTATE_DEF,                   /* curLLPowerState */     \
    {                                                       /* mapLLPowerState */     \
    TDA18250A_PowerNormalMode,                          /* tmPowerOn (D0) */      \
    TDA18250A_PowerStandbyWithXtalOn,			        /* tmPowerStandby (D1) */ \
    TDA18250A_PowerStandbyWithXtalOn,					/* tmPowerSuspend (D2) */ \
    TDA18250A_PowerStandbyWithXtalOn					/* tmPowerOff (D3) */     \
}

    /* Standard Presets Aggregation: */
#define TDA18250A_CONFIG_STD_DEF \
    { \
    TDA18250A_CONFIG_STD_QAM_6MHZ,    \
    TDA18250A_CONFIG_STD_QAM_8MHZ,    \
}


#define TDA18250A_CONFIG_REGMAP_DEF \
    { \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0           \
}


#define TDA18250A_CONFIG_MASTER \
    TDA18250A_CONFIG_POWER_DEF_MASTER, \
    TDA18250A_CONFIG_RF_DEF,                                /* uRF */                            \
    TDA18250A_CONFIG_PROG_RF_DEF,                           /* uProgRF */                        \
    TDA18250A_CONFIG_STANDARDMODE_DEF,                      /* StandardMode */                   \
    Null,                                                   /* pStandard */                      \
    False,                                                  /* bBufferMode */                    \
    False,                                                  /* Manual PLL Calc */                \
    TDA18250A_LoopThrough_Enabled,                          /* single app 18274 */               \
    TDA18250A_Mode_Dual,                                    /* single app */                     \
    TDA18250A_XtalFreq_16000000,                            /* eXtalFreq */                      \
    TDA18250A_XTOUT_Amp_400mV,                              /* eXTOutAmp */                      \
    TDA18250A_XTOUT_Sinusoidal,                             /* eXTOut */                         \
    TDA18250A_LNA_RANGE_Minus8dB_Plus22dB,					/* eLNA_Range */                     \
    TDA18250A_IRQ_POLARITY_LOW,                             /* eIRQPolarity */                   \
    TDA18250A_HwState_InitNotDone,                          /* eHwState */                       \
    TDA18250A_CONFIG_STD_DEF, \
    TDA18250A_CONFIG_REGMAP_DEF

#define TDA18250A_CONFIG_SLAVE \
    TDA18250A_CONFIG_POWER_DEF_SLAVE, \
    TDA18250A_CONFIG_RF_DEF,                                /* uRF */                            \
    TDA18250A_CONFIG_PROG_RF_DEF,                           /* uProgRF */                        \
    TDA18250A_CONFIG_STANDARDMODE_DEF,                      /* StandardMode */                   \
    Null,                                                   /* pStandard */                      \
    True,                                                   /* bBufferMode */                    \
    False,                                                  /* Manual PLL Calc */                \
    TDA18250A_LoopThrough_Enabled,                          /* single app 18274 */               \
    TDA18250A_Mode_Dual,                                    /* single app */                     \
    TDA18250A_XtalFreq_16000000,                            /* eXtalFreq */                      \
    TDA18250A_XTOUT_Amp_400mV,                              /* eXTOutAmp */                      \
    TDA18250A_XTOUT_Sinusoidal,                             /* eXTOut */                         \
    TDA18250A_LNA_RANGE_Minus8dB_Plus22dB,					/* eLNA_Range */                     \
    TDA18250A_IRQ_POLARITY_LOW,                             /* eIRQPolarity */                   \
    TDA18250A_HwState_InitNotDone,                          /* eHwState */                       \
    TDA18250A_CONFIG_STD_DEF, \
    TDA18250A_CONFIG_REGMAP_DEF


    /* Custom Driver Instance Parameters: (Path 0) */
#define TDA18250A_CONFIG_0 \
    TDA18250A_CONFIG_MASTER

    /* Custom Driver Instance Parameters: (Path 1) */
#define TDA18250A_CONFIG_1 \
    TDA18250A_CONFIG_SLAVE


#ifdef __cplusplus
}
#endif

#endif /* _TDA18250A_CONFIG_H */

