#
# Makefile for the NAND driver
#

obj-m += nand_hal.o	
	nand_hal-objs := \
	hal_nand_dev.lib\
	hal_nand.lib\
	hal_nand_bch.lib\
	hal_nand_smart_id.lib\
	hal_nand_bthdr.lib\
    hal_nand_isp.lib\
    nand_platform.lib

obj-m += gp_blk_app.o
	gp_blk_app-objs := \
	app_dev.lib \
	drv_l2_nand_app.lib \
	drv_l2_nand_config.lib \
    drv_l2_nand_app_isp.lib
	
obj-m += gp_nand_module.o
	gp_nand_module-objs := \
	nand_data_dev.lib \
	drv_l2_nand_global.lib \
	drv_l2_nand_cache.lib \
	drv_l2_nand_manage.lib \
	drv_l2_nand_test.lib \
	drv_l2_nand_config.lib \
    drv_l2_nand_data_isp.lib

obj-m += gp_nand_module2.o
	gp_nand_module2-objs := \
	nand_data_dev2.lib \
	drv_l2_nand_global.lib \
	drv_l2_nand_cache.lib \
	drv_l2_nand_manage.lib \
	drv_l2_nand_test.lib \
	drv_l2_nand_config.lib \
    drv_l2_nand_data_isp.lib
