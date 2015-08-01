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
#ifndef _REG_I2S_H_
#define _REG_I2S_H_

#include <mach/hardware.h>                          
#include <mach/typedef.h>

#define	LOGI_ADDR_I2STX_REG		IO3_ADDRESS(0x12000)
#define	LOGI_ADDR_I2SRX_REG		IO3_ADDRESS(0x1D000)

// TX ISCR Register 
#define IISTX_R_MONO	0x400000
#define IISTX_R_LSB		0x200000 // 0 = [right,left] 1 = [left,right]
#define IISTX_MERGE		0x100000 // merge or not
#define IISTX_UNDFLOW	0x080000 
#define IISTX_CLRFIFO	0x040000
#define IISTX_IRT_FLAG	0x020000 // interrupt enable flag bit

#define IISTX_DIS_IRT	0x000000 // interrupt enable bit
#define IISTX_EN_IRT	0x010000 // interrupt enable bit

#define IISTX_DIS_OVWR	0x0000 // overwrite enable bit
#define IISTX_EN_OVWR	0x8000 // overwrite enable bit

#define IISTX_IRT_POLARITY_LOW		0x0000 // 0 : interrupt is low active 1 : High
#define IISTX_IRT_POLARITY_HIGH		0x4000 // 0 : interrupt is low active 1 : High

#define IISTX_MASTER_MODE	0x0000 
#define IISTX_SLAVE_MODE	0x2000 
   // I2S Mode
#define IISTX_MODE_I2S		0x0000
#define IISTX_MODE_NORMAL	0x0800
#define IISTX_MODE_DSP		0x1000
#define IISTX_MODE_DSP_2	0x1800
#define IISTX_MODE_MASK		0x1800
   // Frame Size Mode
#define IISTX_FSMODE_16		0x0000
#define IISTX_FSMODE_24		0x0200
#define IISTX_FSMODE_32		0x0400
#define IISTX_FSMODE_NONPRED	0x0600
#define IISTX_FSMODE_MASK	0x0600
   // Valid Data Mode
#define IISTX_VDMODE_16		0x0000
#define IISTX_VDMODE_18		0x0040
#define IISTX_VDMODE_20		0x0080
#define IISTX_VDMODE_22		0x00C0
#define IISTX_VDMODE_24		0x0100
#define IISTX_VDMODE_32		0x01C0
#define IISTX_VDMODE_8		0x0180
#define IISTX_VDMODE_MASK	0x01C0

#define IISTX_MODE_ALIGN_RIGHT	0x0000
#define IISTX_MODE_ALIGN_LEFT	0x0020
#define IISTX_SENDMODE_MSB		0x0000
#define IISTX_SENDMODE_LSB		0x0010

#define IISTX_EDGEMODE_FALLING	0x0000
#define IISTX_EDGEMODE_RISING	0x0008

#define IISTX_FRAME_POLARITY_R	0x0000
#define IISTX_FRAME_POLARITY_L	0x0004

#define IISTX_FIRSTFRAME_L		0x0000
#define IISTX_FIRSTFRAME_R		0x0002

#define IISTX_ENABLE			0x0001

// TX ISSR Register 
#define IISTX_CLRFIFO_STATUS	0x200
#define IISTX_WORD_NO_MASK		0x3F
#define IISTX_WORD_NO_OFST		3
#define IISTX_OVERWRITE			0x004
#define IISTX_FULL				0x002
#define IISTX_EMPTY				0x001


// TX ISCR Register 
#define IISRX_R_LSB			0x100000 // 0 = [right,left] 1 = [left,right]
#define IISRX_MERGE			0x080000 // merge or not
#define IISRX_CLRFIFO		0x040000 
#define IISRX_IRT_PEND		0x020000 // interrupt status 
#define IISRX_EN_IRT		0x010000 // RX Interrupt enable bit 
#define IISRX_OVF			0x008000 // over flow flag
#define IISRX_IRT_POLARITY	0x004000 // 0 : interrupt is low active 1 : High


#define IISRX_DIS_IRT	0x000000 // interrupt Rx enable bit
#define IISRX_EN_IRT	0x010000 // interrupt Rx enable bit

#define IISRX_IRT_POLARITY_LOW		0x0000 // 0 : interrupt is low active 1 : High
#define IISRX_IRT_POLARITY_HIGH		0x4000 // 0 : interrupt is low active 1 : High

#define IISRX_MASTER_MODE	0x2000 
#define IISRX_SLAVE_MODE	0x0000 

   // I2S Mode
#define IISRX_MODE_I2S		0x0000
#define IISRX_MODE_NORMAL	0x0800
#define IISTX_MODE_DSP		0x1000
#define IISRX_MODE_DSP_2	0x1800
#define IISRX_MODE_MASK		0x1800

   // Frame Size Mode
#define IISRX_FSMODE_16			0x0000
#define IISRX_FSMODE_24			0x0200
#define IISRX_FSMODE_32			0x0400
#define IISRX_FSMODE_NONPRED	0x0600
#define IISRX_FSMODE_MASK		0x0600

   // Valid Data Mode
#define IISRX_VDMODE_16		0x0000
#define IISRX_VDMODE_18		0x0040
#define IISRX_VDMODE_20		0x0080
#define IISRX_VDMODE_22		0x00C0
#define IISRX_VDMODE_24		0x0100
#define IISRX_VDMODE_32		0x0140
#define IISRX_VDMODE_MASK	0x01C0

#define IISRX_MODE_ALIGN_RIGHT	0x0
#define IISRX_MODE_ALIGN_LEFT	0x20
#define IISRX_SENDMODE_MSB		0x0
#define IISRX_SENDMODE_LSB		0x10

#define IISRX_EDGEMODE_FALLING	0x0
#define IISRX_EDGEMODE_RISING	0x08

#define IISRX_FRAME_POLARITY_R	0x0
#define IISRX_FRAME_POLARITY_L	0x04

#define IISRX_FIRSTFRAME_L	0x0
#define IISRX_FIRSTFRAME_R	0x02

#define IISRX_DISABLE		0x0
#define IISRX_ENABLE		0x01

// TX ISSR Register 
#define IISRX_WORD_NO_MASK	0x3F
#define IISRX_WORD_NO_OFST	2
#define IISRX_FULL			0x002
#define IISRX_EMPTY			0x001

#define IISTX_MONO    0x400000
#define IISRX_MONO    0x200000

#define IISTX	0
#define	IISRX	1

/**************************************************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/

typedef struct i2stxReg_s {
	volatile UINT32 ctl;
	volatile UINT32 data;
	volatile UINT32 status;
} i2stxReg_t;

typedef struct i2srxReg_s {
	volatile UINT32 ctl;
	volatile UINT32 data;
	volatile UINT32 status;
} i2srxReg_t;

#endif