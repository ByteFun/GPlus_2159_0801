/**************************************************************************
 *                                                                        *
 *         Copyright (c) 2010 by Generalplus Inc.                         *
 *                                                                        *
 *  This software is copyrighted by and is the property of Generalplus    *
 *  Inc. All rights are reserved by Generalplus Inc.                      *
 *  This software may only be used in accordance with the                 *
 *  corresponding license agreement. Any unauthorized use, duplication,   *
 *  distribution, or disclosure of this software is expressly forbidden.  *
 *                                                                        *
 *  This Copyright notice MUST not be removed or modified without prior   *
 *  written consent of Generalplus Technology Co., Ltd.                   *
 *                                                                        *
 *  Generalplus Inc. reserves the right to modify this software           *
 *  without notice.                                                       *
 *                                                                        *
 *  Generalplus Inc.                                                      *
 *  3F, No.8, Dusing Rd., Hsinchu Science Park,                           *
 *  Hsinchu City 30078, Taiwan, R.O.C.                                    *
 *                                                                        *
 **************************************************************************/

/*!
 * @file lcd_hannstar_HSD070IDW1.h
 * @brief The lcd driver of Hannstar HSD070IDW1
 */
#include <linux/init.h>
#include <linux/configfs.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */

#include <mach/panel_cfg.h>
#include <mach/hardware.h>
#include <mach/hal/hal_clock.h>
#include <mach/hal/hal_disp1.h>
#include <mach/hal/hal_ppu.h>
#include <mach/hal/regmap/reg_scu.h>
#include <mach/gp_display1.h>
#include <mach/clock_mgr/gp_clock.h>
#include <mach/clk/gp_clk_core.h>
#include <mach/gp_board.h>
#include <mach/module.h>
MODULE_LICENSE_GP;

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/
static int32_t lcd_init(void);
static int32_t lcd_suspend(void);
static int32_t lcd_resume(void);
static int32_t lcd_get_size(gp_size_t *size);

/**************************************************************************
 *                           C O N S T A N T S                            *
 **************************************************************************/

/**************************************************************************
 *                              M A C R O S                               *
 **************************************************************************/

/******************************`********************************************
 *                          D A T A    T Y P E S                          *
 **************************************************************************/
typedef struct
{
	char *name;
	uint32_t resolution;
	uint32_t dataMode;
	uint32_t mode;
	uint32_t dclk;
	gpHalDisp1LcdTiming_t vsync;
	gpHalDisp1LcdTiming_t hsync;
	gp_size_t panelSize;	/* Panel physical size in mm */
	uint32_t ditherType;
	uint8_t *pGammaTable[3];
} disp1_panelInfo;

/**************************************************************************
 *                         G L O B A L    D A T A                         *
 **************************************************************************/
static const char gPanelName[] = "LCD_Hannstar_HSD070IDW1_tft1";
static int32_t g_pin_func_handle;

static const uint8_t gammaR[256] = {
0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x04, 0x07, 0x0a, 
0x0d, 0x10, 0x11, 0x12, 0x13, 0x14, 0x16, 0x17, 0x18, 0x19, 0x1b, 0x1c, 0x1d, 0x1f, 0x20, 0x21, 
0x22, 0x22, 0x23, 0x24, 0x24, 0x25, 0x26, 0x27, 0x28, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 
0x2f, 0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x34, 0x34, 0x35, 0x36, 0x36, 0x37, 0x38, 0x38, 0x39, 
0x3a, 0x3b, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x3f, 0x40, 0x41, 0x41, 0x42, 0x43, 0x43, 0x44, 0x44, 
0x45, 0x46, 0x46, 0x47, 0x48, 0x48, 0x49, 0x4a, 0x4a, 0x4b, 0x4c, 0x4d, 0x4d, 0x4e, 0x4f, 0x50, 
0x50, 0x51, 0x52, 0x53, 0x53, 0x54, 0x55, 0x56, 0x57, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5c, 
0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 
0x6d, 0x6e, 0x70, 0x71, 0x72, 0x73, 0x75, 0x76, 0x78, 0x79, 0x7a, 0x7c, 0x7d, 0x7e, 0x80, 0x81, 
0x82, 0x84, 0x85, 0x86, 0x87, 0x89, 0x8a, 0x8b, 0x8c, 0x8e, 0x8f, 0x90, 0x91, 0x93, 0x94, 0x95, 
0x96, 0x97, 0x99, 0x9a, 0x9b, 0x9c, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa4, 0xa5, 0xa6, 0xa7, 0xa9, 
0xaa, 0xab, 0xac, 0xae, 0xaf, 0xb0, 0xb1, 0xb3, 0xb4, 0xb5, 0xb6, 0xb8, 0xb9, 0xba, 0xbc, 0xbd, 
0xbe, 0xc0, 0xc1, 0xc2, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 
0xce, 0xcf, 0xd0, 0xd1, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 
0xdd, 0xde, 0xdf, 0xe1, 0xe2, 0xe4, 0xe5, 0xe7, 0xe8, 0xea, 0xeb, 0xed, 0xef, 0xf1, 0xf8, 0xff, 
};

static const uint8_t gammaG[256] = {
0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x05, 0x07, 0x09, 0x0b, 0x0d, 0x10, 0x11, 0x11, 0x12, 
0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 
0x22, 0x23, 0x23, 0x24, 0x25, 0x25, 0x26, 0x27, 0x28, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2c, 0x2d, 
0x2e, 0x2f, 0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33, 0x34, 0x35, 0x35, 0x36, 0x36, 0x37, 0x38, 
0x38, 0x39, 0x3a, 0x3a, 0x3b, 0x3c, 0x3d, 0x3d, 0x3e, 0x3f, 0x40, 0x40, 0x41, 0x41, 0x42, 0x42, 
0x43, 0x44, 0x44, 0x45, 0x45, 0x46, 0x47, 0x47, 0x48, 0x49, 0x49, 0x4a, 0x4a, 0x4b, 0x4c, 0x4c, 
0x4d, 0x4e, 0x4f, 0x4f, 0x50, 0x51, 0x51, 0x52, 0x53, 0x54, 0x54, 0x55, 0x56, 0x57, 0x57, 0x58, 
0x59, 0x5a, 0x5b, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 
0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x74, 0x75, 0x76, 0x78, 0x79, 
0x7a, 0x7c, 0x7d, 0x7e, 0x80, 0x81, 0x82, 0x83, 0x85, 0x86, 0x87, 0x88, 0x8a, 0x8b, 0x8c, 0x8d, 
0x8f, 0x90, 0x91, 0x92, 0x93, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9b, 0x9c, 0x9d, 0x9e, 0xa0, 0xa1, 
0xa2, 0xa3, 0xa5, 0xa6, 0xa7, 0xa8, 0xaa, 0xab, 0xac, 0xad, 0xaf, 0xb0, 0xb1, 0xb3, 0xb4, 0xb5, 
0xb6, 0xb8, 0xb9, 0xba, 0xbc, 0xbd, 0xbe, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 
0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd8, 0xd9, 
0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xe0, 0xe1, 0xe3, 0xe5, 0xe7, 0xe9, 0xeb, 0xed, 0xef, 0xf6, 0xff,
};

static const uint8_t gammaB[256] = {
0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x04, 0x05, 0x07, 0x08, 0x0a, 0x0c, 0x0e, 0x0f, 
0x10, 0x11, 0x12, 0x12, 0x13, 0x14, 0x14, 0x15, 0x16, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1b, 
0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23, 0x24, 0x24, 0x25, 0x25, 0x26, 
0x26, 0x27, 0x28, 0x28, 0x29, 0x2a, 0x2a, 0x2b, 0x2c, 0x2c, 0x2d, 0x2e, 0x2e, 0x2f, 0x30, 0x30, 
0x31, 0x31, 0x32, 0x32, 0x33, 0x33, 0x34, 0x34, 0x35, 0x35, 0x36, 0x37, 0x37, 0x38, 0x38, 0x39, 
0x39, 0x3a, 0x3b, 0x3b, 0x3c, 0x3c, 0x3d, 0x3e, 0x3e, 0x3f, 0x40, 0x40, 0x41, 0x41, 0x42, 0x42, 
0x43, 0x43, 0x44, 0x44, 0x45, 0x46, 0x46, 0x47, 0x47, 0x48, 0x48, 0x49, 0x4a, 0x4a, 0x4b, 0x4b, 
0x4c, 0x4d, 0x4d, 0x4e, 0x4e, 0x4f, 0x50, 0x50, 0x51, 0x52, 0x53, 0x53, 0x54, 0x55, 0x55, 0x56, 
0x57, 0x58, 0x58, 0x59, 0x5a, 0x5b, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x5f, 0x60, 0x61, 0x62, 0x63, 
0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x72, 0x73, 0x74, 
0x76, 0x77, 0x79, 0x7a, 0x7b, 0x7d, 0x7e, 0x80, 0x81, 0x82, 0x83, 0x85, 0x86, 0x87, 0x88, 0x8a, 
0x8b, 0x8c, 0x8d, 0x8f, 0x90, 0x91, 0x93, 0x94, 0x95, 0x96, 0x98, 0x99, 0x9a, 0x9c, 0x9d, 0x9e, 
0xa0, 0xa1, 0xa2, 0xa4, 0xa5, 0xa6, 0xa8, 0xa9, 0xaa, 0xac, 0xad, 0xaf, 0xb0, 0xb2, 0xb3, 0xb5, 
0xb6, 0xb8, 0xb9, 0xbb, 0xbc, 0xbe, 0xbf, 0xc1, 0xc2, 0xc3, 0xc5, 0xc6, 0xc7, 0xc9, 0xca, 0xcb, 
0xcd, 0xce, 0xcf, 0xd1, 0xd3, 0xd5, 0xd6, 0xd8, 0xda, 0xdc, 0xde, 0xe0, 0xe4, 0xe8, 0xed, 0xff, 
};

static disp1_panelInfo gPanelInfo = {
	.name = (char*) &gPanelName,
	.resolution = HAL_DISP1_RES_800_480,
	.dataMode = HAL_DISP1_DATA_MODE_888,
	.mode = HAL_DISP1_MODE_PARALLEL,
	.dclk = HAL_DISP1_DCLK_SEL_90,
	.vsync = {
		.polarity = 0,
		.period = 525,
		.start = 8,
		.end = 488,
	},
	.hsync = {
		.polarity = 0,
		.period = 928,
		.start = 100,
		.end = 899,
	},
	.panelSize = {
		.width = 1920,
		.height = 1083,
	},
	.ditherType = HAL_DISP1_DITHER_8TO6,
	.pGammaTable = {
		(uint8_t*) gammaR,
		(uint8_t*) gammaG,
		(uint8_t*) gammaB,
	},
};

/* access functions */
static gp_disp_panel_ops_t lcd_fops = {
	.init = lcd_init,
	.suspend = lcd_suspend,
	.resume = lcd_resume,
	.get_size = lcd_get_size,
};

/**************************************************************************
 *                 E X T E R N A L    R E F E R E N C E S                 *
 **************************************************************************/

/**************************************************************************
 *               F U N C T I O N    D E C L A R A T I O N S               *
 **************************************************************************/
static int32_t
Hannstar_HSD070IDW1_tft1(
	void
)
{
	/* Enable clock */
#ifdef CONFIG_PM
	gp_enable_clock((int*)"PPU_TFT", 1);	
	gpHalScuClkEnable(SCU_A_PERI_PPU_TFT, SCU_A2, 1);
#else
	scuaReg_t *pscuaReg = (scuaReg_t *)LOGI_ADDR_SCU_A_REG;
	pscuaReg->scuaPeriClkEn2 |= 0x26000000;
#endif

	g_pin_func_handle = gp_board_pin_func_request(GP_PIN_DISP0_LCD, GP_BOARD_WAIT_FOREVER);
	if (g_pin_func_handle < 0) {
		printk("[%s:%d] Error!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	/* initial panel parameter */
	gpHalDisp1SetDevType(HAL_DISP1_DEV_LCD);

	/* Panel Init */
	gpHalDisp1SetRes(gPanelInfo.resolution);
	gpHalPPUSetTftBurst(1);
	gpHalPPUSetFbEnable(1);
	gpHalPPUSetVgaEnable(0);

	/* Vertical settings */
	gpHalDisp1SetVerticalPeriod(gPanelInfo.vsync.period);
	gpHalDisp1SetVerticalStart(gPanelInfo.vsync.start);
	gpHalDisp1SetVerticalEnd(gPanelInfo.vsync.end);	

	/* Horizontal settings */
	gpHalDisp1SetHorizontalPeriod(gPanelInfo.hsync.period);
	gpHalDisp1SetHorizontalStart(gPanelInfo.hsync.start);
	gpHalDisp1SetHorizontalEnd(gPanelInfo.hsync.end);


	gpHalDisp1SetVSyncEnd(0);

	gpHalDisp1SetDataMode(gPanelInfo.dataMode);
	gpHalDisp1SetVSyncUnit(STATE_TRUE);
	gpHalDisp1SetDClkSel(gPanelInfo.dclk);
	gpHalDisp1SetSignalInv( HAL_DISP1_VSYNC_INV|HAL_DISP1_HSYNC_INV,(HAL_DISP1_ENABLE & HAL_DISP1_VSYNC_INV)|(HAL_DISP1_ENABLE & HAL_DISP1_HSYNC_INV) );
	gpHalDisp1SetMode(gPanelInfo.mode);
	gpHalDisp1SetClk(HAL_DISP1_CLK_DIVIDE_8);

	return 0;
}

static int32_t
lcd_init(
	void
)
{
	printk("[%s:%d]\n", __FUNCTION__, __LINE__);	

	Hannstar_HSD070IDW1_tft1();

#if 0	
	/* Set dither */
	disp1_set_dither_type(gPanelInfo.ditherType);
	disp1_set_dither_enable(1);

	/* Set gamma table */
	disp1_set_gamma_enable(0);
	disp1_set_gamma_table(SP_DISP1_GAMMA_R, (uint8_t*) gPanelInfo.pGammaTable[SP_DISP1_GAMMA_R]);
	disp1_set_gamma_table(SP_DISP1_GAMMA_G, (uint8_t*) gPanelInfo.pGammaTable[SP_DISP1_GAMMA_G]);
	disp1_set_gamma_table(SP_DISP1_GAMMA_B, (uint8_t*) gPanelInfo.pGammaTable[SP_DISP1_GAMMA_B]);
	disp1_set_gamma_enable(1);
#endif
	return 0;
}

static int32_t
lcd_suspend(
	void
)
{
	printk("[%s:%d]\n", __FUNCTION__, __LINE__);

	/* board config : release pin function */
	gp_board_pin_func_release(g_pin_func_handle);

	return 0;
}

static int32_t
lcd_resume(
	void
)
{
	printk("[%s:%d]\n", __FUNCTION__, __LINE__);

	Hannstar_HSD070IDW1_tft1();
        
#if 0
	/* Set dither */
	disp1_set_dither_type(gPanelInfo.ditherType);
	disp1_set_dither_enable(1);

	/* Set gamma table */
	disp1_set_gamma_enable(0);
	disp1_set_gamma_table(SP_DISP1_GAMMA_R, (uint8_t*) gPanelInfo.pGammaTable[SP_DISP1_GAMMA_R]);
	disp1_set_gamma_table(SP_DISP1_GAMMA_G, (uint8_t*) gPanelInfo.pGammaTable[SP_DISP1_GAMMA_G]);
	disp1_set_gamma_table(SP_DISP1_GAMMA_B, (uint8_t*) gPanelInfo.pGammaTable[SP_DISP1_GAMMA_B]);
	disp1_set_gamma_enable(1);
#endif
	return 0;
}

static int32_t
lcd_get_size(
	gp_size_t *size
)
{
	if (gPanelInfo.panelSize.width && gPanelInfo.panelSize.height) {
		size->width = gPanelInfo.panelSize.width;
		size->height = gPanelInfo.panelSize.height;
		return 0;
	}
	else {
		return -1;
	}
}

static int32_t
panel_init(
	void
)
{
  printk("[%s:%d]\n", __FUNCTION__, __LINE__);
  register_paneldev1(SP_DISP_OUTPUT_LCD, gPanelInfo.name, &lcd_fops);
  return 0;
}

static void
panel_exit(
	void
)
{
  printk("[%s:%d]\n", __FUNCTION__, __LINE__);
  unregister_paneldev1(SP_DISP_OUTPUT_LCD, gPanelInfo.name);
}

module_init(panel_init);
module_exit(panel_exit);
