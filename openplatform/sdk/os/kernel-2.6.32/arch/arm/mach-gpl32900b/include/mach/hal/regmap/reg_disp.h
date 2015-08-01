/**************************************************************************
 *                                                                        *
 *         Copyright (c) 2010 by Generalplus Technology Co., Ltd.         *
 *                                                                        *
 *  This software is copyrighted by and is the property of Generalplus    *
 *  Technology Co., Ltd. All rights are reserved by Generalplus Technology*
 *  Co., Ltd. This software may only be used in accordance with the       *
 *  corresponding license agreement. Any unauthorized use, duplication,   *
 *  distribution, or disclosure of this software is expressly forbidden.  *
 *                                                                        *
 *  This Copyright notice MUST not be removed or modified without prior   *
 *  written consent of Generalplus Technology Co., Ltd.                   *
 *                                                                        *
 *  Generalplus Technology Co., Ltd. reserves the right to modify this    *
 *  software without notice.                                              *
 *                                                                        *
 *  Generalplus Technology Co., Ltd.                                      *
 *  19, Innovation First Road, Science-Based Industrial Park,             *
 *  Hsin-Chu, Taiwan, R.O.C.                                              *
 *                                                                        *
 **************************************************************************/
#ifndef _REG_DISP_H_
#define _REG_DISP_H_

#include <mach/hardware.h>
#include <mach/typedef.h>

#define	LOGI_ADDR_DISP_REG		(IO2_BASE + 0x00010000)
/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
typedef struct {
	volatile UINT32 dispHDMIPacketHD;	    /* 0x0100 ~ 0x0103 */
	volatile UINT32 dispHDMIPacketBD[7];	    /* 0x0104 ~ 0x0107 */
} HDMI_PKT;

typedef struct dispReg_s {
    volatile UINT32 dispFmt;                /* 0x0000 ~ 0x0003 */
    volatile UINT32 dispCtrl;               /* 0x0004 ~ 0x0007 */
	volatile UINT32 dispBurst;              /* 0x0008 ~ 0x000b */
	volatile UINT32 dispUVAddr;             /* 0x000c ~ 0x000f */
	volatile UINT32 dispPriAddr;            /* 0x0010 ~ 0x0013 */
	volatile UINT32 dispPriPitch;           /* 0x0014 ~ 0x0017 */
	volatile UINT32 dispPriRes;             /* 0x0018 ~ 0x001b */
	volatile UINT32 dispPriSclRes;          /* 0x001c ~ 0x001f */
	volatile UINT32 dispPriSclHParam;       /* 0x0020 ~ 0x0023 */
	volatile UINT32 dispPriSclVParam;       /* 0x0024 ~ 0x0027 */
	volatile UINT32 dispPriSclCtrl;         /* 0x0028 ~ 0x002b */
	volatile UINT32 rsv02c;                 /* 0x002c ~ 0x002f */
	volatile UINT32 dispOsd0Addr;           /* 0x0030 ~ 0x0033 */
	volatile UINT32 dispOsd0Pitch;          /* 0x0034 ~ 0x0037 */
	volatile UINT32 dispOsd0Res;            /* 0x0038 ~ 0x003b */
	volatile UINT32 dispOsd0XY;             /* 0x003c ~ 0x003f */
	volatile UINT32 dispOsd0Fmt;            /* 0x0040 ~ 0x0043 */
	volatile UINT32 dispOsd0Ctrl;           /* 0x0044 ~ 0x0047 */
	volatile UINT32 dispOsd1Fmt;            /* 0x0048 ~ 0x004b */
	volatile UINT32 dispOsd1Ctrl;           /* 0x004c ~ 0x004f */
	volatile UINT32 dispOsd1Addr;           /* 0x0050 ~ 0x0053 */
	volatile UINT32 dispOsd1Pitch;          /* 0x0054 ~ 0x0057 */
	volatile UINT32 dispOsd1Res;            /* 0x0058 ~ 0x005b */
	volatile UINT32 dispOsd1XY;             /* 0x005c ~ 0x005f */
	volatile UINT32 dispOsd0SclHParam;      /* 0x0060 ~ 0x0063 */
	volatile UINT32 dispOsd0SclVParam0;     /* 0x0064 ~ 0x0067 */
	volatile UINT32 dispOsd0SclVParam1;     /* 0x0068 ~ 0x006b */
	volatile UINT32 dispOsd0SclRes;         /* 0x006c ~ 0x006f */
	volatile UINT32 dispOsd1SclHParam;      /* 0x0070 ~ 0x0073 */
	volatile UINT32 dispOsd1SclVParam0;     /* 0x0074 ~ 0x0077 */
	volatile UINT32 dispOsd1SclVParam1;     /* 0x0078 ~ 0x007b */
	volatile UINT32 dispOsd1SclRes;         /* 0x007c ~ 0x007f */
	volatile UINT32 dispRes;                /* 0x0080 ~ 0x0083 */
	volatile UINT32 dispVBlank;             /* 0x0084 ~ 0x0087 */
	volatile UINT32 dispHBlank;             /* 0x0088 ~ 0x008b */
	volatile UINT32 dispBlankData;          /* 0x008c ~ 0x008f */
	volatile UINT32 dispCBarCtrl;           /* 0x0090 ~ 0x0093 */
	volatile UINT32 dispCBar;               /* 0x0094 ~ 0x0097 */
	volatile UINT32 dispOsdColorKey;        /* 0x0098 ~ 0x009b */
	volatile UINT32 dispCtrl2;              /* 0x009c ~ 0x009f */
	volatile UINT32 dispCMatrix0;           /* 0x00a0 ~ 0x00a3 */
	volatile UINT32 dispCMatrix1;           /* 0x00a4 ~ 0x00a7 */
	volatile UINT32 dispCMatrix2;           /* 0x00a8 ~ 0x00ab */
	volatile UINT32 dispCMatrix3;           /* 0x00ac ~ 0x00af */
	volatile UINT32 dispCMatrix4;           /* 0x00b0 ~ 0x00b3 */
	volatile UINT32 dispCMatrix5;           /* 0x00b4 ~ 0x00b7 */
	volatile UINT32 dispDitherMap0;         /* 0x00b8 ~ 0x00bb */
	volatile UINT32 dispDitherMap1;         /* 0x00bc ~ 0x00bf */
	volatile UINT32 rsv0c0[6];              /* 0x00c0 ~ 0x00d7 */
	volatile UINT32 dispTvAmp;              /* 0x00d8 ~ 0x00db */
	volatile UINT32 dispTvPos;              /* 0x00dc ~ 0x00df */
	volatile UINT32 dispLcmAcTiming;        /* 0x00e0 ~ 0x00e3 */
	volatile UINT32 dispLcdVsync;           /* 0x00e4 ~ 0x00e7 */
	volatile UINT32 dispLcdHsync;           /* 0x00e8 ~ 0x00eb */
	volatile UINT32 dispLcdTiming;          /* 0x00ec ~ 0x00ef */
	volatile UINT32 dispIrqEn;              /* 0x00f0 ~ 0x00f3 */
	volatile UINT32 dispIrqSrc;             /* 0x00f4 ~ 0x00f7 */
	volatile UINT32 dispGlobalConfig;       /* 0x00f8 ~ 0x00fb */
	volatile UINT32 rsv0fc;                 /* 0x00fc ~ 0x00ff */
	volatile HDMI_PKT HdmiPkt[4];           /* 0x0100 ~ 0x017f */
	volatile UINT32 dispHDMIAudioN;         /* 0x0180 ~ 0x0183 */
	volatile UINT32 dispHDMIAudioCTS;       /* 0x0184 ~ 0x0187 */
	volatile UINT32 dispHDMIAudioSample;    /* 0x0188 ~ 0x018b */
	volatile UINT32 dispHDMIGCtrlPacket;    /* 0x018c ~ 0x018f */
	volatile UINT32 dispHDMITimeCycle;      /* 0x0190 ~ 0x0193 */
	volatile UINT32 dispHDMIConfig;         /* 0x0194 ~ 0x0197 */
	volatile UINT32 rsv198;                 /* 0x0198 ~ 0x019b */
	volatile UINT32 dispLVDSCtrl;           /* 0x019c ~ 0x019e */
	volatile UINT32 dispCMatxThre[4];       /* 0x01a0 ~ 0x01af */
	volatile UINT32 dispCMatxLimit;         /* 0x01b0 ~ 0x01b3 */
	volatile UINT32 rsv1b4[11];             /* 0x01b4 ~ 0x01df */
	volatile UINT32 dispCMatxPara[12][6];   /* 0x01e0 ~ 0x02ff */
	volatile UINT32 dispTCON[7];		    /* 0x0300 ~ 0x031b */
	volatile UINT32 hdmiTXPHYConfig1;       /* 0x031c ~ 0x031f */
	volatile UINT32 hdmiTXPHYConfig2;       /* 0x0320 ~ 0x0323 */
	volatile UINT32 dispVDACConfig;         /* 0x0324 ~ 0x0327 */
	volatile UINT32 rsv328[54];             /* 0x0328 ~ 0x03ff */
	volatile UINT32 dispGammaPtr[256];      /* 0x0400 ~ 0x07ff */
	volatile UINT32 dispPalettePtr[256];    /* 0x0800 ~ 0x0bff */
	volatile UINT32 rsv804[192];            /* 0x0c00 ~ 0x0eff */
	volatile UINT32 dispLcmPgmIo00;         /* 0x0f00 ~ 0x0f03 */
	volatile UINT32 rsvf04[3];              /* 0x0f04 ~ 0x0f0f */
	volatile UINT32 dispLcmPgmIo10;         /* 0x0f10 ~ 0x0f13 */
	volatile UINT32 rsvf14[7];              /* 0x0f14 ~ 0x0f2f */
	volatile UINT32 dispLcmPgmIo30;         /* 0x0f30 ~ 0x0f33 */
} dispReg_t;

typedef struct dispOsdRegArray_s
{
	volatile UINT32 *DISP_OSD_ADDR[2];
	volatile UINT32 *DISP_OSD_PITCH[2];
	volatile UINT32 *DISP_OSD_RES[2];
	volatile UINT32 *DISP_OSD_XY[2];
	volatile UINT32 *DISP_OSD_FMT[2];
	volatile UINT32 *DISP_OSD_CTRL[2];
	volatile UINT32 *DISP_OSD_HPARAM[2];
	volatile UINT32 *DISP_OSD_VPARAM0[2];
	volatile UINT32 *DISP_OSD_VPARAM1[2];
	volatile UINT32 *DISP_OSD_SCLRES[2];
} dispOsdRegArray_t;

#if 0 /* Check scu for display */
typedef struct scu_a_Reg_s {
	volatile UINT32 rsv0[7];        /* 0x0000 ~ 0x001b */
	volatile UINT32 spll2Ctrl;      /* 0x001c ~ 0x001f */
	volatile UINT32 clkSrcMux;      /* 0x0020 ~ 0x0023 */
	volatile UINT32 rsv24[200];     /* 0x0024 ~ 0x0343 */
	volatile UINT32 dispRstCtrl;    /* 0x0344 ~ 0x0347 */
	volatile UINT32 rsv348[78];     /* 0x0348 ~ 0x0347 */
	volatile UINT32 clko8Ctrl0;     /* 0x0480 ~ 0x0483 */
	volatile UINT32 clko8Ctrl1;     /* 0x0484 ~ 0x0487 */
	volatile UINT32 clko8Ctrl2;     /* 0x0488 ~ 0x048b */
	volatile UINT32 clko8Ctrl3;     /* 0x048c ~ 0x048f */
	volatile UINT32 rsv490[24];     /* 0x0490 ~ 0x048f */
	volatile UINT32 disp2clkCtrl0;  /* 0x04f0 ~ 0x04f3 */
	volatile UINT32 disp2clkCtrl1;  /* 0x04f4 ~ 0x04f7 */
	volatile UINT32 disp2clkCtrl2;  /* 0x04f8 ~ 0x04fb */
	volatile UINT32 disp2clkCtrl3;  /* 0x04fc ~ 0x04ff */
} scu_a_Reg_t;

typedef struct scu_e_Reg_s {
	volatile UINT32 dispMisc;   /* 0x0000 ~ 0x0003 */
	volatile UINT32 dummy[3];   /* 0x0004 ~ 0x000f */
	volatile UINT32 dispCtrl;   /* 0x0010 ~ 0x0013 */
} scu_e_Reg_t;
#endif

#endif /* _REG_DISP_H_ */

