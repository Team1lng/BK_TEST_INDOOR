cmake_minimum_required(VERSION 2.8)

# cmake最低使用版本
# /opt/arm-anykav500-linux-uclibcgnueabi/usr/bin/
# /opt/arm-anykav500-linux-uclibcgnueabi/usr/bin/
set(CMAKE_C_COMPILER "arm-anykav500-linux-uclibcgnueabi-gcc")
set(CMAKE_CXX_COMPILER "arm-anykav500-linux-uclibcgnueabi-g++")
# 设置编译器，此处为ARM平台，如果你安装了编译链就用名称，没安装请用绝对路径
SET(leo_proj ANYKA37E.BIN)
add_definitions(-DLV_LVGL_H_INCLUDE_SIMPLE)
add_definitions(
	-D__LITTLE_ENDIAN__
	-D_GNU_SOURCE
	#回铃声优化
	-DRING_BACK_MODIFY
)

set(PUBLIC_LOGO_NAME "public.rgb")
set(TMEZON_LOGO_NAME "tmezon.rgb")
set(STECH_LOGO_NAME "stech.rgb")
set(MASIF_LOGO_NAME "masif.rgb")
set(JIAYI_LOGO_NAME "jiayi.rgb")
set(NAIWEI_LOGO_NAME "naiwei.rgb")
set(JEATONE_LOGO_NAME "jeatone.rgb")
set(KAIDI_LOGO_NAME "kaidi.rgb")
set(MASYB_LOGO_NAME "masyb.rgb")
set(INTERCOM_LOGO_NAME "intercom.rgb")
set(GVS_LOGO_NAME "gvs.rgb")
set(HOMSECUR_LOGO_NAME "homsecur.rgb")
set(TARGET_LOGO_NAME "anyka_logo.rgb")

SET(IPS_SCREEN 10)
SET(CMAKE_APP_OID OFF)

# add_definitions(-DDHCP_IPCAMERA)
add_definitions(-DDOORBELL_UPGRADE)
add_definitions(-DRING_BACK_MODIFY)
add_definitions(-DSTECH_VERSION)
# 使能法语
SET(CMAKE_FRENCH_ENABLE OFF)
if(CMAKE_FRENCH_ENABLE)
	add_definitions(-DLANGUAGE_FRENCH_ENABLE)
	add_definitions(-DIPC_APP_VERSION="2.1.8")
else()
	add_definitions(-DIPC_APP_VERSION="9.9.9")
endif()

#定制PID
if(NOT CMAKE_APP_OID)
	if(IPS_SCREEN EQUAL 10)
	add_definitions(-D_10_IPS_SCREEN)
	add_definitions(-DIPC_APP_PID="0htjckdwyflg65oi")
	MESSAGE("_10_IPS_SCREEN")
	elseif(IPS_SCREEN EQUAL 8)
	add_definitions(-D_8_IPS_SCREEN)
	add_definitions(-DIPC_APP_PID="wlmukdvb76nx93ul")
	MESSAGE("_8_IPS_SCREEN")
	else()
	add_definitions(-DIPC_APP_PID="ggdi7gxpghq6fbxf")
	MESSAGE("_7_IPS_SCREEN")
	endif()
else()
	if(IPS_SCREEN EQUAL 10)
		add_definitions(-D_10_IPS_SCREEN)
	elseif(IPS_SCREEN EQUAL 8)
		add_definitions(-D_8_IPS_SCREEN)
	endif()
	add_definitions(-DIPC_APP_PID="8hbui777kxqebtpk")
endif()

#博科天气模块
add_definitions(-DWEATHER_MOUDLE)	
#信箱模块
add_definitions(-DMAILBOX_MODULE_EN)
#户户通
add_definitions(-DFAMILY_TRANSFER_MODULE)
#SD卡分区
add_definitions(-DSDCARD_PARTITION)


#门口机编译日期
# add_definitions(-DDOOR_COMPILE_TIME)

# #指纹管理
add_definitions(-DADMIN_FINGER_MANAGEMENT)
# #卡片管理
add_definitions(-DADMIN_CARD_MANAGEMENT)
# #默认音频参数
# add_definitions(-DAUDIO_CONFIG_DEFAULT)


#开启调音工具
SET(CMAKE_ATS_OPEN OFF)
if(CMAKE_ATS_OPEN)
    add_definitions(-DAPP_ATS_OPEN)
endif()

#ROM文件临时存储路径
add_definitions(-DROM_BIN_TEMP_PATH="/tmp/rom.bin")


# add_definitions(-DLINK_LIST_ENABLE)

if(CMAKE_APP_OID)
add_definitions(-DBCOM_OID_VERSION)
#字库路径
add_definitions(-DTTF_FILE_PATH="/etc/config/sat_leo.ttf")
#ROM文件存储路径
add_definitions(-DROM_BIN_PATH="/etc/config/rom.bin")
#涂鸦文件存储路径
add_definitions(-DTUYA_DATA_PATH="/etc/config/")
#铃声文件存储路径
add_definitions(-DRING_FILE_PATH="/etc/config/rings/")
#xls文件存储路径
add_definitions(-DTTF_XLS_PATH="/etc/config/language.xls")
#用户数据存储路径
add_definitions(-DUSER_FILE_PATH="/etc/config/")
else()
#字库路径
add_definitions(-DTTF_FILE_PATH="/app/app/sat_leo.ttf")
#ROM文件存储路径
add_definitions(-DROM_BIN_PATH="/app/app/rom.bin")
#涂鸦文件存储路径
add_definitions(-DTUYA_DATA_PATH="/app/data/")
#铃声文件存储路径
add_definitions(-DRING_FILE_PATH="/app/app/rings/")
#xls文件存储路径
add_definitions(-DTTF_XLS_PATH="/app/app/language.xls")
#用户数据存储路径
add_definitions(-DUSER_FILE_PATH="/app/data/")
#tuya uuid存储路径
add_definitions(-DTUYA_KEY_PATH="/app/data/tuya_key/")
#tuya缓存存储路径
add_definitions(-DTUYA_CACHE_PATH="/app/data/tuya_cache/")
endif()


add_definitions(-DAPP_DEFAULT_PATH="/etc/config/")
#wlan_check文件存储路径
add_definitions(-DWLAN_CHECK_PATH="/etc/config/wlan_check.sh")
#wpa_supplicant文件存储路径
add_definitions(-DWPA_SUPPLICANT_PATH="/etc/config/wpa_supplicant.conf")
#音乐文件存储路径
add_definitions(-DMUSIC_FILE_PATH="/mnt/tf/music/")



project(${leo_proj})

# 工程名

# 添加src目录
add_subdirectory(src)
