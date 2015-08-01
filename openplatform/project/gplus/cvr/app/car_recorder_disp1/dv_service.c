#include "ap_state_config.h"
#include "font.h"
#include "disp.h"
#include "cvr_pipe.h"

extern iconManager_t Icon_Manager[];
extern menuManager_t DV_MENU[];
extern menuManager_t DC_MENU[];
extern menuManager_t PLAY_MENU[];
extern menuManager_t DV_SETTING[];
extern UINT8 setup_date_time[];
extern char dt_str[];
extern HANDLE hDisp;
extern UINT8 Playmode_flag;
extern struct tm DV_start_time;
extern struct timeval cap_start;
extern UINT8 power_off;
UINT8 foreground_draw_flag=0;
UINT8 cur_draw_flag=0;
extern SYSTEM_USER_OPTION Global_User_Optins;
static UINT32 DV_BPS[5] = {1408,820,820,410,410};
static char str_resolution[][10] = {"1080FHD","720P60FPS","720P30FPS","WVGA","VGA"};
static char date_str[][15] = {"2050 / 02 / 27", "02 / 27 / 2048", "27 / 02 / 2048"};
static char date1_str[][10] = {"YY/MM/DD","MM/DD/YY ","DD/MM/YY"};
static char time_str[] = "00 : 00 : 00";
static char time_str1[] = "00:00:00";
static char page[] = "0/0";
static char zoom[] = "x0.0";
UINT8 flip_flag = 0;
UINT8 flip_flag1 = 0;
extern mqd_t menumodeMsgQ;
UINT32 time_num= 0;
dv_set_t dv_set;
extern struct tm DV_start_time;
extern char file_path[256];
extern UINT8 HDMIxNum;
extern UINT16 HDMI_X_offset;
extern UINT16	 HDMI_Y_offset;
void DV_UI_foreground(void);
void UI_Menu_draw(UINT8 mode);
void DV_UI_Setting(void);
extern void Play_Key_sound(UINT8 sound_type);
extern void DV_pipe_init(void);
extern void DV_power_off();

LDW_DISPLINE_t ldw_displine;
extern void LDW_GetLine(LDW_DISPLINE_t *para);
int gb_disp_LDWlineDisp;
/*==========================================================
dv background UI draw function
===========================================================*/
void DV_UI_foreground(void)
{
	STRING_INFO sub_str;
	STRING_ASCII_INFO ascii_str;
	t_STRING_TABLE_STRUCT str_res;
	gp_size_t resolution;
	UINT16 *fb;
	UINT16* addr1;
	time_t t;
	struct tm tm;	
	UINT32 free_size;
	UINT8 time_flag;
	UINT8 temp;
	iconManager_t ldw_rect;
	
	if(dv_set.change_disp != 0)
		return;
	dispGetResolution(hDisp, &resolution);	
	addr1 = fb = (UINT16*)dispGetFramebuffer(hDisp);
	if((cur_draw_flag != DV_FOREGROUND_MODE)||(dv_set.sd_check!=1)||(dv_set.sdc_full !=0)||(dv_set.no_power_flag !=0)||(dv_set.sdc_error== 1))
	{
	 	//printf("============clean screen============[%d]\n",foreground_draw_flag);	
		//printf("resolution.width=%d,resolution.height=%d\n",resolution.width,resolution.height);	
		for (int y = 0; y <resolution.height; y++) {		
			for (int x = 0; x < resolution.width; x++) {			
				*addr1++ = TRANSPARENT_COLOR;		
			}	
		}
		foreground_draw_flag++;
	} 
	if(dv_set.ui_show == 0)
	{
		dispFlip(hDisp);
		cur_draw_flag = DV_EXIT_MODE;
		foreground_draw_flag=0;		
		return;
	}
	if(dv_set.no_power_flag == 2)	
	{
		gp_Icon_draw(fb, (resolution.width-Icon_Manager[ICON_NOFILE].icon_width*HDMIxNum)/2,(resolution.height-Icon_Manager[ICON_NOFILE].icon_height*HDMIxNum)/2,resolution.width, resolution.height,ICON_NOFILE,Icon_Manager[ICON_NOFILE].tran_color);
		sub_str.font_type = 0;	
		sub_str.font_color = FONT_COLOR;
		sub_str.buff_w = resolution.width;
		sub_str.buff_h = resolution.height;	
		sub_str.language = setting_config_get(SET_LANGUAGE);
		sub_str.str_idx = STR_NO_POWER;		
		ap_state_resource_string_resolution_get(&sub_str,&str_res);
		sub_str.pos_x = (resolution.width -str_res.string_width) /2;
		sub_str.pos_y = (resolution.height -str_res.string_height) /2;
		ap_state_resource_string_draw((INT16U *)fb, &sub_str);		
		dispFlip(hDisp);
		return;		
	}
	else if(dv_set.sd_check == 2)
	{	
		gp_Icon_draw(fb, (resolution.width-Icon_Manager[ICON_NOFILE].icon_width*HDMIxNum)/2,(resolution.height-Icon_Manager[ICON_NOFILE].icon_height*HDMIxNum)/2,resolution.width, resolution.height,ICON_NOFILE,Icon_Manager[ICON_NOFILE].tran_color);
		sub_str.font_type = 0;	
		sub_str.font_color = FONT_COLOR;
		sub_str.buff_w = resolution.width;
		sub_str.buff_h = resolution.height;	
		sub_str.language = setting_config_get(SET_LANGUAGE);
		sub_str.str_idx = STR_INSERT_SDC;		
		ap_state_resource_string_resolution_get(&sub_str,&str_res);
		sub_str.pos_x = (resolution.width -str_res.string_width) /2;
		sub_str.pos_y = (resolution.height -str_res.string_height) /2;
		ap_state_resource_string_draw((INT16U *)fb, &sub_str);		
		dispFlip(hDisp);
		return;		
	}
	else if(dv_set.sdc_full == 2)
	{	
		gp_Icon_draw(fb, (resolution.width-Icon_Manager[ICON_NOFILE].icon_width*HDMIxNum)/2,(resolution.height-Icon_Manager[ICON_NOFILE].icon_height*HDMIxNum)/2,resolution.width, resolution.height,ICON_NOFILE,Icon_Manager[ICON_NOFILE].tran_color);
		sub_str.font_type = 0;	
		sub_str.font_color = FONT_COLOR;
		sub_str.buff_w = resolution.width;
		sub_str.buff_h = resolution.height;	
		sub_str.language = setting_config_get(SET_LANGUAGE);
		sub_str.str_idx = STR_SD_FULL;		
		ap_state_resource_string_resolution_get(&sub_str,&str_res);
		sub_str.pos_x = (resolution.width -str_res.string_width) /2;
		sub_str.pos_y = (resolution.height -str_res.string_height) /2;
		ap_state_resource_string_draw((INT16U *)fb, &sub_str);		
		dispFlip(hDisp);
		return;		
	}	
	else if(dv_set.sdc_error== 1)
	{	
		gp_Icon_draw(fb, (resolution.width-Icon_Manager[ICON_NOFILE].icon_width*HDMIxNum)/2,(resolution.height-Icon_Manager[ICON_NOFILE].icon_height*HDMIxNum)/2,resolution.width, resolution.height,ICON_NOFILE,Icon_Manager[ICON_NOFILE].tran_color);
		sub_str.font_type = 0;	
		sub_str.font_color = FONT_COLOR;
		sub_str.buff_w = resolution.width;
		sub_str.buff_h = resolution.height;	
		sub_str.language = setting_config_get(SET_LANGUAGE);
		sub_str.str_idx = STR_SDC_ERROR;		
		ap_state_resource_string_resolution_get(&sub_str,&str_res);
		sub_str.pos_x = (resolution.width -str_res.string_width) /2;
		sub_str.pos_y = (resolution.height -str_res.string_height) /2;
		ap_state_resource_string_draw((INT16U *)fb, &sub_str);		
		dispFlip(hDisp);
		return;		
	}	
	//printf("flip_flag = %d\n",flip_flag);
	ascii_str.font_color = FONT_COLOR;
	ascii_str.font_type = 0;
	ascii_str.buff_w = resolution.width;
	ascii_str.buff_h = resolution.height;
	if(foreground_draw_flag!=0)
	{
		//resolution string
		ascii_str.str_ptr = str_resolution[DV_config_get(DV_RESOLUTION)];
		ap_state_resource_string_ascii_resolution_get(ascii_str.str_ptr,&str_res);
		if(dv_set.display_mode == SP_DISP_OUTPUT_HDMI)
		{
			ascii_str.pos_x = resolution.width - ICON_LEFT_X_POS -str_res.string_width-Y_STEP ;
			ascii_str.pos_y = ICON_LEFT_Y_POS+Y_STEP+Y_STEP;			
		}
		else
		{
			ascii_str.pos_x = resolution.width - ICON_LEFT_X_POS -str_res.string_width ;
			ascii_str.pos_y = ICON_LEFT_Y_POS+Y_STEP;			
		}
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);	
		
		//play mode icon
		gp_Icon_draw(fb, Icon_Manager[ICON_VIDEO].x_pos*HDMIxNum,Icon_Manager[ICON_VIDEO].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_VIDEO,Icon_Manager[ICON_VIDEO].tran_color);
		
		//loop recording icon
		if(DV_config_get(DV_LOOPRECORDING)>0)
		{
			gp_Icon_draw(fb, Icon_Manager[ICON_LOOPRECORDING2+DV_config_get(DV_LOOPRECORDING)-1].x_pos*HDMIxNum,Icon_Manager[ICON_LOOPRECORDING2+DV_config_get(DV_LOOPRECORDING)-1].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_LOOPRECORDING2+DV_config_get(DV_LOOPRECORDING)-1,Icon_Manager[ICON_LOOPRECORDING2+DV_config_get(DV_LOOPRECORDING)-1].tran_color);
		}
		//motion detection
		if(DV_config_get(DV_MOTIONDETECTION) == 1)
		{
			gp_Icon_draw(fb, Icon_Manager[ICON_MOTIONDETECT_F].x_pos*HDMIxNum,Icon_Manager[ICON_MOTIONDETECT_F].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_MOTIONDETECT_F,Icon_Manager[ICON_MOTIONDETECT_F].tran_color);
		}
		//wdr
		gp_Icon_draw(fb, Icon_Manager[ICON_WDR_F+DV_config_get(DV_WDR)].x_pos*HDMIxNum,Icon_Manager[ICON_WDR_F+DV_config_get(DV_WDR)].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_WDR_F+DV_config_get(DV_WDR),Icon_Manager[ICON_WDR_F+DV_config_get(DV_WDR)].tran_color);

		//exporse
		gp_Icon_draw(fb, Icon_Manager[ICON_EXPOSURE_P2_0+DV_config_get(DV_EXPOSURE)].x_pos*HDMIxNum,Icon_Manager[ICON_EXPOSURE_P2_0+DV_config_get(DV_EXPOSURE)].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_EXPOSURE_P2_0+DV_config_get(DV_EXPOSURE),Icon_Manager[ICON_EXPOSURE_P2_0+DV_config_get(DC_EXPOSURE)].tran_color);

		//sdc
		gp_Icon_draw(fb, resolution.width-Icon_Manager[ICON_INT-dv_set.sd_check].x_pos*HDMIxNum,resolution.height -Icon_Manager[ICON_INT-dv_set.sd_check].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_INT-dv_set.sd_check,Icon_Manager[ICON_INT-dv_set.sd_check].tran_color);

		//mic
		gp_Icon_draw(fb, resolution.width-Icon_Manager[ICON_MIC_OFF-DV_config_get(DV_AUDIO)].x_pos*HDMIxNum,resolution.height -Icon_Manager[ICON_MIC_OFF-DV_config_get(DV_AUDIO)].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_MIC_OFF-DV_config_get(DV_AUDIO),Icon_Manager[ICON_MIC_OFF-DV_config_get(DV_AUDIO)].tran_color);
		
	}
	temp = time_num;
	if(dv_set.dv_ctrl == 0)
	{
		
		//time_str1 is time free
		if((dv_set.sd_check == 1)&&(dv_set.sdc_full == 0)&&(dv_set.dv_UI_flag == DV_FOREGROUND_MODE))
		{
			free_size = SDC_FreeSize()-5*1024;
			time_num = free_size/DV_BPS[DV_config_get(DV_RESOLUTION)];
		}
		else
		{
			time_num = 0;
		}
		//printf("time_num=[%d]\n",time_num);
		time_str1[7] = time_num%10 +0x30;		
		time_str1[6] = (time_num%60-time_num%10)/10 +0x30;		
		time_str1[4] = ((time_num/60)%60)%10 +0x30;		
		time_str1[3] = (((time_num/60)%60)%60-((time_num/60)%60)%10)/10 +0x30;		
		time_str1[1] = ((time_num/3600)%24)%10 +0x30;		
		time_str1[0] = (((time_num/3600)%24)%24-((time_num/3600)%24)%10)/10 +0x30;	
		ascii_str.str_ptr = time_str1;
		ap_state_resource_string_ascii_resolution_get(&time_str1[0],&str_res);
		//printf("resolution.width=%d,str_res.string_width=%d\n",resolution.width,str_res.string_width);
		if(dv_set.display_mode == SP_DISP_OUTPUT_HDMI)
		{
			ascii_str.pos_x = resolution.width - str_res.string_width - ICON_LEFT_X_POS-Y_STEP;
			ascii_str.pos_y = ICON_LEFT_Y_POS;			
		}
		else
		{
			ascii_str.pos_x = resolution.width - str_res.string_width - ICON_LEFT_X_POS;
			ascii_str.pos_y = ICON_LEFT_Y_POS;		
		}		
		fill_rectangle(fb,ascii_str.pos_x,ascii_str.pos_y ,resolution.width,resolution.height,str_res.string_width,str_res.string_height,TRANSPARENT_COLOR);
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);			
		fill_rectangle(fb,Icon_Manager[ICON_REDLIGHT].x_pos*HDMIxNum,Icon_Manager[ICON_REDLIGHT].y_pos*HDMIxNum ,resolution.width,resolution.height,Icon_Manager[ICON_REDLIGHT].icon_width,Icon_Manager[ICON_REDLIGHT].icon_height,TRANSPARENT_COLOR);
	}
	else
	{
		if(flip_flag == 0)
		{
			gp_Icon_draw(fb, Icon_Manager[ICON_REDLIGHT].x_pos*HDMIxNum,Icon_Manager[ICON_REDLIGHT].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_REDLIGHT,Icon_Manager[ICON_REDLIGHT].tran_color);
			LED_Set(NORMAL_LED,1);
		}
		else
		{
			fill_rectangle(fb,Icon_Manager[ICON_REDLIGHT].x_pos*HDMIxNum,Icon_Manager[ICON_REDLIGHT].y_pos*HDMIxNum ,resolution.width,resolution.height,Icon_Manager[ICON_REDLIGHT].icon_width,Icon_Manager[ICON_REDLIGHT].icon_height,TRANSPARENT_COLOR);
			LED_Set(NORMAL_LED,0);
		}
		//start dv time

		time(&t);
		tm = *localtime(&t);
		if(tm.tm_hour >= DV_start_time.tm_hour)
		time_num = 	tm.tm_hour - DV_start_time.tm_hour;
		else 
		time_num = 	tm.tm_hour - DV_start_time.tm_hour+24;
		if(tm.tm_min < DV_start_time.tm_min)
		time_num -= 1;		
		time_str1[1] = time_num%10 +0x30;
		time_str1[0] = time_num/10 +0x30;	
		//printf("DV Start Time = [%d]:",time_num);	
		if(tm.tm_min >= DV_start_time.tm_min)
		time_num = 	tm.tm_min - DV_start_time.tm_min;
		else 
		time_num = 	tm.tm_min - DV_start_time.tm_min+60;
		if(tm.tm_sec <  DV_start_time.tm_sec)
		{
			if(time_num != 0)
			{
				time_num -= 1;
				time_flag = 0;	
			}
			else
			{
				printf("get dv start time error\n");
				time_flag = 1;	
			}
		}
		
		time_str1[4] = time_num%10 +0x30;
		time_str1[3] = time_num/10 +0x30;
		//printf("[%d]:",time_num);
		if(tm.tm_sec >= DV_start_time.tm_sec)
		time_num = 	tm.tm_sec - DV_start_time.tm_sec;
		else  if(time_flag ==0)
		time_num = 	tm.tm_sec - DV_start_time.tm_sec+60;
		else
		{
			time_flag = 0;
			time_num = 0;
		}
	
		time_str1[7] = time_num%10 +0x30;
		time_str1[6] = time_num/10 +0x30;
		//printf("[%d]\n",time_num);
		
		ascii_str.str_ptr = time_str1;
		ap_state_resource_string_ascii_resolution_get(&time_str1[0],&str_res);
		ascii_str.pos_x = resolution.width - str_res.string_width - ICON_LEFT_X_POS;
		ascii_str.pos_y = ICON_LEFT_Y_POS;
		fill_rectangle(fb,ascii_str.pos_x,ascii_str.pos_y ,resolution.width,resolution.height,str_res.string_width,str_res.string_height,TRANSPARENT_COLOR);
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);			
	}
	time_num = temp;

	if((DV_config_get(DV_LDW)!=0)&&(DV_config_get(DV_RESOLUTION) == 0)) //only support 1080P and 720P
	{
		if(dv_set.display_mode == SP_DISP_OUTPUT_LCD)
		{

			ldw_rect.icon_width=240;					
			ldw_rect.icon_height=90;
			ldw_rect.x_pos=40;
			ldw_rect.y_pos=147; 		
			
			if(dv_set.LDW_speed_flag == 1)
			{
#if  LDW_LINEDISP
				if(gb_disp_LDWlineDisp)
				{					
					gb_disp_LDWlineDisp = 0;

					if(dv_set.LDW_v_offset_flag)
					{
						ldw_rect.icon_height=ldw_rect.icon_height*3/4;
						ldw_rect.y_pos=ldw_rect.y_pos*3/4+30; 			
					}
					fill_rectangle(fb,ldw_rect.x_pos,ldw_rect.y_pos ,resolution.width,resolution.height,ldw_rect.icon_width,ldw_rect.icon_height,TRANSPARENT_COLOR);//TRANSPARENT_COLOR	

					if((ldw_displine.LLcheckFlg && ldw_displine.RLcheckFlg && ldw_displine.LTP_X<=ldw_displine.RTP_X+10) || ldw_displine.LLAlarmFlg || ldw_displine.RLAlarmFlg)
					{//LR checkLine <10pixcel ,||  AlalrmLine not disp
						if(ldw_displine.LLAlarmFlg==1)
						{
							drawLDWLine(fb, ldw_displine.LT_alarmP_X, ldw_displine.LT_alarmP_Y,ldw_displine.LB_alarmP_X,ldw_displine.LB_alarmP_Y,5,255,0,0); 			
						}else if(ldw_displine.LLcheckFlg == 1)
						{
							drawLDWLine(fb, ldw_displine.LTP_X, ldw_displine.LTP_Y,ldw_displine.LBP_X,ldw_displine.LBP_Y,5,0,255,0); 	 									
						}
				
						if(ldw_displine.RLAlarmFlg==1)
						{
							drawLDWLine(fb, ldw_displine.RT_alarmP_X, ldw_displine.RT_alarmP_Y,ldw_displine.RB_alarmP_X,ldw_displine.RB_alarmP_Y,5,255,0,0); 	
						}else if(ldw_displine.RLcheckFlg == 1)
						{
							drawLDWLine(fb, ldw_displine.RTP_X, ldw_displine.RTP_Y,ldw_displine.RBP_X,ldw_displine.RBP_Y,5,0,255,0); 										
						}
					}else
					{
						//printf("LDW_line over 10 pixel\n");			
						drawLDWLine(fb, 159, 147, 46, 240,3, 200, 230, 200);
						drawLDWLine(fb, 161, 147, 274, 240,3, 200, 230, 200);						
					}
				}

				drawLDWLine(fb, 0, 147, 320, 147,3, 0xff, 0, 0);	
#else 
				drawLDWLine(fb, 159, 147, 46, 240,3, 200, 230, 200);
				drawLDWLine(fb, 161, 147, 274, 240,3, 200, 230, 200);	
				drawLDWLine(fb, 0, 147, 320, 147,3, 0xff, 0, 0);

#endif				

				ldw_rect.icon_width=240;					
				ldw_rect.icon_height=9;
				ldw_rect.x_pos=40;
				ldw_rect.y_pos=137;

				if(dv_set.LDW_v_offset_flag)
				{
					ldw_rect.icon_height=ldw_rect.icon_height*3/4;
					ldw_rect.y_pos=ldw_rect.y_pos*3/4+30; 			
				}
				fill_rectangle(fb,ldw_rect.x_pos,ldw_rect.y_pos ,resolution.width,resolution.height,ldw_rect.icon_width,ldw_rect.icon_height,TRANSPARENT_COLOR);//TRANSPARENT_COLOR 
				gp_Icon_draw(fb, Icon_Manager[ICON_LDW_1].x_pos*HDMIxNum,resolution.height -Icon_Manager[ICON_LDW_1].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_LDW_1,Icon_Manager[ICON_LDW_1].tran_color);	
			}
			else
			{
				
				fill_rectangle(fb,ldw_rect.x_pos,ldw_rect.y_pos ,resolution.width,resolution.height,ldw_rect.icon_width,ldw_rect.icon_height,TRANSPARENT_COLOR);//TRANSPARENT_COLOR	

				drawLDWLine(fb, 159, 147, 46, 240,3, 200, 230, 200);
				drawLDWLine(fb, 161, 147, 274, 240,3, 200, 230, 200);	
				drawLDWLine(fb, 0, 147, 320, 147,3, 0xff, 0, 0);
						
				fill_rectangle(fb,Icon_Manager[ICON_LDW_1].x_pos*HDMIxNum,resolution.height -Icon_Manager[ICON_LDW_1].y_pos*HDMIxNum ,resolution.width,resolution.height,Icon_Manager[ICON_LDW_1].icon_width,Icon_Manager[ICON_LDW_1].icon_height,TRANSPARENT_COLOR);	
			}		
		}
	}
 	if(foreground_draw_flag!=0)
	{
		if(setting_config_get(SET_GSENSOR1)!=0)
		{
			ascii_str.font_color = FONT_SELECT_COLOR;
			ascii_str.pos_x -= 40*HDMIxNum;
			ascii_str.str_ptr = "P";
			ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
			ascii_str.font_color = FONT_COLOR;
		}
		if(DV_config_get(DV_GSENSOR)!=0)
		{
			ascii_str.font_color = FONT_SELECT_COLOR;
			if(HDMIxNum != 1)
			ascii_str.pos_x = resolution.width - str_res.string_width - ICON_LEFT_X_POS - 20*HDMIxNum-40;
			else
			ascii_str.pos_x = resolution.width - str_res.string_width - ICON_LEFT_X_POS - 20*HDMIxNum;	
			ascii_str.str_ptr = "G";
			ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
			ascii_str.font_color = FONT_COLOR;
		}		
	}
	//charge and battery 0: no power  1: zero battery 	2: one battery 3:two battery 4:three battery 5:charge  
	if(dv_set.battery_state == 0)
	{
			gp_Icon_draw(fb, resolution.width-Icon_Manager[ICON_BATTERYX].x_pos*HDMIxNum,resolution.height-Icon_Manager[ICON_BATTERYX].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_BATTERYX,Icon_Manager[ICON_BATTERYX].tran_color);		
	}
	else if(dv_set.battery_state == 5)
	{
		if(flip_flag1 == 0)
		{
			gp_Icon_draw(fb, resolution.width-Icon_Manager[ICON_CHARGE].x_pos*HDMIxNum,resolution.height-Icon_Manager[ICON_CHARGE].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_CHARGE,Icon_Manager[ICON_CHARGE].tran_color);

		}
		else
		{
			fill_rectangle(fb,resolution.width-Icon_Manager[ICON_CHARGE].x_pos*HDMIxNum,resolution.height-Icon_Manager[ICON_CHARGE].y_pos*HDMIxNum ,resolution.width,resolution.height,Icon_Manager[ICON_CHARGE].icon_width*HDMIxNum,Icon_Manager[ICON_CHARGE].icon_height*HDMIxNum,TRANSPARENT_COLOR);
		}		
	}
	else
	{
		if(dv_set.battery_state > 5)
		gp_Icon_draw(fb, resolution.width-Icon_Manager[ICON_CHARGE+4].x_pos*HDMIxNum,resolution.height-Icon_Manager[ICON_CHARGE+4].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_CHARGE+4,Icon_Manager[ICON_CHARGE+4].tran_color);
		else
		gp_Icon_draw(fb, resolution.width-Icon_Manager[ICON_CHARGE+dv_set.battery_state].x_pos*HDMIxNum,resolution.height-Icon_Manager[ICON_CHARGE+dv_set.battery_state].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_CHARGE+dv_set.battery_state,Icon_Manager[ICON_CHARGE+dv_set.battery_state].tran_color);
	}

	//date and time
	RTC_get();	
	ap_setting_date_time_string_process(0xFF);
	ascii_str.str_ptr = dt_str;
	ap_state_resource_string_ascii_resolution_get(&dt_str[0],&str_res);	
	ascii_str.pos_x = resolution.width/2 -  str_res.string_width/2;
	ascii_str.pos_y = resolution.height - str_res.string_height - ICON_LEFT_Y_POS;
	fill_rectangle(fb,ascii_str.pos_x,ascii_str.pos_y ,resolution.width,resolution.height,str_res.string_width,str_res.string_height,TRANSPARENT_COLOR);	
	ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);	


	/*if(dv_set.zoom_num != 0)
	{
		zoom[1] = dv_set.zoom_num/10+1+0x30;
		zoom[3] =  dv_set.zoom_num%10+0x30;
		ascii_str.str_ptr = zoom;
		ap_state_resource_string_ascii_resolution_get(&zoom[0],&str_res);	
		ascii_str.pos_x = Icon_Manager[ICON_WDR_F].x_pos;
		ascii_str.pos_y = resolution.height - str_res.string_height - ICON_LEFT_Y_POS-Y_STEP;
		fill_rectangle(fb,ascii_str.pos_x,ascii_str.pos_y ,resolution.width,resolution.height,str_res.string_width,str_res.string_height,TRANSPARENT_COLOR);	
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);		
	}
	else
	{
		ap_state_resource_string_ascii_resolution_get(&zoom[0],&str_res);	
		ascii_str.pos_x = Icon_Manager[ICON_WDR_F].x_pos;
		ascii_str.pos_y = resolution.height - str_res.string_height - ICON_LEFT_Y_POS-Y_STEP;
		fill_rectangle(fb,ascii_str.pos_x,ascii_str.pos_y ,resolution.width,resolution.height,str_res.string_width,str_res.string_height,TRANSPARENT_COLOR);	
	}*/
	if(dv_set.lock_flag == 1)
	{
		gp_Icon_draw(fb, Icon_Manager[ICON_PROTECT_F].x_pos*HDMIxNum,Icon_Manager[ICON_PROTECT_F].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_PROTECT_F,Icon_Manager[ICON_PROTECT_F].tran_color);		
	}else
	{
		fill_rectangle(fb,Icon_Manager[ICON_PROTECT_F].x_pos*HDMIxNum,Icon_Manager[ICON_PROTECT_F].y_pos*HDMIxNum ,resolution.width,resolution.height,Icon_Manager[ICON_PROTECT_F].icon_width*HDMIxNum,Icon_Manager[ICON_PROTECT_F].icon_height*HDMIxNum,TRANSPARENT_COLOR);			
	}
	
	if(foreground_draw_flag>=2)
	{
		foreground_draw_flag = 0;
		cur_draw_flag = DV_FOREGROUND_MODE;
	}	
	dispFlip(hDisp);
	if(cur_draw_flag !=DV_FOREGROUND_MODE)
	disp_OSD0_Set_Alpha(hDisp,100);
	
}
/*==========================================================
dv task entry function
===========================================================*/
int
DV_task_entry(
	int argc,
	char **argv
)
{
	UINT16 i;
	gp_size_t resolution;
	UINT8* addr;
	UINT16* addr1;
	STRING_ASCII_INFO ascii_str;
	STRING_INFO str;
	UINT16 charge_flag = 1;
	int ret;
	UINT16 *fb;
	UINT8 msg_id,msg_prio;
	struct timespec timeout;
	time_t t;
	
	dv_set.dv_UI_flag = DV_EXIT_MODE;
	//run dv and init pipe
	while(dv_set.change_disp == 1);
	dv_set.zoom_flag = 0;
	dv_set.zoom_num = 0;
	DV_pipe_init();
	if(dv_set.display_mode == SP_DISP_OUTPUT_LCD)
		CVR_Set_PIPE(CMD_SET_DISPLAY_MODE,1);
	else
		CVR_Set_PIPE(CMD_SET_DISPLAY_MODE,0);
		
	dispGetResolution(hDisp, &resolution);
	//printf("resolution.width=%d,resolution.height=%d\n",resolution.width,resolution.height);	
	//init UI for DV
	dv_set.ui_show = 1;
	DV_UI_foreground();
	
	do
	{
		clock_gettime(CLOCK_REALTIME, &timeout);
		timeout.tv_nsec += 10000;
		ret = mq_timedreceive(menumodeMsgQ, (char *)&msg_id, 1, (unsigned int *)&msg_prio,&timeout);
		//if(ret != -1)	
		//DV_UI_foreground();
	}
	while((dv_set.dv_UI_flag == DV_EXIT_MODE)||(ret != -1));
	dv_set.dv_UI_flag = DV_FOREGROUND_MODE;
	if(dv_set.display_mode == SP_DISP_OUTPUT_LCD)
	{
		LCD_Backlight_Set(1);
	}
	if(dv_set.default_setting_flag == 1)
	{	
		dv_set.default_setting_flag = 0;
		CVR_Set_PIPE_With_CMD(CMD_SET_DEFAULT_SETTING,0);
	}
	if(dv_set.date_type_flag == 1)
	{
		CVR_Set_PIPE(CMD_SET_DATE_TYPE,setting_config_get(SET_DATA_FORMAT));
		dv_set.date_type_flag = 0;
	}	

	printf("dv_set.upgrade_flag = %d\n",dv_set.upgrade_flag);
	if(dv_set.upgrade_flag == 2)
	{
		if(dv_set.dv_ctrl == 1)
		{
			CVR_Set_PIPE_With_CMD(CMD_STOP_DV,1);
			dv_set.dv_ctrl =0;				
		}	
		dv_set.upgrade_flag = SPI_Flash_update();
		printf("dv_set.upgrade_flag = %d\n",dv_set.upgrade_flag);
		if((dv_set.display_mode != SP_DISP_OUTPUT_HDMI)&&(dv_set.upgrade_flag != 1))
		{
			CVR_Set_PIPE_With_CMD(CMD_START_DV,0);
			time(&t);
			DV_start_time= *localtime(&t);	
			gettimeofday(&cap_start,NULL);
			dv_set.dv_ctrl =1;
			printf("First start DV!\n"); 
		}
	}
	
	while(1)
	{

		ret = mq_receive(menumodeMsgQ, (char *)&msg_id, 1, (unsigned int *)&msg_prio);
		switch(msg_id){
			case DV_FOREGROUND_MODE:
				dv_set.draw_start = 1;
				DV_UI_foreground();
				dv_set.draw_start = 0;
			break;
			case DV_MENU_MODE:
				dv_set.draw_start = 1;
				disp_OSD0_Set_Alpha(hDisp,65);
				UI_Menu_draw(Playmode_flag);
				dv_set.draw_start = 0;
			break;
			case DV_SETTING_MODE:
				dv_set.draw_start = 1;
				DV_UI_Setting();
				dv_set.draw_start = 0;
			break;
			default:
			break;	
		}
		if(msg_id == DV_EXIT_MODE)
		{
			CVR_Set_PIPE_With_CMD(CMD_SET_EXIT,1);
			break;
		}					//pend message from TimeKeyThread and update UI/send message to ipcam
	}
	
	pipeDelete();	
	return 0;
}
/*==========================================================
dv menu  UI draw function
===========================================================*/
void UI_Menu_draw(UINT8 mode)
{
	STRING_INFO sub_str;
	STRING_ASCII_INFO ascii_str;
	t_STRING_TABLE_STRUCT str_res;
	gp_size_t resolution;
	UINT16 *fb;
	UINT16* addr1;
	
	UINT8 item_total_num = 0;
	UINT8 item_page_num = 0;
	UINT8 cur_page_num=0;
	SINT8 menu_select_count;															
	static SINT8 item_select_count;
	UINT8 menu_total_num;
	UINT8 total_item1=0;
	UINT8 item1_offset=0;
	menuManager_t *p_menu;
	if(dv_set.change_disp != 0)
		return;
	if(dv_set.display_mode == SP_DISP_OUTPUT_LCD)
	{
		dv_set.display_x = DISP_LCD_X_OFFSET;
		dv_set.display_y = DISP_LCD_Y_OFFSET;		
	}
	else if(dv_set.display_mode == SP_DISP_OUTPUT_TV)
	{
		dv_set.display_x = DISP_TV_X_OFFSET;
		dv_set.display_y = DISP_TV_Y_OFFSET;
	}
	else if(dv_set.display_mode == SP_DISP_OUTPUT_HDMI)
	{
		dv_set.display_x = DISP_HDMI_X_OFFSET;
		dv_set.display_y = DISP_HDMI_Y_OFFSET;		
	}
	else
	{
		dv_set.display_x = 0;
		dv_set.display_y = 0;		
	}

	dispGetResolution(hDisp, &resolution);	
	addr1 = fb = (UINT16*)dispGetFramebuffer(hDisp);
	for (int y = 0; y <resolution.height; y++) {		
		for (int x = 0; x < resolution.width; x++) {			
			*addr1++ = TRANSPARENT_COLOR;		
		}	
	}
	cur_draw_flag = DV_MENU_MODE;
	foreground_draw_flag=0;
	/*
	draw_rectangle(fb,UI_MENU_X_POS,UI_MENU_Y_POS,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS,resolution.height-2*UI_MENU_Y_POS,0x001f,TRANSPARENT_COLOR);
	fill_rectangle(fb,UI_MENU_X_POS+1,UI_MENU_Y_POS+1,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-2,Y_STEP-3,TRANSPARENT_COLOR-1);
	fill_rectangle(fb,UI_MENU_X_POS+1,resolution.height -UI_MENU_Y_POS -1 -(Y_STEP-8) ,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-2,Y_STEP-8,TRANSPARENT_COLOR-1);
	*/
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_MENU_B].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_MENU_B].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_MENU_B,Icon_Manager[ICON_MENU_B].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_TOP_S].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_TOP_S].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_TOP_S,Icon_Manager[ICON_TOP_S].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_SETUP].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_SETUP].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_SETUP,Icon_Manager[ICON_SETUP].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_MENU].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_MENU].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_MENU,Icon_Manager[ICON_MENU].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_UP].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_UP].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_UP,Icon_Manager[ICON_UP].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_DOWN].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_DOWN].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_DOWN,Icon_Manager[ICON_DOWN].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_LEFT].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_LEFT].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_LEFT,Icon_Manager[ICON_LEFT].tran_color-1);
	if(dv_set.menu_select_flag != 0)
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_SELECT].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_SELECT].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_SELECT,Icon_Manager[ICON_SELECT].tran_color-1);	
	else
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_RIGHT].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_RIGHT].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_RIGHT,Icon_Manager[ICON_RIGHT].tran_color-1);	
		
	if(mode == CMD_VIDEO_MODE)
	{
		menu_total_num = DV_MENU_NUM;
		p_menu = &DV_MENU[0];
		gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_VIDEO_M].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_VIDEO_M].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_VIDEO_M,Icon_Manager[ICON_VIDEO_M].tran_color-1);	
	}
	else if(mode == CMD_STILL_MODE)
	{
		menu_total_num = DC_MENU_NUM;
		p_menu = &DC_MENU[0];		
		gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_CAPTURE_M].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_CAPTURE_M].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_CAPTURE_M,Icon_Manager[ICON_CAPTURE_M].tran_color-1);	
	}
	else if(mode == CMD_PLAYBACK_MODE)
	{
		menu_total_num = PLAY_MENU_NUM;
		p_menu = &PLAY_MENU[0];		
		gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_PLAYBACK_M].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_PLAYBACK_M].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_PLAYBACK_M,Icon_Manager[ICON_PLAYBACK_M].tran_color-1);	
	}
	else
	{
		return;
	}
	
		menu_select_count = dv_set.menu_select_count;
		if(menu_select_count > menu_total_num-1)
		{
			menu_select_count = 0;
			dv_set.menu_select_count = 0;
		}
		if(dv_set.menu_select_count < 0)
		{
			menu_select_count = menu_total_num-1;
			dv_set.menu_select_count = menu_total_num-1;
		}
		cur_page_num = menu_select_count/PAGE_ITEM_NUM;
		menu_select_count %= PAGE_ITEM_NUM;		
		//printf("menu_total_num=%d\n",menu_total_num);
		//printf("menu_select_count=%d\n",menu_select_count);
		//printf("cur_page_num=%d\n",cur_page_num);	
		
	/*fill_rectangle(fb,UI_MENU_X_POS+4,UI_MENU_Y_POS+Y_STEP+menu_select_count*Y_STEP,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-10,ICON_HEIGHT,0x001f);
	*/
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_MENU_S].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_MENU_S].y_pos*HDMIxNum+HDMI_Y_offset+menu_select_count*Y_STEP*HDMIxNum,resolution.width, resolution.height,ICON_MENU_S,Icon_Manager[ICON_MENU_S].tran_color-1);	
	sub_str.font_type = 0;	
	sub_str.buff_w = resolution.width;
	sub_str.buff_h = resolution.height;	
	for(int i=0;i<PAGE_ITEM_NUM;i++)
	{	
		if((cur_page_num*PAGE_ITEM_NUM+i) < menu_total_num)
		{	
			//show icon
			if(menu_select_count != i)
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[p_menu[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[p_menu[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,p_menu[cur_page_num*PAGE_ITEM_NUM+i].icon_Index,Icon_Manager[p_menu[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].tran_color-1);
			else 
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[p_menu[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[p_menu[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,p_menu[cur_page_num*PAGE_ITEM_NUM+i].icon_Index,0x001f);	
			if(dv_set.menu_select_flag == 0)
			{
				//show font
				if(menu_select_count == i)	
				sub_str.font_color = FONT_SELECT_COLOR;
				else				
				sub_str.font_color = p_menu[cur_page_num*PAGE_ITEM_NUM+i].color;
				sub_str.pos_x = dv_set.display_x+Icon_Manager[p_menu[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].x_pos*HDMIxNum + Icon_Manager[p_menu[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].icon_width*HDMIxNum + 8+HDMI_X_offset;	
				sub_str.pos_y = dv_set.display_y+Icon_Manager[p_menu[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].y_pos*HDMIxNum + 6+HDMI_Y_offset;
				if(dv_set.display_mode == SP_DISP_OUTPUT_HDMI)
				{
					sub_str.pos_y += 12;
				}
				sub_str.language = setting_config_get(SET_LANGUAGE);
				sub_str.str_idx = p_menu[cur_page_num*PAGE_ITEM_NUM+i].str_index;
				ap_state_resource_string_draw((INT16U *)fb, &sub_str);										
			}
		}
	}
	
	if(dv_set.menu_select_flag == 0)
	{
		sub_str.font_color = p_menu[0].color;		
		sub_str.language = setting_config_get(SET_LANGUAGE);
		if(mode == CMD_VIDEO_MODE)
		sub_str.str_idx = STR_VIDEO;
		else if(mode == CMD_STILL_MODE)
		sub_str.str_idx = STR_CAPTURE;	
		else if(mode == CMD_PLAYBACK_MODE)
		sub_str.str_idx = STR_PLAYBACK;			
		ap_state_resource_string_resolution_get(&sub_str,&str_res);
		if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
		{		
			sub_str.pos_x = dv_set.display_x+UI_MENU_X_POS + Icon_Manager[ICON_MENU_B].icon_width*HDMIxNum - str_res.string_width - 16;
			sub_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 25 - str_res.string_height/2;
		}
		else
		{
			sub_str.pos_x = dv_set.display_x+1200 - str_res.string_width;
			sub_str.pos_y = dv_set.display_y+60;//  + str_res.string_height/2;			
		}
		ap_state_resource_string_draw((INT16U *)fb, &sub_str);	
		//show page num
		ascii_str.font_color = FONT_COLOR;
		ascii_str.font_type = 0;
		ascii_str.buff_w = resolution.width;
		ascii_str.buff_h = resolution.height;
		if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
		{
			ascii_str.pos_x = dv_set.display_x+UI_MENU_X_POS + 280;
			ascii_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 212;
		}
		else
		{
			ascii_str.pos_x = dv_set.display_x+UI_MENU_X_POS + 1100;
			ascii_str.pos_y = dv_set.display_y + 842;			
		}
		page[0] = cur_page_num + 1 + 0x30;
		if((menu_total_num%PAGE_ITEM_NUM) != 0)
		page[2] = (menu_total_num/PAGE_ITEM_NUM) + 1 + 0x30;
		else 
		page[2] = menu_total_num/PAGE_ITEM_NUM + 0x30;
		ascii_str.str_ptr = page;
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);

		
	}
	if(dv_set.menu_select_flag == 1)
	{

		//draw_rectangle(fb,UI_MENU_X_POS+4+ICON_WIDTH+2,UI_MENU_Y_POS+Y_STEP-2,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2,resolution.height-2*UI_MENU_Y_POS-2*Y_STEP+7+2,0x001f,TRANSPARENT_COLOR);
		gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_ITEM_B].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_ITEM_B,Icon_Manager[ICON_ITEM_B].tran_color-1);	
		
		item_total_num = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item_num;
		item_select_count = dv_set.item_select_count;//DV_config_get(cur_page_num*PAGE_ITEM_NUM+menu_select_count);
		
		if(item_select_count > item_total_num-1)
		{
			item_select_count = 0;
			dv_set.item_select_count = 0;
		}
		if(dv_set.item_select_count < 0)
		{
			item_select_count = item_total_num-1;
			dv_set.item_select_count = item_total_num-1;
		}
		item_page_num = item_select_count/PAGE_ITEM_NUM;
		item_select_count %= PAGE_ITEM_NUM;
		//printf("item_total_num=%d\n",item_total_num);
		//printf("item_select_count=%d\n",item_select_count);
		//printf("item_page_num=%d\n",item_page_num);
		//fill_rectangle(fb,UI_MENU_X_POS+4+ICON_WIDTH+2+15,UI_MENU_Y_POS+Y_STEP+item_select_count*Y_STEP,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2-30,ICON_HEIGHT,0x001f);
		gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_ITEM_S].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+(Icon_Manager[ICON_ITEM_S].y_pos+HDMI_Y_offset+item_select_count*(Y_STEP-2))*HDMIxNum,resolution.width, resolution.height,ICON_ITEM_S,Icon_Manager[ICON_ITEM_S].tran_color-1);	

		for(int i=0;i<PAGE_ITEM_NUM;i++)
		{
			if((item_page_num*PAGE_ITEM_NUM+i)<item_total_num)					
			{
				//show font
				if(item_select_count == i)	
				sub_str.font_color = FONT_SELECT_COLOR;
				else
				sub_str.font_color = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;
				sub_str.language = setting_config_get(SET_LANGUAGE);
				sub_str.str_idx = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[item_page_num*PAGE_ITEM_NUM+i].str_index;	
				ap_state_resource_string_resolution_get(&sub_str,&str_res);
				//sub_str.pos_x = UI_MENU_X_POS+4+ICON_WIDTH+2 + (resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2)/2- str_res.string_width/2;
				//sub_str.pos_y = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[item_page_num*PAGE_ITEM_NUM+i].yPos + 6;				
				sub_str.pos_x = dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum + Icon_Manager[ICON_ITEM_B].icon_width*HDMIxNum/2 - str_res.string_width/2;
				sub_str.pos_y = dv_set.display_y+Icon_Manager[ICON_ITEM_S].y_pos*HDMIxNum + Icon_Manager[ICON_ITEM_S].icon_height*HDMIxNum/2 - str_res.string_height/2 + i*38*HDMIxNum;	
				ap_state_resource_string_draw((INT16U *)fb, &sub_str);				
			}
		}
		sub_str.font_color = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;
		sub_str.language = setting_config_get(SET_LANGUAGE);
		sub_str.str_idx = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].str_index;
		ap_state_resource_string_resolution_get(&sub_str,&str_res);
		if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
		{		
			sub_str.pos_x = dv_set.display_x+UI_MENU_X_POS + Icon_Manager[ICON_MENU_B].icon_width*HDMIxNum - str_res.string_width - 16;
			sub_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 25 - str_res.string_height/2;
		}
		else
		{
			sub_str.pos_x = dv_set.display_x+1200 - str_res.string_width;
			sub_str.pos_y = dv_set.display_y+60;//  + str_res.string_height/2;			
		}					
		ap_state_resource_string_draw((INT16U *)fb, &sub_str);				
	}
	else if(dv_set.menu_select_flag == 2)
	{
			//draw_rectangle(fb,UI_MENU_X_POS+4+ICON_WIDTH+2,UI_MENU_Y_POS+Y_STEP-2,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2,resolution.height-2*UI_MENU_Y_POS-2*Y_STEP+7+2,0x001f,TRANSPARENT_COLOR);
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_ITEM_B].y_pos*HDMIxNum+HDMI_Y_offset,resolution.width, resolution.height,ICON_ITEM_B,Icon_Manager[ICON_ITEM_B].tran_color-1);	

			if(Playmode_flag != CMD_VIDEO_MODE)
			{
				sub_str.font_color = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;
				sub_str.language = setting_config_get(SET_LANGUAGE);
				sub_str.str_idx = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].str_index;
				ap_state_resource_string_resolution_get(&sub_str,&str_res);
				if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
				{		
					sub_str.pos_x = dv_set.display_x+UI_MENU_X_POS + Icon_Manager[ICON_MENU_B].icon_width*HDMIxNum - str_res.string_width - 16;
					sub_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 25 - str_res.string_height/2;
				}
				else
				{
					sub_str.pos_x = dv_set.display_x+1200 - str_res.string_width;
					sub_str.pos_y = dv_set.display_y+60;//  + str_res.string_height/2;			
				}			
				ap_state_resource_string_draw((INT16U *)fb, &sub_str);	

				if(dv_set.item_select_count > 3)
					dv_set.item_select_count = 2;
				else if(dv_set.item_select_count < 2)
					dv_set.item_select_count = 3;
				//fill_rectangle(fb,UI_MENU_X_POS+4+ICON_WIDTH+2+15,UI_MENU_Y_POS+Y_STEP+dv_set.item_select_count*Y_STEP,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2-30,ICON_HEIGHT,0x001f);
				gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_ITEM_S].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_ITEM_S].y_pos*HDMIxNum+HDMI_Y_offset+dv_set.item_select_count*(Y_STEP-2)*HDMIxNum,resolution.width, resolution.height,ICON_ITEM_S,Icon_Manager[ICON_ITEM_S].tran_color-1);	

				for(int i=0;i<3;i++)
				{
					//show font
					if(dv_set.item_select_count == (i+1))
					sub_str.font_color = FONT_SELECT_COLOR;
					else
					sub_str.font_color = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;

					sub_str.language = setting_config_get(SET_LANGUAGE);
					if(i == 0)
					sub_str.str_idx = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[item_select_count].str_index;
					else
					{
						if(menu_select_count == 0)
							sub_str.str_idx = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[2+i].str_index;	
						else
						{
							sub_str.str_idx = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[4+i].str_index;	
						}
					}
					ap_state_resource_string_resolution_get(&sub_str,&str_res);
					sub_str.pos_x = dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum + Icon_Manager[ICON_ITEM_B].icon_width*HDMIxNum/2 - str_res.string_width/2;
					if(i > 0)
					sub_str.pos_y = dv_set.display_y+Icon_Manager[ICON_ITEM_S].y_pos*HDMIxNum + Icon_Manager[ICON_ITEM_S].icon_height*HDMIxNum/2 - str_res.string_height/2 + (i+1)*38*HDMIxNum;
					else
					sub_str.pos_y = dv_set.display_y+Icon_Manager[ICON_ITEM_S].y_pos*HDMIxNum + Icon_Manager[ICON_ITEM_S].icon_height*HDMIxNum/2 - str_res.string_height/2;	
					ap_state_resource_string_draw((INT16U *)fb, &sub_str);				
				}
			}
			else
			{
				if(dv_set.item_select_count == 0)
				{
					total_item1=2;
					item1_offset=6;					
				}
				else if(dv_set.item_select_count == 1)
				{
					total_item1=3;
					item1_offset=8;					
				}					
				else if(dv_set.item_select_count == 2)
				{
					total_item1=3;
					item1_offset=11;					
				}
				else if(dv_set.item_select_count == 3)
				{
					total_item1=2;
					item1_offset=14;					
				}
				else if(dv_set.item_select_count == 4)
				{
					total_item1=3;
					item1_offset=16;					
				}	
				else if(dv_set.item_select_count == 5)
				{
					total_item1=2;
					item1_offset=19;					
				}					
				if(dv_set.item_select_count1 > total_item1-1)
				{
					dv_set.item_select_count1 = 0;
				}
				if(dv_set.item_select_count1 < 0)
				{
					dv_set.item_select_count1 = total_item1-1;
				}	
				sub_str.font_color = p_menu[dv_set.menu_select_count].color;
				sub_str.language = setting_config_get(SET_LANGUAGE);
				sub_str.str_idx = p_menu[dv_set.menu_select_count].item[dv_set.item_select_count].str_index;
				ap_state_resource_string_resolution_get(&sub_str,&str_res);
				if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
				{		
					sub_str.pos_x = dv_set.display_x+UI_MENU_X_POS + Icon_Manager[ICON_MENU_B].icon_width*HDMIxNum - str_res.string_width - 16;
					sub_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 25 - str_res.string_height/2;
				}
				else
				{
					sub_str.pos_x = dv_set.display_x+1200 - str_res.string_width;
					sub_str.pos_y = dv_set.display_y+60;//  + str_res.string_height/2;			
				}			
				ap_state_resource_string_draw((INT16U *)fb, &sub_str);
				gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_ITEM_S].x_pos*HDMIxNum+HDMI_X_offset,dv_set.display_y+Icon_Manager[ICON_ITEM_S].y_pos*HDMIxNum+HDMI_Y_offset+dv_set.item_select_count1*(Y_STEP-2)*HDMIxNum,resolution.width, resolution.height,ICON_ITEM_S,Icon_Manager[ICON_ITEM_S].tran_color-1);	
				//printf("dv_set.menu_select_flag= %d,dv_set.item_select_count1=%d\n",dv_set.menu_select_flag,dv_set.item_select_count1);
				
				for(int i=0;i<total_item1;i++)
				{
							//show font
							if(dv_set.item_select_count1 == i)	
							sub_str.font_color = FONT_SELECT_COLOR;
							else
							sub_str.font_color = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;
							sub_str.language = setting_config_get(SET_LANGUAGE);
							printf("item1_offset+i = %d\n",item1_offset+i);
							sub_str.str_idx = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[item1_offset+i].str_index;
							printf("sub_str.str_idx = %d\n",sub_str.str_idx);
							ap_state_resource_string_resolution_get(&sub_str,&str_res);
							//sub_str.pos_x = UI_MENU_X_POS+4+ICON_WIDTH+2 + (resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2)/2- str_res.string_width/2;
							//sub_str.pos_y = p_menu[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[item_page_num*PAGE_ITEM_NUM+i].yPos + 6;				
							sub_str.pos_x = dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum + Icon_Manager[ICON_ITEM_B].icon_width*HDMIxNum/2 - str_res.string_width/2;
							sub_str.pos_y = dv_set.display_y+Icon_Manager[ICON_ITEM_S].y_pos*HDMIxNum + Icon_Manager[ICON_ITEM_S].icon_height*HDMIxNum/2 - str_res.string_height/2 + i*38*HDMIxNum;	
							ap_state_resource_string_draw((INT16U *)fb, &sub_str);				
				}				
			}
	}
	
	dispFlip(hDisp);

}
/*==========================================================
setting  UI draw function
===========================================================*/
void DV_UI_Setting(void)
{
	STRING_INFO sub_str;
	STRING_ASCII_INFO ascii_str;
	t_STRING_TABLE_STRUCT str_res;
	gp_size_t resolution;
	UINT16 *fb;
	UINT16* addr1;

	UINT8 item_total_num = 0;
	UINT8 item_page_num = 0;
	UINT8 cur_page_num=0;
	SINT8 menu_select_count;															
	SINT8 item_select_count;
	UINT8 date_type;
	UINT8 item_num_temp;
	UINT16 time_x_pos; 
	UINT16 old_str_len;
	if(dv_set.change_disp != 0)
		return;	
	dispGetResolution(hDisp, &resolution);	
	addr1 = fb = (UINT16*)dispGetFramebuffer(hDisp);
	for (int y = 0; y <resolution.height; y++) {		
		for (int x = 0; x < resolution.width; x++) {			
			*addr1++ = TRANSPARENT_COLOR;		
		}	
	}
	cur_draw_flag = DV_SETTING_MODE;
	foreground_draw_flag=0;

	if(dv_set.display_mode == SP_DISP_OUTPUT_LCD)
	{
		dv_set.display_x = DISP_LCD_X_OFFSET;
		dv_set.display_y = DISP_LCD_Y_OFFSET;		
	}
	else if(dv_set.display_mode == SP_DISP_OUTPUT_TV)
	{
		dv_set.display_x = DISP_TV_X_OFFSET;
		dv_set.display_y = DISP_TV_Y_OFFSET;
	}
	else if(dv_set.display_mode == SP_DISP_OUTPUT_HDMI)
	{
		dv_set.display_x = DISP_HDMI_X_OFFSET;
		dv_set.display_y = DISP_HDMI_Y_OFFSET;		
	}
	else
	{
		dv_set.display_x = 0;
		dv_set.display_y = 0;		
	}

	if(dv_set.sd_check == 2)
	{	
		gp_Icon_draw(fb, (resolution.width-Icon_Manager[ICON_NOFILE].icon_width*HDMIxNum)/2,(resolution.height-Icon_Manager[ICON_NOFILE].icon_height*HDMIxNum)/2,resolution.width, resolution.height,ICON_NOFILE,Icon_Manager[ICON_NOFILE].tran_color);
		sub_str.font_type = 0;	
		sub_str.font_color = FONT_COLOR;
		sub_str.buff_w = resolution.width;
		sub_str.buff_h = resolution.height;	
		sub_str.language = setting_config_get(SET_LANGUAGE);
		sub_str.str_idx = STR_NO_CARD;		
		ap_state_resource_string_resolution_get(&sub_str,&str_res);
		sub_str.pos_x = (resolution.width -str_res.string_width) /2;
		sub_str.pos_y = (resolution.height -str_res.string_height) /2;
		ap_state_resource_string_draw((INT16U *)fb, &sub_str);		
		dispFlip(hDisp);
		return;		
	}	
	
	/*
	draw_rectangle(fb,UI_MENU_X_POS,UI_MENU_Y_POS,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS,resolution.height-2*UI_MENU_Y_POS,0x001f,TRANSPARENT_COLOR);
	fill_rectangle(fb,UI_MENU_X_POS+1,UI_MENU_Y_POS+1,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-2,Y_STEP-3,TRANSPARENT_COLOR-1);
	fill_rectangle(fb,UI_MENU_X_POS+1,resolution.height -UI_MENU_Y_POS -1 -(Y_STEP-8) ,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-2,Y_STEP-8,TRANSPARENT_COLOR-1);
	*/
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_MENU_B].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_MENU_B].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_MENU_B,Icon_Manager[ICON_MENU_B].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_TOP_S].x_pos*HDMIxNum+48*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_TOP_S].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_TOP_S,Icon_Manager[ICON_TOP_S].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_SETUP].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_SETUP].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_SETUP,Icon_Manager[ICON_SETUP].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_MENU].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_MENU].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_MENU,Icon_Manager[ICON_MENU].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_UP].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_UP].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_UP,Icon_Manager[ICON_UP].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_DOWN].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_DOWN].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_DOWN,Icon_Manager[ICON_DOWN].tran_color-1);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_LEFT].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_LEFT].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_LEFT,Icon_Manager[ICON_LEFT].tran_color-1);
	if(dv_set.menu_select_flag != 0)
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_SELECT].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_SELECT].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_SELECT,Icon_Manager[ICON_SELECT].tran_color-1);	
	else
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_RIGHT].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_RIGHT].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_RIGHT,Icon_Manager[ICON_RIGHT].tran_color-1);	
		
	if(Playmode_flag == CMD_VIDEO_MODE)
	{
		gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_VIDEO_M].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_VIDEO_M].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_VIDEO_M,Icon_Manager[ICON_VIDEO_M].tran_color-1);	
	}
	else if(Playmode_flag == CMD_STILL_MODE)
	{	
		gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_CAPTURE_M].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_CAPTURE_M].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_CAPTURE_M,Icon_Manager[ICON_CAPTURE_M].tran_color-1);	
	}
	else if(Playmode_flag == CMD_PLAYBACK_MODE)
	{
		gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_PLAYBACK_M].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_PLAYBACK_M].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_PLAYBACK_M,Icon_Manager[ICON_PLAYBACK_M].tran_color-1);	
	}
	else
	{
		return;
	}

	
		menu_select_count = dv_set.menu_select_count;
		if(menu_select_count > DV_SETTING_NUM-1)
		{
			menu_select_count = 0;
			dv_set.menu_select_count = 0;
		}
		if(dv_set.menu_select_count < 0)
		{
			menu_select_count = DV_SETTING_NUM-1;
			dv_set.menu_select_count = DV_SETTING_NUM-1;
		}
		cur_page_num = menu_select_count/PAGE_ITEM_NUM;
		menu_select_count %= PAGE_ITEM_NUM;		
		//printf("DV_SETTING_NUM=%d\n",DV_SETTING_NUM);
		//printf("menu_select_count=%d\n",menu_select_count);
		//printf("cur_page_num=%d\n",cur_page_num);	
	//fill_rectangle(fb,UI_MENU_X_POS+4,UI_MENU_Y_POS+Y_STEP+menu_select_count*Y_STEP,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-10,ICON_HEIGHT,0x001f);
	gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_MENU_S].x_pos*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_MENU_S].y_pos+menu_select_count*Y_STEP)*HDMIxNum,resolution.width, resolution.height,ICON_MENU_S,Icon_Manager[ICON_MENU_S].tran_color-1);	
	sub_str.font_type = 0;	
	sub_str.buff_w = resolution.width;
	sub_str.buff_h = resolution.height;	
	for(int i=0;i<PAGE_ITEM_NUM;i++)
	{	
		if((cur_page_num*PAGE_ITEM_NUM+i) < DV_SETTING_NUM)
		{	
			//show icon
			if(menu_select_count != i)
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].x_pos*HDMIxNum,dv_set.display_y+(UI_MENU_Y_POS+((i+1)%5)*Y_STEP+8)*HDMIxNum/*Icon_Manager[DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].y_pos*/,resolution.width, resolution.height,DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].icon_Index,Icon_Manager[DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].tran_color-1);
			else
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].x_pos*HDMIxNum,dv_set.display_y+(UI_MENU_Y_POS+((i+1)%5)*Y_STEP+8)*HDMIxNum/*Icon_Manager[DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].y_pos*/,resolution.width, resolution.height,DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].icon_Index,0x001f);
	
			if(dv_set.menu_select_flag == 0)
			{
				//show font
				if(menu_select_count == i)
				sub_str.font_color = FONT_SELECT_COLOR;	
				else
				sub_str.font_color = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].color;
				sub_str.pos_x = dv_set.display_x+Icon_Manager[DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].x_pos*HDMIxNum + Icon_Manager[DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].icon_width*HDMIxNum + 8;	
				sub_str.pos_y = dv_set.display_y+(UI_MENU_Y_POS+((i+1)%5)*Y_STEP+8)*HDMIxNum/*Icon_Manager[DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].icon_Index].y_pos*/ + 6;
				if(dv_set.display_mode == SP_DISP_OUTPUT_HDMI)
				{
					sub_str.pos_y += 12;
				}
				sub_str.language = setting_config_get(SET_LANGUAGE);
				sub_str.str_idx = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+i].str_index;
				ap_state_resource_string_draw((INT16U *)fb, &sub_str);		
			}
		}
	}
	
	if(dv_set.menu_select_flag == 0)
	{
		sub_str.font_color = DV_SETTING[0].color;
		sub_str.language = setting_config_get(SET_LANGUAGE);
		sub_str.str_idx = STR_SETUP;
		ap_state_resource_string_resolution_get(&sub_str,&str_res);
		if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
		{		
			sub_str.pos_x = dv_set.display_x+UI_MENU_X_POS + Icon_Manager[ICON_MENU_B].icon_width*HDMIxNum - str_res.string_width - 16;
			sub_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 25 - str_res.string_height/2;
		}
		else
		{
			sub_str.pos_x = dv_set.display_x+1200 - str_res.string_width;
			sub_str.pos_y = dv_set.display_y+60;//  + str_res.string_height/2;			
		}		
		ap_state_resource_string_draw((INT16U *)fb, &sub_str);
		//show page num
		ascii_str.font_color = FONT_COLOR;
		ascii_str.font_type = 0;
		ascii_str.buff_w = resolution.width;
		ascii_str.buff_h = resolution.height;
		if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
		{
			ascii_str.pos_x = dv_set.display_x+UI_MENU_X_POS + 280;
			ascii_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 212;
		}
		else
		{
			ascii_str.pos_x = dv_set.display_x+UI_MENU_X_POS + 1100;
			ascii_str.pos_y = dv_set.display_y + 842;			
		}
		
		page[0] = cur_page_num + 1 + 0x30;
		if((DV_SETTING_NUM%PAGE_ITEM_NUM) != 0)
		page[2] = (DV_SETTING_NUM/PAGE_ITEM_NUM) + 1 + 0x30;
		else 
		page[2] = DV_SETTING_NUM/PAGE_ITEM_NUM + 0x30;
		ascii_str.str_ptr = page;
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);				
	}	
	if(dv_set.menu_select_flag == 1)
	{
		//draw_rectangle(fb,UI_MENU_X_POS+4+ICON_WIDTH+2,UI_MENU_Y_POS+Y_STEP-2,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2,resolution.height-2*UI_MENU_Y_POS-2*Y_STEP+7+2,0x001f,TRANSPARENT_COLOR);
		gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_ITEM_B].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_ITEM_B,Icon_Manager[ICON_ITEM_B].tran_color-1);			
		if(dv_set.menu_select_count == SET_DATA_FORMAT) //date and time setting
		{
			item_total_num = 6;
			item_select_count = dv_set.item_select_count;
			date_type = setting_config_get(dv_set.menu_select_count);

			if((item_select_count > 2)&&(item_select_count < 6))
			{
				//fill_rectangle(fb,DV_SETTING[0].item[1].xPos+(item_select_count-3)*50,DV_SETTING[0].item[1].yPos,resolution.width,resolution.height,32,32,0x001f);
				gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos+(item_select_count-3)*56)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos+38)*HDMIxNum,resolution.width, resolution.height,ICON_DATE_D,Icon_Manager[ICON_DATE_D].tran_color-1);			
				gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos+(item_select_count-3)*56)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos+38-20)*HDMIxNum,resolution.width, resolution.height,ICON_UP,Icon_Manager[ICON_UP].tran_color-1);
				gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos+(item_select_count-3)*56)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos+38+20)*HDMIxNum,resolution.width, resolution.height,ICON_DOWN,Icon_Manager[ICON_DOWN].tran_color-1);
			}
			else if(item_select_count == 6)
			{
				//fill_rectangle(fb,DV_SETTING[0].item[2].xPos,DV_SETTING[0].item[2].yPos,resolution.width,resolution.height,32,32,0x001f);
				gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos+20)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos+38+38)*HDMIxNum,resolution.width, resolution.height,ICON_DATE_D,Icon_Manager[ICON_DATE_D].tran_color-1);			
				gp_Icon_draw(fb,dv_set.display_x+ (Icon_Manager[ICON_DATE_D].x_pos+20)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos+38+38-20)*HDMIxNum,resolution.width, resolution.height,ICON_UP,Icon_Manager[ICON_UP].tran_color-1);
				gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos+20)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos+38+38+20)*HDMIxNum,resolution.width, resolution.height,ICON_DOWN,Icon_Manager[ICON_DOWN].tran_color-1);
			}
			if(date_type == 0)
			{	
				if(item_select_count == 0)
				{
					//fill_rectangle(fb,DV_SETTING[0].item[0].xPos,DV_SETTING[0].item[0].yPos,resolution.width,resolution.height,64,32,0x001f);
					gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_DATE_Y].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_DATE_Y].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_DATE_Y,Icon_Manager[ICON_DATE_Y].tran_color-1);			
					gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_Y].x_pos+16)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_Y].y_pos-20)*HDMIxNum,resolution.width, resolution.height,ICON_UP,Icon_Manager[ICON_UP].tran_color-1);
					gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_Y].x_pos+16)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_Y].y_pos+20)*HDMIxNum,resolution.width, resolution.height,ICON_DOWN,Icon_Manager[ICON_DOWN].tran_color-1);
				}
				else if((item_select_count == 1)||(item_select_count == 2))
				{
					//fill_rectangle(fb,DV_SETTING[0].item[0].xPos+item_select_count*50,DV_SETTING[0].item[0].yPos,resolution.width,resolution.height,32,32,0x001f);
					gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos+14+item_select_count*52)*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_DATE_D].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_DATE_D,Icon_Manager[ICON_DATE_D].tran_color-1);			
					gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos+14+item_select_count*52)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos-20)*HDMIxNum,resolution.width, resolution.height,ICON_UP,Icon_Manager[ICON_UP].tran_color-1);
					gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos+14+item_select_count*52)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos+20)*HDMIxNum,resolution.width, resolution.height,ICON_DOWN,Icon_Manager[ICON_DOWN].tran_color-1);
				}
			}
			else if((date_type == 1)||(date_type==2))
			{
				if(item_select_count == 2)
				{
					//fill_rectangle(fb,DV_SETTING[0].item[0].xPos+100,DV_SETTING[0].item[0].yPos,resolution.width,resolution.height,64,32,0x001f);
					gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_Y].x_pos+110)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_Y].y_pos)*HDMIxNum,resolution.width, resolution.height,ICON_DATE_Y,Icon_Manager[ICON_DATE_Y].tran_color-1);			
					gp_Icon_draw(fb,dv_set.display_x+ (Icon_Manager[ICON_DATE_Y].x_pos+110+16)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_Y].y_pos-20)*HDMIxNum,resolution.width, resolution.height,ICON_UP,Icon_Manager[ICON_UP].tran_color-1);
					gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_Y].x_pos+110+16)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_Y].y_pos+20)*HDMIxNum,resolution.width, resolution.height,ICON_DOWN,Icon_Manager[ICON_DOWN].tran_color-1);
				}
				else if((item_select_count == 0)||(item_select_count == 1))
				{
					//fill_rectangle(fb,DV_SETTING[0].item[0].xPos+item_select_count*50,DV_SETTING[0].item[0].yPos,resolution.width,resolution.height,32,32,0x001f);
					gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos-14+item_select_count*56)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos)*HDMIxNum,resolution.width, resolution.height,ICON_DATE_D,Icon_Manager[ICON_DATE_D].tran_color-1);			
					gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos-14+item_select_count*56)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos-20)*HDMIxNum,resolution.width, resolution.height,ICON_UP,Icon_Manager[ICON_UP].tran_color-1);
					gp_Icon_draw(fb, dv_set.display_x+(Icon_Manager[ICON_DATE_D].x_pos-14+item_select_count*56)*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_DATE_D].y_pos+20)*HDMIxNum,resolution.width, resolution.height,ICON_DOWN,Icon_Manager[ICON_DOWN].tran_color-1);
				}				
			}


			//printf("date_type=%d\n",date_type);	
			//printf("item_select_count=%d\n",item_select_count);	
			//printf("date_str[date_type]=%s\n",date_str[date_type]);	
			//printf("date1_str[date_type]=%s\n",date1_str[date_type]);	
			for(int i=0;i<6;i++)
			{
				printf("setup_date_time[%d]=%d\n",i,setup_date_time[i]);
			}
			
			ascii_str.font_color = FONT_COLOR;
			ascii_str.font_type = 0;
			ascii_str.buff_w = resolution.width;
			ascii_str.buff_h = resolution.height;

			//date
			if(item_select_count == 0)
			ascii_str.font_color = FONT_SELECT_COLOR;	
			else
			ascii_str.font_color = FONT_COLOR;	
			set_date_time_string_process(&date_str[date_type], 0,date_type);
			ap_state_resource_string_ascii_resolution_get(&date_str[date_type],&str_res);
			ascii_str.str_ptr = date_str[date_type];
			old_str_len = str_res.string_width;
			ascii_str.pos_x = time_x_pos = dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum + Icon_Manager[ICON_ITEM_B].icon_width*HDMIxNum/2 - str_res.string_width/2;
			ascii_str.pos_y = dv_set.display_y+Icon_Manager[ICON_MENU_B].y_pos*HDMIxNum +  76*HDMIxNum;		
			ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);	
			if(item_select_count == 0)
			{
				if(date_type == 0)
				{
					ap_state_resource_string_ascii_resolution_get(&date_str[date_type][4],&str_res);
					ascii_str.str_ptr = &date_str[date_type][4];
				}
				else
				{
					ap_state_resource_string_ascii_resolution_get(&date_str[date_type][2],&str_res);
					ascii_str.str_ptr = &date_str[date_type][2];					
				}
				ascii_str.font_color = FONT_COLOR;
				ascii_str.pos_x = time_x_pos + old_str_len -str_res.string_width;	
				ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
			}
			else if(item_select_count == 1)
			{
				if(date_type == 0)
				{
					ap_state_resource_string_ascii_resolution_get(&date_str[date_type][7],&str_res);
					ascii_str.str_ptr = &date_str[date_type][7];
				}
				else
				{
					ap_state_resource_string_ascii_resolution_get(&date_str[date_type][5],&str_res);
					ascii_str.str_ptr = &date_str[date_type][5];					
				}
				ascii_str.font_color = FONT_SELECT_COLOR;
				ascii_str.pos_x = time_x_pos + old_str_len -str_res.string_width;	
				ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);	
				if(date_type == 0)
				{
					ap_state_resource_string_ascii_resolution_get(&date_str[date_type][9],&str_res);
					ascii_str.str_ptr = &date_str[date_type][9];
				}
				else
				{
					ap_state_resource_string_ascii_resolution_get(&date_str[date_type][7],&str_res);
					ascii_str.str_ptr = &date_str[date_type][7];					
				}
				ascii_str.font_color = FONT_COLOR;
				ascii_str.pos_x = time_x_pos + old_str_len -str_res.string_width;	
				ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);					
			}
			else if(item_select_count == 2)
			{
				if(date_type == 0)
				{
					ap_state_resource_string_ascii_resolution_get(&date_str[date_type][12],&str_res);
					ascii_str.str_ptr = &date_str[date_type][12];
				}
				else
				{
					ap_state_resource_string_ascii_resolution_get(&date_str[date_type][10],&str_res);
					ascii_str.str_ptr = &date_str[date_type][10];					
				}
				ascii_str.font_color = FONT_SELECT_COLOR;
				ascii_str.pos_x = time_x_pos + old_str_len -str_res.string_width;	
				ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
			}			

			//time

			if(item_select_count == 3)
			ascii_str.font_color = FONT_SELECT_COLOR;	
			else
			ascii_str.font_color = FONT_COLOR;	
		 	set_date_time_string_process(time_str, 1,date_type);
			ascii_str.str_ptr = time_str;			
			ap_state_resource_string_ascii_resolution_get(&time_str,&str_res);
			old_str_len = str_res.string_width;
			ascii_str.pos_x = time_x_pos = dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum + Icon_Manager[ICON_ITEM_B].icon_width*HDMIxNum/2 - str_res.string_width/2;
			ascii_str.pos_y = dv_set.display_y+Icon_Manager[ICON_MENU_B].y_pos*HDMIxNum +  114*HDMIxNum;
			ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
			if(item_select_count == 3) //hour
			{
				ap_state_resource_string_ascii_resolution_get(&time_str[2],&str_res);
				ascii_str.font_color = FONT_COLOR;
				ascii_str.str_ptr = &time_str[2];
				ascii_str.pos_x = time_x_pos + old_str_len -str_res.string_width;	
				ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
			}
			else if(item_select_count == 4)//min
			{
				ascii_str.font_color = FONT_SELECT_COLOR;
				ap_state_resource_string_ascii_resolution_get(&time_str[5],&str_res);
				ascii_str.str_ptr = &time_str[5];
				ascii_str.pos_x = time_x_pos + old_str_len -str_res.string_width;	
				ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
				ascii_str.font_color = FONT_COLOR;
				ap_state_resource_string_ascii_resolution_get(&time_str[7],&str_res);
				ascii_str.str_ptr = &time_str[7];
				ascii_str.pos_x = time_x_pos + old_str_len -str_res.string_width;	
				ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);				
			}else if(item_select_count == 5)
			{
				ascii_str.font_color = FONT_SELECT_COLOR;
				ap_state_resource_string_ascii_resolution_get(&time_str[10],&str_res);
				ascii_str.str_ptr = &time_str[10];
				ascii_str.pos_x = time_x_pos + old_str_len -str_res.string_width;	
				ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);				
			}
			//type
			if(item_select_count == 6)
			ascii_str.font_color = FONT_SELECT_COLOR;
			else 
			ascii_str.font_color = FONT_COLOR;	
			ap_state_resource_string_ascii_resolution_get(&date1_str[date_type],&str_res);
			ascii_str.str_ptr = date1_str[date_type];
			old_str_len = str_res.string_width;
			ascii_str.pos_x  = time_x_pos = dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum + Icon_Manager[ICON_ITEM_B].icon_width*HDMIxNum/2 - str_res.string_width/2;
			ascii_str.pos_y = dv_set.display_y+Icon_Manager[ICON_MENU_B].y_pos*HDMIxNum +  152*HDMIxNum;	
			ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
			if(item_select_count == 6)
			{
				ascii_str.font_color = FONT_COLOR;	
				ap_state_resource_string_ascii_resolution_get(&date1_str[date_type][2],&str_res);
				ascii_str.str_ptr = &date1_str[date_type][2];
				ascii_str.pos_x =time_x_pos + old_str_len -str_res.string_width;	
				ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);					
			}
			
		}
		else if(((dv_set.menu_select_count >= SET_SOUND)&&(dv_set.menu_select_count <= SET_FORMAT))||(dv_set.menu_select_count == SET_GSENSOR1))
		{
			item_total_num = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item_num;
			item_select_count = dv_set.item_select_count;//DV_config_get(cur_page_num*PAGE_ITEM_NUM+menu_select_count);
			
			if(item_select_count > item_total_num-1)
			{
				item_select_count = 0;
				dv_set.item_select_count = 0;
			}
			if(dv_set.item_select_count < 0)
			{
				item_select_count = item_total_num-1;
				dv_set.item_select_count = item_total_num-1;
			}
			item_page_num = item_select_count/PAGE_ITEM_NUM;
			item_select_count %= PAGE_ITEM_NUM;
			//printf("item_total_num=%d\n",item_total_num);
			//printf("item_select_count=%d\n",item_select_count);
			//printf("item_page_num=%d\n",item_page_num);
			//fill_rectangle(fb,UI_MENU_X_POS+4+ICON_WIDTH+2+15,UI_MENU_Y_POS+Y_STEP+item_select_count*Y_STEP,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2-30,ICON_HEIGHT,0x001f);
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_ITEM_S].x_pos*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_ITEM_S].y_pos+item_select_count*(Y_STEP-2))*HDMIxNum,resolution.width, resolution.height,ICON_ITEM_S,Icon_Manager[ICON_ITEM_S].tran_color-1);	
			for(int i=0;i<PAGE_ITEM_NUM;i++)
			{
				if((item_page_num*PAGE_ITEM_NUM+i)<item_total_num)					
				{
					//show font
					if(item_select_count == i)
					sub_str.font_color = FONT_SELECT_COLOR;
					else
					sub_str.font_color = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;
					if((dv_set.menu_select_count != SET_LANGUAGE))
					sub_str.language = setting_config_get(SET_LANGUAGE);
					else
					{
						sub_str.language = item_page_num*PAGE_ITEM_NUM+i;
						
					}
					sub_str.str_idx = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[item_page_num*PAGE_ITEM_NUM+i].str_index;
					ap_state_resource_string_resolution_get(&sub_str,&str_res);
					//sub_str.pos_x = UI_MENU_X_POS+4+ICON_WIDTH+2 + (resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2)/2- str_res.string_width/2;		
					//sub_str.pos_y = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[item_page_num*PAGE_ITEM_NUM+i].yPos + 6;	
					sub_str.pos_x = dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum + Icon_Manager[ICON_ITEM_B].icon_width*HDMIxNum/2 - str_res.string_width/2;
					sub_str.pos_y = dv_set.display_y+Icon_Manager[ICON_ITEM_S].y_pos*HDMIxNum + Icon_Manager[ICON_ITEM_S].icon_height*HDMIxNum/2 - str_res.string_height/2 + i*38*HDMIxNum;					
					ap_state_resource_string_draw((INT16U *)fb, &sub_str);				
				}
			}
		}

		sub_str.font_color = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;
		sub_str.language = setting_config_get(SET_LANGUAGE);
		sub_str.str_idx = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].str_index;
		ap_state_resource_string_resolution_get(&sub_str,&str_res);
		if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
		{		
			sub_str.pos_x = dv_set.display_x+UI_MENU_X_POS + Icon_Manager[ICON_MENU_B].icon_width*HDMIxNum - str_res.string_width - 16;
			sub_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 25 - str_res.string_height/2;
		}
		else
		{
			sub_str.pos_x = dv_set.display_x+1200 - str_res.string_width;
			sub_str.pos_y = dv_set.display_y+60;//  + str_res.string_height/2;			
		}		
		ap_state_resource_string_draw((INT16U *)fb, &sub_str);	
		
	}
	else if(dv_set.menu_select_flag == 2)
	{
			//draw_rectangle(fb,UI_MENU_X_POS+4+ICON_WIDTH+2,UI_MENU_Y_POS+Y_STEP-2,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2,resolution.height-2*UI_MENU_Y_POS-2*Y_STEP+7+2,0x001f,TRANSPARENT_COLOR);
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_ITEM_B].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_ITEM_B,Icon_Manager[ICON_ITEM_B].tran_color-1);	
			sub_str.font_color = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;
			sub_str.language = setting_config_get(SET_LANGUAGE);
			sub_str.str_idx = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].str_index;
			ap_state_resource_string_resolution_get(&sub_str,&str_res);
			if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
			{		
				sub_str.pos_x = dv_set.display_x+UI_MENU_X_POS + Icon_Manager[ICON_MENU_B].icon_width*HDMIxNum - str_res.string_width - 16;
				sub_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 25 - str_res.string_height/2;
			}
			else
			{
				sub_str.pos_x = dv_set.display_x+1200 - str_res.string_width;
				sub_str.pos_y = dv_set.display_y+60;//  + str_res.string_height/2;			
			}				
			ap_state_resource_string_draw((INT16U *)fb, &sub_str);	

			if(dv_set.item_select_count > 3)
				dv_set.item_select_count = 2;
			else if(dv_set.item_select_count < 2)
				dv_set.item_select_count = 3;
			//fill_rectangle(fb,UI_MENU_X_POS+4+ICON_WIDTH+2+15,UI_MENU_Y_POS+Y_STEP+dv_set.item_select_count*Y_STEP,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2-30,ICON_HEIGHT,0x001f);
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_ITEM_S].x_pos*HDMIxNum,dv_set.display_y+(Icon_Manager[ICON_ITEM_S].y_pos+dv_set.item_select_count*(Y_STEP-2))*HDMIxNum,resolution.width, resolution.height,ICON_ITEM_S,Icon_Manager[ICON_ITEM_S].tran_color-1);	
			if(dv_set.menu_select_count == SET_FORMAT)
			{
				item_num_temp = 4;
			}
			else
			{
				item_num_temp = 3;
			}
			
			for(int i=0;i<item_num_temp;i++)
			{
				//show font
				if(dv_set.item_select_count == (i+1))
				sub_str.font_color = FONT_SELECT_COLOR;
				else
				sub_str.font_color = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;
				sub_str.language = setting_config_get(SET_LANGUAGE);
				sub_str.str_idx = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[1+i].str_index;
				ap_state_resource_string_resolution_get(&sub_str,&str_res);
				//sub_str.pos_x = UI_MENU_X_POS+4+ICON_WIDTH+2 + (resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2)/2- str_res.string_width/2;		
				//sub_str.pos_y = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[i+1].yPos + 6;		
				sub_str.pos_x = dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum + Icon_Manager[ICON_ITEM_B].icon_width*HDMIxNum/2 - str_res.string_width/2;
				if(i == 3)
				sub_str.pos_y = dv_set.display_y+Icon_Manager[ICON_ITEM_S].y_pos*HDMIxNum + Icon_Manager[ICON_ITEM_S].icon_height*HDMIxNum/2 - str_res.string_height/2+26*HDMIxNum ;	
				else if(i > 0)
				sub_str.pos_y = dv_set.display_y+Icon_Manager[ICON_ITEM_S].y_pos*HDMIxNum + Icon_Manager[ICON_ITEM_S].icon_height*HDMIxNum/2 - str_res.string_height/2 + (i+1)*38*HDMIxNum;
				else
				sub_str.pos_y = dv_set.display_y+Icon_Manager[ICON_ITEM_S].y_pos*HDMIxNum + Icon_Manager[ICON_ITEM_S].icon_height*HDMIxNum/2 - str_res.string_height/2;					
				ap_state_resource_string_draw((INT16U *)fb, &sub_str);				
			}	
	}

	else if(dv_set.menu_select_flag == 3)
	{
			//draw_rectangle(fb,UI_MENU_X_POS+4+ICON_WIDTH+2,UI_MENU_Y_POS+Y_STEP-2,resolution.width,resolution.height,resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2,resolution.height-2*UI_MENU_Y_POS-2*Y_STEP+7+2,0x001f,TRANSPARENT_COLOR);
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_ITEM_B].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_ITEM_B,Icon_Manager[ICON_ITEM_B].tran_color-1);
			sub_str.font_color = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;
			sub_str.language = setting_config_get(SET_LANGUAGE);
			sub_str.str_idx = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].str_index;
			ap_state_resource_string_resolution_get(&sub_str,&str_res);
			if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
			{		
				sub_str.pos_x = dv_set.display_x+UI_MENU_X_POS + Icon_Manager[ICON_MENU_B].icon_width*HDMIxNum - str_res.string_width - 16;
				sub_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 25 - str_res.string_height/2;
			}
			else
			{
				sub_str.pos_x = dv_set.display_x+1200 - str_res.string_width;
				sub_str.pos_y = dv_set.display_y+60;//  + str_res.string_height/2;			
			}					
			ap_state_resource_string_draw((INT16U *)fb, &sub_str);	
			
				//show font
			#if 0
			sub_str.font_color = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].color;

			sub_str.language = setting_config_get(SET_LANGUAGE);
			sub_str.str_idx = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[0].str_index;
			ap_state_resource_string_resolution_get(&sub_str,&str_res);
			//sub_str.pos_x = UI_MENU_X_POS+4+ICON_WIDTH+2 + (resolution.width-2*UI_MENU_X_POS-4-ICON_WIDTH-2)/2- str_res.string_width/2;		
			//sub_str.pos_y = DV_SETTING[cur_page_num*PAGE_ITEM_NUM+menu_select_count].item[0].yPos + 6;			
			sub_str.pos_x = dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos + Icon_Manager[ICON_ITEM_B].icon_width/2 - str_res.string_width/2;
			sub_str.pos_y = dv_set.display_y+Icon_Manager[ICON_ITEM_B].y_pos + Icon_Manager[ICON_ITEM_B].icon_height/2 - str_res.string_height/2;	
			ap_state_resource_string_draw((INT16U *)fb, &sub_str);				
			#else
			#if 1
			Get_cmputer_build_time();
			#endif
			ascii_str.font_color = FONT_COLOR;
			ascii_str.font_type = 0;
			ascii_str.buff_w = resolution.width;
			ascii_str.buff_h = resolution.height;	
			ascii_str.str_ptr = Global_User_Optins.item.version;
			ap_state_resource_string_ascii_resolution_get(Global_User_Optins.item.version,&str_res);
			ascii_str.pos_x = dv_set.display_x+Icon_Manager[ICON_ITEM_B].x_pos*HDMIxNum + Icon_Manager[ICON_ITEM_B].icon_width*HDMIxNum/2 - str_res.string_width/2;;
			ascii_str.pos_y = dv_set.display_y+Icon_Manager[ICON_ITEM_B].y_pos*HDMIxNum + Icon_Manager[ICON_ITEM_B].icon_height*HDMIxNum/2 - str_res.string_height/2;	;		
			ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
			#endif
	}
	
	if(dv_set.sd_check == 3)
	{
		gp_Icon_draw(fb, (resolution.width-Icon_Manager[ICON_NOFILE].icon_width*HDMIxNum)/2,(resolution.height-Icon_Manager[ICON_NOFILE].icon_height*HDMIxNum)/2,resolution.width, resolution.height,ICON_NOFILE,Icon_Manager[ICON_NOFILE].tran_color);
		sub_str.font_type = 0;	
		sub_str.font_color = FONT_COLOR;
		sub_str.buff_w = resolution.width;
		sub_str.buff_h = resolution.height;	
		sub_str.language = setting_config_get(SET_LANGUAGE);
		sub_str.str_idx = STR_PLEASE_WAIT;		
		ap_state_resource_string_resolution_get(&sub_str,&str_res);
		sub_str.pos_x = (resolution.width -str_res.string_width) /2;
		sub_str.pos_y = (resolution.height -str_res.string_height) /2;
		ap_state_resource_string_draw((INT16U *)fb, &sub_str);			
	}

	
	dispFlip(hDisp);
	
}

int
USB_task_entry(
	int argc,
	char **argv
)
{
	STRING_INFO sub_str;
	t_STRING_TABLE_STRUCT str_res;
	gp_size_t resolution;
	UINT16 *fb;
	UINT16* addr1;	
	menuManager_t *p_menu;
	int i;
	int ret;
	UINT8 msg_id,msg_prio;
	time_t t;
	int mode;
	printf("USB_task_entry start\n");
	if(dv_set.change_disp != 0)
	{
		printf("dv_set.change_disp =%d\n",dv_set.change_disp);
		return;	
	}
	p_menu = &DV_MENU[0];
	dv_set.ui_show = 1;
	dv_set.dv_UI_flag = DV_FOREGROUND_MODE;
	msg_id = dv_set.dv_UI_flag;
	ret = mq_send(menumodeMsgQ, &msg_id, 1, 0);	
	printf("dv_set.upgrade_flag = %d\n",dv_set.upgrade_flag);
	if(dv_set.upgrade_flag == 2)
	{
		dv_set.upgrade_flag = 0;
		ret = CheckMemoryPath();
		if(ret == 0)
		{
			dv_set.upgrade_flag = SPI_Flash_update();
		}
		
	}
	printf("dv_set.upgrade_flag = %d\n",dv_set.upgrade_flag);
	while(1)
	{
		ret = mq_receive(menumodeMsgQ, (char *)&msg_id, 1, (UINT32 *)&msg_prio);
		dispGetResolution(hDisp, &resolution);	
		addr1 = fb = (UINT16*)dispGetFramebuffer(hDisp);
		if(dv_set.dv_UI_flag == DV_EXIT_MODE)
			break;
		dv_set.ui_show = 1;
		if(dv_set.menu_select_flag == 1)
		{
			for (int y = 0; y <resolution.height; y++) {		
				for (int x = 0; x < resolution.width; x++) {			
					*addr1++ = FONT_SELECT_COLOR;	
				}	
			}
			sub_str.font_type = 0;	
			sub_str.font_color = FONT_COLOR;
			sub_str.buff_w = resolution.width;
			sub_str.buff_h = resolution.height;	
			sub_str.language = setting_config_get(SET_LANGUAGE);
			if(dv_set.menu_select_count == 0)
			sub_str.str_idx = STR_MASS_STORAGE;
			else
			sub_str.str_idx = STR_PC_CAMERA;	
			ap_state_resource_string_resolution_get(&sub_str,&str_res);
			sub_str.pos_x = (resolution.width -str_res.string_width) /2;
			sub_str.pos_y = (resolution.height -str_res.string_height) /2;
			ap_state_resource_string_draw((INT16U *)fb, &sub_str);				

			dispFlip(hDisp);
			if(dv_set.display_mode == SP_DISP_OUTPUT_LCD)
			disp_pipemsg_send1(DISP_BUF_FREE,4, &mode);
		}
		else
		{
		
			for (int y = 0; y <resolution.height; y++) {		
				for (int x = 0; x < resolution.width; x++) {			
					*addr1++ = TRANSPARENT_COLOR;		
				}	
			}	

		if(dv_set.display_mode == SP_DISP_OUTPUT_LCD)
		{
			dv_set.display_x = DISP_LCD_X_OFFSET;
			dv_set.display_y = DISP_LCD_Y_OFFSET;		
		}
		else if(dv_set.display_mode == SP_DISP_OUTPUT_TV)
		{
			dv_set.display_x = DISP_TV_X_OFFSET;
			dv_set.display_y = DISP_TV_Y_OFFSET;
		}
		else if(dv_set.display_mode == SP_DISP_OUTPUT_HDMI)
		{
			dv_set.display_x = DISP_HDMI_X_OFFSET;
			dv_set.display_y = DISP_HDMI_Y_OFFSET;		
		}
		else
		{
			dv_set.display_x = 0;
			dv_set.display_y = 0;		
		}			
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_MENU_B].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_MENU_B].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_MENU_B,Icon_Manager[ICON_MENU_B].tran_color-1);
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_TOP_S].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_TOP_S].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_TOP_S,Icon_Manager[ICON_TOP_S].tran_color-1);
			gp_Icon_draw(fb, dv_set.display_x+ Icon_Manager[ICON_VIDEO_M].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_VIDEO_M].y_pos*HDMIxNum,resolution.width, resolution.height,ICON_SETUP,Icon_Manager[ICON_SETUP].tran_color-1);
			sub_str.font_type = 0;	
			sub_str.font_color = FONT_COLOR;
			sub_str.buff_w = resolution.width;
			sub_str.buff_h = resolution.height;	
			sub_str.language = setting_config_get(SET_LANGUAGE);
			sub_str.str_idx = STR_USB;	
			ap_state_resource_string_resolution_get(&sub_str,&str_res);
			if(dv_set.display_mode != SP_DISP_OUTPUT_HDMI)
			{		
				sub_str.pos_x = dv_set.display_x+UI_MENU_X_POS + Icon_Manager[ICON_MENU_B].icon_width*HDMIxNum - str_res.string_width - 16;
				sub_str.pos_y = dv_set.display_y+UI_MENU_Y_POS + 25 - str_res.string_height/2;
			}
			else
			{
				sub_str.pos_x = dv_set.display_x+1200 - str_res.string_width;
				sub_str.pos_y = dv_set.display_y+60;//  + str_res.string_height/2;			
			}
			ap_state_resource_string_draw((INT16U *)fb, &sub_str);	

			printf("dv_set.menu_select_count = %d\n",dv_set.menu_select_count);
			
			if(dv_set.menu_select_count > 1)
			{
				dv_set.menu_select_count = 0;
			}
			if(dv_set.menu_select_count < 0)
			{
				dv_set.menu_select_count = 1;
			}
			gp_Icon_draw(fb, dv_set.display_x+Icon_Manager[ICON_MENU_S].x_pos*HDMIxNum,dv_set.display_y+Icon_Manager[ICON_MENU_S].y_pos*HDMIxNum+dv_set.menu_select_count*Y_STEP*HDMIxNum,resolution.width, resolution.height,ICON_MENU_S,Icon_Manager[ICON_MENU_S].tran_color-1);	

			for(i=0;i<2;i++)
			{
				if(dv_set.menu_select_count == i)	
					sub_str.font_color = FONT_SELECT_COLOR;
				else				
					sub_str.font_color = p_menu[i].color;
				sub_str.pos_x = dv_set.display_x+(Icon_Manager[p_menu[i].icon_Index].x_pos + Icon_Manager[p_menu[i].icon_Index].icon_width + 8)*HDMIxNum;	
				sub_str.pos_y = dv_set.display_y+(Icon_Manager[p_menu[i].icon_Index].y_pos + 6)*HDMIxNum;
				sub_str.language = setting_config_get(SET_LANGUAGE);
				if(i==0)
				sub_str.str_idx = STR_MASS_STORAGE;
				else
				sub_str.str_idx = STR_PC_CAMERA;	
				ap_state_resource_string_draw((INT16U *)fb, &sub_str);	
			}
			dispFlip(hDisp);
			if(dv_set.display_mode == SP_DISP_OUTPUT_LCD)
			{
				disp_pipemsg_send1(DISP_BUF_FREE,4, &mode);
			}
		}
	}	
}
void USB_entry_init(void)
{
	UINT8 msg_id;
	Playmode_flag =  CMD_USB_MODE;
	dv_set.dv_UI_flag = DV_EXIT_MODE;
	dv_set.menu_select_flag = 0;
	dv_set.menu_select_count = 0;
	if(dv_set.dv_ctrl == 1)
	{
		CVR_Set_PIPE_With_CMD(CMD_STOP_DV,1);
		dv_set.dv_ctrl =0;				
	}
	else if(dv_set.dc_ctrl == 1)
	{
		dv_set.dc_ctrl = 0;
	}
	msg_id = dv_set.dv_UI_flag;
	mq_send(menumodeMsgQ, &msg_id, 1, 0);
}

void Power_off(void)
{
	UINT8 msg_id;
	Playmode_flag =  CMD_POWEROFF_MODE;
	dv_set.dv_UI_flag = DV_EXIT_MODE;
	dv_set.menu_select_flag = 0;
	dv_set.menu_select_count = 0;
	if(dv_set.dv_ctrl == 1)
	{
		CVR_Set_PIPE_With_CMD(CMD_STOP_DV,1);
		dv_set.dv_ctrl =0;				
	}
	else if(dv_set.dc_ctrl == 1)
	{
		dv_set.dc_ctrl = 0;
	}
	msg_id = dv_set.dv_UI_flag;
	mq_send(menumodeMsgQ, &msg_id, 1, 0);
	//DV_power_off();
}
extern int pllhd; 
void PowerOff_entry(void)
{
	int fd;
	gp_size_t resolution;
	UINT16 *fb;
	UINT16* addr1;	
	gp_bitmap_t fb1;
	int ret;
	dv_set.ui_show = 1;
	//show power off UI
	dispGetResolution(hDisp, &resolution);	
	addr1 = fb = (UINT16*)dispGetFramebuffer(hDisp);
	strcpy(file_path, Icon_path);
	strcat(file_path, "POWER_OFF_LOGO.JPG");
	printf("open file %s\n",file_path);
	fb1.pData = fb;
	fb1.validRect.x = 0;
	fb1.validRect.y = 0;
	fb1.validRect.width = resolution.width;
	fb1.validRect.height=resolution.height;
	Gdjpegdecode(file_path, &fb1, resolution.width,resolution.height,&resolution, 0);
	dispFlip(hDisp);

	Play_Key_sound(SOUND_POWER_OFF);
	ap_state_config_store();
	sleep(2);
	while(1)
	{
		if(dv_set.power_off == 1)
		{
			printf("wait sound end or pic show  \n");
			if(setting_config_get(SET_GSENSOR1)!=0)
			{
				Gsensor_parking_mode(setting_config_get(SET_GSENSOR1));
				Gsensor_parking_mode_int_read();
			}
			break;
		}
		usleep(10000);
	}
	//if(dv_set.usb_detect != 1)
	Power_Off_Set(); 
	LED_Set(NORMAL_LED,0);
	Enable_Sensor_Power(0);
	//USB plug in
	LCD_Backlight_Set(0);
	LED_Set(IR_LED,0);
	LED_Set(MUTE_ON,0);	
	dispDestroy(hDisp);
	PLL_ctl_en(pllhd, 1, 0);
	dv_set.power_off = 2;
	while(1)
	{

		Power_Off_Set(); 
		printf("have power off, release power key!\n");
		if(power_off == 3||power_off == 0)
		{
			printf("sleep 2s!\n");
			sleep(2);
			printf("~~~~~~~start watchdog\n");
			printf("power on!\n");			
			fd = open("/dev/watchdog",0);
			ioctl(fd,WDIOC_SETTIMEROUT,0);	//set timeout 0s		
			ioctl(fd,WDIOC_CTRL,1);		//enable watchdog	
			close(fd);
		}
		usleep(100);
	}
}

void UPgrade_UI(UINT8 step,int value)
{
	STRING_ASCII_INFO ascii_str;
	t_STRING_TABLE_STRUCT str_res;
	gp_size_t resolution;
	UINT16 *fb;
	UINT16* addr1;	
	int i;
	int ret;
	char buffer[]="000%";
	dispGetResolution(hDisp, &resolution);	
	addr1 = fb = (UINT16*)dispGetFramebuffer(hDisp);
	ascii_str.font_color = FONT_COLOR;
	ascii_str.font_type = 0;
	ascii_str.buff_w = resolution.width;
	ascii_str.buff_h = resolution.height;	
	if(step == 0)
	{
		for (int y = 0; y <resolution.height; y++) {		
			for (int x = 0; x < resolution.width; x++) {			
				*addr1++ = 0x0000;	
			}	
		}
		ascii_str.str_ptr = "========================";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 10;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);		
		ascii_str.str_ptr = "Upgrading firmware...";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 20;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);	
		ascii_str.str_ptr = "Old Version:";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 40;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
		ascii_str.font_color = 0x07e0;
		ascii_str.str_ptr = Global_User_Optins.item.version;
		ascii_str.pos_x = 160;
		ascii_str.pos_y = 40;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
		ascii_str.font_color = 0xf800;
		ascii_str.str_ptr = "Do not Power off now";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 60;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);				
	}
	else if(step == 1)
	{
		ascii_str.str_ptr = "Step 1:";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 80;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);	
		ascii_str.str_ptr = "Erase Flash...:";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 100;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
	}
	else if(step == 2)
	{
		ascii_str.font_color = 0x07e0;
		ascii_str.str_ptr = "OK";
		ascii_str.pos_x = 180;
		ascii_str.pos_y = 100;	
		fill_rectangle(fb, ascii_str.pos_x, ascii_str.pos_y,resolution.width, resolution.height,60, 20,0x0000);
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);	
		ascii_str.font_color = FONT_COLOR;
		ascii_str.str_ptr = "Step 2:";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 120;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);	
		ascii_str.str_ptr = "write code to Flash:";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 140;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);	
	}
	else if(step == 3)
	{
		if(value !=100)
		{
			buffer[2] = value%10+0x30;
			buffer[1] = value/10 +0x30;
			buffer[0] = 0x30;
		}
		else
		{
			buffer[2] = 0x30;
			buffer[1] =  0x30;
			buffer[0] = 1+0x30;			
		}
		if(value<10)
		ascii_str.str_ptr = &buffer[2] ;
		else if(value ==100)
		ascii_str.str_ptr = &buffer[0] ;	
		else
		ascii_str.str_ptr = &buffer[1] ;		
		ascii_str.pos_x = 240;
		ascii_str.pos_y = 140;	
		fill_rectangle(fb, ascii_str.pos_x, ascii_str.pos_y,resolution.width, resolution.height,60, 20,0x0000);
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);		
	}
	else if(step == 4)
	{
		ascii_str.font_color = 0x07e0;
		buffer[2] = 0x30;
		buffer[1] =  0x30;
		buffer[0] = 1+0x30;			
		ascii_str.str_ptr = buffer ;
		ascii_str.pos_x = 240;
		ascii_str.pos_y = 140;	
		fill_rectangle(fb, ascii_str.pos_x, ascii_str.pos_y,resolution.width, resolution.height,60, 20,0x0000);
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
		ascii_str.font_color = FONT_COLOR;
		ascii_str.str_ptr = "Step 3:";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 160;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
		ascii_str.font_color = 0x07e0;
		ascii_str.str_ptr = "Upgrading finish";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 180;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
		ascii_str.font_color = 0xf800;
		ascii_str.str_ptr = "Please Remove SD card";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 200;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);
		ascii_str.font_color = FONT_COLOR;
		ascii_str.str_ptr = "========================";
		ascii_str.pos_x = 15;
		ascii_str.pos_y = 212;		
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);			
	}
	else if(step == 5)
	{
		if(value == 0)
		ascii_str.str_ptr = "*";
		else if(value == 1)
		ascii_str.str_ptr = "**";	
		else if(value == 2)
		ascii_str.str_ptr = "***";	
		else if(value == 3)
		ascii_str.str_ptr = "****";	
		else if(value == 4)
		ascii_str.str_ptr = "*****";		
		else
		{
			ascii_str.str_ptr = "*****";	
		}
		ascii_str.pos_x = 180;
		ascii_str.pos_y = 100;	
		fill_rectangle(fb, ascii_str.pos_x, ascii_str.pos_y,resolution.width, resolution.height,100, 20,0x0000);
		ap_state_resource_string_ascii_draw((UINT16 *)fb, &ascii_str);	
	}	
	dispFlip(hDisp);
}

