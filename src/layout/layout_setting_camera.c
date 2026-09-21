#include "layout_define.h"
#include "leo_api.h"
#include "../api/common/cctv_stream_policy.h"

typedef enum camera_module_list
{
	SWITCH_MODULE,
	MODEL_MODULE,
	STREAM_MODULE,
	IP_MODULE,
	ACCOUNT_MODULE,
	PASSWORD_MODULE,
	TOTAL_MODULE
}camera_module_list;

#define CAMERA_MODULE_COORDINATE_INIT  {\
			{199,75,700,52},\
			{199,127,700,52},\
			{199,179,700,52},\
			{199,231,700,52},\
			{199,285,700,52},\
			{199,339,700,52},\
	};

static bool set_camera_flag = 0;
extern char input_data[16];
extern int get_pwd_str;

static void ccamera_setting_display(void);

static void cctv_diag_camera_config(const char *stage, int camera_index, const camera_info *camera, int stream_type)
{
	const char *ip = camera->ip[0] != '\0' ? camera->ip : "<empty>";
	unsigned int account_len = (unsigned int)strlen(camera->account);
	unsigned int password_len = (unsigned int)strlen(camera->pwd);
	int invalid = camera->ip[0] == '\0' || camera->ip[0] == '0' ||
		strcmp(camera->ip, "0.0.0.0") == 0 || account_len == 0 || password_len == 0;

	Debug("[CCTV_DIAG] %s camera%d ip=%s account_len=%u password_len=%u model=%d stream=%d url_present=%d invalid=%d\n",
		stage, camera_index, ip, account_len, password_len, camera->model, stream_type,
		camera->url[0] == 'r', invalid);
}



static void set_camera1_flag_up(lv_obj_t * obj)
{
	if(set_camera_flag != 0){
		set_camera_flag = 0;
		lv_obj_clean(lv_scr_act());
		ccamera_setting_display();
	}
}
static void set_camera2_flag_up(lv_obj_t * obj)
{
	if(set_camera_flag != 1){
		set_camera_flag = 1;
		lv_obj_clean(lv_scr_act());
		ccamera_setting_display();
	
	}
}




static void camera_setting_btn_text_create(void){
	lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);
   	lv_obj_set_pos(btn, 58, 162);
    lv_obj_set_size(btn, 66, 66);
    lv_obj_set_style_local_bg_opa(btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_bg_opa(btn, LV_LABEL_PART_MAIN, LV_STATE_PRESSED, LV_OPA_TRANSP);

	static rom_bin_info info = rom_bin_info_get(ROM_RES_SETTING_CAMERA1_FOCUS_PNG);
	static rom_bin_info info1 = rom_bin_info_get(ROM_RES_SETTING_CAMERA1_UNFOCUS_PNG);
	lv_obj_set_style_local_pattern_image(btn,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,set_camera_flag?&info1:&info);


    lv_obj_set_style_local_value_str(btn,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,text_str(STR_CAMERA1));
	if(set_camera_flag){
		lv_obj_set_style_local_value_color(btn,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT ,lv_color_make(255, 255,255));
		lv_obj_set_style_local_value_color(btn,LV_OBJ_PART_MAIN, LV_STATE_PRESSED,BTN_PRESS_COLOR);
	}else{
		lv_obj_set_style_local_value_color(btn,LV_OBJ_PART_MAIN, LV_STATE_DEFAULT ,BTN_PRESS_COLOR);
		lv_obj_set_style_local_value_color(btn,LV_OBJ_PART_MAIN, LV_STATE_PRESSED,BTN_PRESS_COLOR);
	}
	lv_obj_set_style_local_value_align(btn,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_ALIGN_OUT_BOTTOM_MID);
	lv_obj_set_style_local_value_ofs_y(btn,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,15);
	lv_obj_set_style_local_value_font(btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, FONT_SIZE(24));
	
	static btn_data btn_data1 = btn_data_create(NULL, set_camera1_flag_up, NULL);
    btn->user_data = &btn_data1;
    btn_touch_event_listen(btn);


	

	lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
   	lv_obj_set_pos(btn1, 58, 321);
    lv_obj_set_size(btn1, 66, 66);
    lv_obj_set_style_local_bg_opa(btn1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_bg_opa(btn1, LV_LABEL_PART_MAIN, LV_STATE_PRESSED, LV_OPA_TRANSP);

	static rom_bin_info info2 = rom_bin_info_get(ROM_RES_SETTING_CAMERA2_FOCUS_PNG);
	static rom_bin_info info3 = rom_bin_info_get(ROM_RES_SETTING_CAMERA2_UNFOCUS_PNG);
	lv_obj_set_style_local_pattern_image(btn1,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,set_camera_flag?&info2:&info3);

    lv_obj_set_style_local_value_str(btn1,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,text_str(STR_CAMERA2));
	if(!set_camera_flag){
		lv_obj_set_style_local_value_color(btn1,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT ,lv_color_make(255, 255,255));
		lv_obj_set_style_local_value_color(btn1,LV_OBJ_PART_MAIN, LV_STATE_PRESSED,BTN_PRESS_COLOR);
	}else{
		lv_obj_set_style_local_value_color(btn1,LV_OBJ_PART_MAIN, LV_STATE_DEFAULT ,BTN_PRESS_COLOR);
		lv_obj_set_style_local_value_color(btn1,LV_OBJ_PART_MAIN, LV_STATE_PRESSED,BTN_PRESS_COLOR);
	}
	lv_obj_set_style_local_value_align(btn1,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_ALIGN_OUT_BOTTOM_MID);
	lv_obj_set_style_local_value_ofs_y(btn1,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,15);
	lv_obj_set_style_local_value_font(btn1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, FONT_SIZE(24));
	
	static btn_data btn_data2 = btn_data_create(NULL, set_camera2_flag_up, NULL);
    btn1->user_data = &btn_data2;
    btn_touch_event_listen(btn1);

	lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
	lv_obj_set_pos(label, 470, 28);
    lv_obj_set_size(label, 124, 38);
	
	lv_label_set_text( label, set_camera_flag ? text_str(STR_CAMERA2) : text_str(STR_CAMERA1));
	lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
	lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, FONT_SIZE(24));
	
	


}

static void camera_set_btn_syn_up(lv_obj_t * obj)
{
		btn_data *pdata = (btn_data *) obj->user_data;
		lv_obj_t * btn = (lv_obj_t *) pdata->user_data;
		lv_obj_set_state(btn,LV_STATE_DEFAULT);
}
static void camera_set_btn_syn_down(lv_obj_t * obj)
{
		btn_data *pdata = (btn_data *) obj->user_data;
   		lv_obj_t * btn = (lv_obj_t *) pdata->user_data;
		lv_obj_set_state(btn,LV_STATE_PRESSED);
}

static void camera_set_btn_syn_event(lv_obj_t * obj,lv_event_t event)
{

	if(LV_EVENT_PRESS_LOST == event){
		camera_set_btn_syn_up(obj);
	}
		
}

static void camera_camera_switch_right_btn_up(lv_obj_t *obj)
{
    camera_set_btn_syn_up(obj);
	char* str1 = NULL;
	if(set_camera_flag){
		user_data_get()->camera2.enable = !user_data_get()->camera2.enable;
		str1 = user_data_get()->camera2.enable ? text_str(STR_ON) : text_str(STR_OFF);
	}else{
		user_data_get()->camera1.enable = !user_data_get()->camera1.enable;
		str1 = user_data_get()->camera1.enable ? text_str(STR_ON) : text_str(STR_OFF);
	}
	lv_obj_t * btn = lv_obj_get_child_form_id(lv_scr_act(),1);
	lv_obj_set_style_local_value_str(btn,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,str1);
}


static void camera_camera_switch_set_btn_create(Controls_location coordinate){
	static btn_data btn_data1 = btn_data_create(camera_set_btn_syn_down, camera_camera_switch_right_btn_up, NULL);
	static btn_data btn_data2 = btn_data_create(camera_set_btn_syn_down, camera_camera_switch_right_btn_up, NULL);
	static btn_data btn_data3 = btn_data_create(NULL, NULL, NULL);
	btn_data1.OPS_ANYTHING = camera_set_btn_syn_event;
	btn_data2.OPS_ANYTHING = camera_set_btn_syn_event;
	
	static char* str1 = NULL;
	
	if(set_camera_flag){
		str1 = user_data_get()->camera2.enable ? text_str(STR_ON) : text_str(STR_OFF);
	}else{
		str1 = user_data_get()->camera1.enable ? text_str(STR_ON) : text_str(STR_OFF);
	}
	lv_obj_t * btn = sys_setting_btn_create(coordinate,str1,text_str(STR_STATE),&btn_data3,&btn_data1,&btn_data2);
	lv_obj_set_id(btn, 1);

}


static void camera_camera_model_right_btn_up(lv_obj_t *obj)
{
    camera_set_btn_syn_up(obj);
	char* str1 = NULL;
	if(set_camera_flag){
		user_data_get()->camera2.model = (user_data_get()->camera2.model + 1) % 3;
		if (user_data_get()->camera2.model == 0)
			str1 = text_str(STR_DAHUA);
		else if (user_data_get()->camera2.model == 1)
			str1 = text_str(STR_HIKVISION);
		else
			str1 = text_str(STR_STECH);
	}else{
		user_data_get()->camera1.model = (user_data_get()->camera1.model + 1) % 3;
		if (user_data_get()->camera1.model == 0)
			str1 = text_str(STR_DAHUA);
		else if (user_data_get()->camera1.model == 1)
			str1 = text_str(STR_HIKVISION);
		else
			str1 = text_str(STR_STECH);
	}
	lv_obj_t * btn = lv_obj_get_child_form_id(lv_scr_act(),2);
	lv_obj_set_style_local_value_str(btn,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,str1);
}


static void camera_camera_model_set_btn_create(Controls_location coordinate){
	static btn_data btn_data1 = btn_data_create(camera_set_btn_syn_down, camera_camera_model_right_btn_up, NULL);
	static btn_data btn_data2 = btn_data_create(camera_set_btn_syn_down, camera_camera_model_right_btn_up, NULL);
	static btn_data btn_data3 = btn_data_create(NULL, NULL, NULL);
	btn_data1.OPS_ANYTHING = camera_set_btn_syn_event;
	btn_data2.OPS_ANYTHING = camera_set_btn_syn_event;

	static char* str1 = NULL;
	int model;

	if(set_camera_flag){
		model = user_data_get()->camera2.model;
	}else{
		model = user_data_get()->camera1.model;
	}

	if (model == 0)
		str1 = text_str(STR_DAHUA);
	else if (model == 1)
		str1 = text_str(STR_HIKVISION);
	else
		str1 = text_str(STR_STECH);

	lv_obj_t * btn = sys_setting_btn_create(coordinate,str1,text_str(STR_CAMERA_MODEL),&btn_data3,&btn_data1,&btn_data2);
	lv_obj_set_id(btn, 2);

}

static void camera_camera_stream_right_btn_up(lv_obj_t *obj)
{
	camera_set_btn_syn_up(obj);
	int *stream = &user_data_get()->camera_stream[set_camera_flag];

	*stream = (*stream == CCTV_STREAM_MAIN) ? CCTV_STREAM_SUB : CCTV_STREAM_MAIN;
	lv_obj_t *btn = lv_obj_get_child_form_id(lv_scr_act(), 9);
	lv_obj_set_style_local_value_str(btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,
		*stream == CCTV_STREAM_SUB ? text_str(STR_SUB_STREAM) : text_str(STR_MAIN_STREAM));
}

static void camera_camera_stream_set_btn_create(Controls_location coordinate)
{
	static btn_data btn_data1 = btn_data_create(camera_set_btn_syn_down, camera_camera_stream_right_btn_up, NULL);
	static btn_data btn_data2 = btn_data_create(camera_set_btn_syn_down, camera_camera_stream_right_btn_up, NULL);
	static btn_data btn_data3 = btn_data_create(NULL, NULL, NULL);
	btn_data1.OPS_ANYTHING = camera_set_btn_syn_event;
	btn_data2.OPS_ANYTHING = camera_set_btn_syn_event;

	lv_obj_t *btn = sys_setting_btn_create(coordinate,
		user_data_get()->camera_stream[set_camera_flag] == CCTV_STREAM_SUB ? text_str(STR_SUB_STREAM) : text_str(STR_MAIN_STREAM),
		text_str(STR_CHANNEL_SWITCH), &btn_data3, &btn_data1, &btn_data2);
	lv_obj_set_id(btn, 9);
}

static void camera_camera_ip_set_btn_up(lv_obj_t *obj)
{
    camera_set_btn_syn_up(obj);
	get_pwd_str = 2+set_camera_flag;
	goto_layout(pLAYOUT(password_input));
}
static void camera_camera_ip_set_btn_up_1(lv_obj_t *obj)
{
	get_pwd_str = 2+set_camera_flag;
	goto_layout(pLAYOUT(password_input));
}


static void camera_camera_ip_set_btn_create(Controls_location coordinate){
	static btn_data btn_data2 = btn_data_create(camera_set_btn_syn_down, camera_camera_ip_set_btn_up, NULL);
	static btn_data btn_data3 = btn_data_create(NULL, camera_camera_ip_set_btn_up_1, NULL);
	btn_data2.OPS_ANYTHING = camera_set_btn_syn_event;
	
	static char* str1 = NULL;
	if(set_camera_flag){
		str1 = user_data_get()->camera2.ip;
	}else{
		str1 = user_data_get()->camera1.ip;
	}
	lv_obj_t * btn = sys_setting_btn_create(coordinate,str1,text_str(STR_CAMERA_IP_ADDRESS),&btn_data3,NULL,&btn_data2);
	lv_obj_set_id(btn, 8);
}




static void camera_camera_account_set_btn_up(lv_obj_t *obj)
{
	camera_set_btn_syn_up(obj);
	get_pwd_str = 4+set_camera_flag;
	goto_layout(pLAYOUT(password_input));
	
}
static void camera_camera_account_set_btn_up_1(lv_obj_t *obj)
{
	get_pwd_str = 4+set_camera_flag;
	goto_layout(pLAYOUT(password_input));
	
}

static void camera_camera_account_set_btn_create(Controls_location coordinate){
	
	static btn_data btn_data2 = btn_data_create(camera_set_btn_syn_down, camera_camera_account_set_btn_up, NULL);
	static btn_data btn_data3 = btn_data_create(NULL, camera_camera_account_set_btn_up_1, NULL);
	btn_data2.OPS_ANYTHING = camera_set_btn_syn_event;
	
	static char* str1 = NULL;
	if(set_camera_flag){
		str1 = user_data_get()->camera2.account;
	}else{
		str1 = user_data_get()->camera1.account;
	}
	lv_obj_t * btn = sys_setting_btn_create(coordinate,str1,text_str(STR_ACCOUNT_NUMBER),&btn_data3,NULL,&btn_data2);
	lv_obj_set_id(btn, 3);
}


static void camera_camera_pwd_mode_set_btn_up(lv_obj_t *obj)
{
    camera_set_btn_syn_up(obj);
	get_pwd_str = 6+set_camera_flag;
	goto_layout(pLAYOUT(password_input));
}
static void camera_camera_pwd_mode_set_btn_up_1(lv_obj_t *obj)
{
	get_pwd_str = 6+set_camera_flag;
	goto_layout(pLAYOUT(password_input));
	
	
}


static void camera_camera_pwd_mode_set_btn_create(Controls_location coordinate){
	static btn_data btn_data2 = btn_data_create(camera_set_btn_syn_down, camera_camera_pwd_mode_set_btn_up, NULL);
	static btn_data btn_data3 = btn_data_create(NULL, camera_camera_pwd_mode_set_btn_up_1, NULL);
	btn_data2.OPS_ANYTHING = camera_set_btn_syn_event;
	
	static char* str1 = NULL;
	if(set_camera_flag){
		str1 = user_data_get()->camera2.pwd;
	}else{
		str1 = user_data_get()->camera1.pwd;
	}
	lv_obj_t * btn = sys_setting_btn_create(coordinate,str1,text_str(STR_PASSWORD),&btn_data3,NULL,&btn_data2);
	lv_obj_set_id(btn, 4);
}



static void camera_setting_back_btn_up(lv_obj_t *obj)
{

	#ifndef DHCP_IPCAMERA
    	goto_layout(pLAYOUT(setting));
	#else
		goto_layout(pLAYOUT(setting_ipc));
	#endif
}


static void ccamera_setting_display(void)
{	
	Controls_location module_coordinate[] =  CAMERA_MODULE_COORDINATE_INIT;
    camera_setting_btn_text_create();
	camera_camera_switch_set_btn_create(module_coordinate[SWITCH_MODULE]);
	camera_camera_model_set_btn_create(module_coordinate[MODEL_MODULE]);
	camera_camera_stream_set_btn_create(module_coordinate[STREAM_MODULE]);
	camera_camera_ip_set_btn_create(module_coordinate[IP_MODULE]);
	camera_camera_account_set_btn_create(module_coordinate[ACCOUNT_MODULE]);
	camera_camera_pwd_mode_set_btn_create(module_coordinate[PASSWORD_MODULE]);
	home_back_btn_create(camera_setting_back_btn_up,NULL);

}


static void LAYOUT_ENETER_FUNC(setting_camera)
{
	Debug("======================\n");
	setting_bg_display();
	if(prev_layout_get() != &layout_password_input)
	{
		set_camera_flag = 0;
	}
	cctv_diag_camera_config("setting_enter", 1, &user_data_get()->camera1, user_data_get()->camera_stream[0]);
	cctv_diag_camera_config("setting_enter", 2, &user_data_get()->camera2, user_data_get()->camera_stream[1]);
	ccamera_setting_display();

}


static void LAYOUT_QUIT_FUNC(setting_camera)
{
	cctv_diag_camera_config("setting_quit_before_save", 1, &user_data_get()->camera1, user_data_get()->camera_stream[0]);
	cctv_diag_camera_config("setting_quit_before_save", 2, &user_data_get()->camera2, user_data_get()->camera_stream[1]);
	if(user_data_get()->camera1.ip[0] != 0 && user_data_get()->camera1.ip[0] != '0')
	{
		cctv_stream_url_build(user_data_get()->camera1.url, sizeof(user_data_get()->camera1.url), user_data_get()->camera1.model,
			(cctv_stream_type)user_data_get()->camera_stream[0], user_data_get()->camera1.account, user_data_get()->camera1.pwd, user_data_get()->camera1.ip);
   }
   else
   {
		memset(user_data_get()->camera1.url,0,sizeof(user_data_get()->camera1.url));
   }
	if(user_data_get()->camera2.ip[0] != 0 && user_data_get()->camera2.ip[0] != '0')
	{
		cctv_stream_url_build(user_data_get()->camera2.url, sizeof(user_data_get()->camera2.url), user_data_get()->camera2.model,
			(cctv_stream_type)user_data_get()->camera_stream[1], user_data_get()->camera2.account, user_data_get()->camera2.pwd, user_data_get()->camera2.ip);
   }
   else
   {
		memset(user_data_get()->camera2.url,0,sizeof(user_data_get()->camera2.url));
   }
	cctv_diag_camera_config("setting_quit_after_url", 1, &user_data_get()->camera1, user_data_get()->camera_stream[0]);
	cctv_diag_camera_config("setting_quit_after_url", 2, &user_data_get()->camera2, user_data_get()->camera_stream[1]);
	 user_data_save();
	Debug("[CCTV_DIAG] setting_save_complete\n");
}


CREATE_LAYOUT(setting_camera);
