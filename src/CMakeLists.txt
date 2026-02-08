# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_C_FLAGS_DEBUG  "$ENV{CXXFLAGS} -Wall -std=gnu11  -g"  ) # -Werror
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -std=c++1y  -g") # -Werror
SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_C_FLAGS_RELEASE  	"$ENV{CXXFLAGS} -Os -Wall -std=gnu11 -s -g -Werror") # -Werror
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Os -Wall -std=c++1y -s -g -Werror") # -Werror

find_package(Threads)

aux_source_directory(. DIR_SRCS)
aux_source_directory(./layout DIR_SRCS)

# aux_source_directory(./layout/resource/icon_font/output DIR_SRCS) 

aux_source_directory(./api/live555 DIR_SRCS) 
#aux_source_directory(./tuya_batch_production_sdk DIR_SRCS) 
#message("DIR_SRC : ${DIR_SRCS}")

add_subdirectory(lvgl)        #添加目录 lvgl
add_subdirectory(lv_drivers)  #添加目录 lv_drivers
add_subdirectory(lv_qrcode)  #添加目录 lv_qrcode
add_subdirectory(api)  #添加目录 api
add_subdirectory(tuya_batch_production_sdk)  #添加目录 tuya_batch_production_sdk
add_subdirectory(iniparser)  
add_subdirectory(tuya)

include_directories(./include/anyka)  
include_directories(./include/api)  
include_directories(./include/api/video) 
include_directories(./include/freetype)  
include_directories(./include/libmad)  
include_directories(./include/tuya)
include_directories(./include/g711)
include_directories(./tuya/)

include_directories(./include/live555/BasicUsageEnvironment/include)  
include_directories(./include/live555/groupsock/include)  
include_directories(./include/live555/liveMedia/include)  
include_directories(./include/live555/UsageEnvironment/include)  



include_directories(./lvgl/) 
include_directories(./lv_drivers/)  
include_directories(./lv_qrcode/)  
include_directories(./layout/) 

include_directories(./api/freetype/) 
include_directories(./api/network/) 
include_directories(./api/tcp_network/) 
include_directories(./iniparser/src)  
include_directories(./tuya_batch_production_sdk/include/libxls) 
include_directories(./tuya_batch_production_sdk/include/iconv) 
include_directories(./tuya_batch_production_sdk/xls) 
include_directories(./onvif/include) 

# include_directories(./sqlite/build/include) 

#set(VAR lv_examples_lib)
set(VAR ${VAR} lv_qrcode_lib)
set(VAR ${VAR} lvgl_lib)
set(VAR ${VAR} lv_drivers_lib)
set(VAR ${VAR} leo_api_lib)
set(VAR ${VAR} xls)
set(VAR ${VAR} iniparser)
set(VAR ${VAR} tuya)

add_executable(${leo_proj} ${DIR_SRCS})  #链接生成文件${leo_proj} 由DIR_SRC链接。

message("VAR : ${VAR}")

set(ANYKA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/anyka)
set(LIVE555_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/live555)

target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_tde.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_common.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_thread.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_mem.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_osal.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_log.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_dbg.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libmpi_vdec.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libakv_decode.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libmpi_venc.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libakv_encode.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_ao.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_vqe.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libakaudiofilter.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_ai.so)

# 调音所需库，不调音可以屏蔽
if(${CMAKE_ATS_OPEN})
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libakaudiocodec.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libapp_ats.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libmpi_aenc.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libmpi_adec.so)
target_link_libraries(${leo_proj} ${ANYKA_PATH}/libplat_timer.so)
endif()

target_link_libraries(${leo_proj}  -Wl,--start-group 
	${VAR}

	m
	pthread
	dl

	${CMAKE_CURRENT_SOURCE_DIR}/onvif/lib/libbase64.a
	${CMAKE_CURRENT_SOURCE_DIR}/onvif/lib/libmxml.a
	${CMAKE_CURRENT_SOURCE_DIR}/onvif/lib/libonvif.a
	${CMAKE_CURRENT_SOURCE_DIR}/onvif/lib/libsha1.a

	${LIVE555_PATH}/libBasicUsageEnvironment.a
	${LIVE555_PATH}/libgroupsock.a
	${LIVE555_PATH}/libliveMedia.a
	${LIVE555_PATH}/libUsageEnvironment.a

	${CMAKE_CURRENT_SOURCE_DIR}/tuya/libs/libtuyaos.a
	${CMAKE_CURRENT_SOURCE_DIR}/lib/iconv/libiconv.a
	${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype/libfreetype.a
	${CMAKE_CURRENT_SOURCE_DIR}/lib/mad/libmad.a
	${CMAKE_CURRENT_SOURCE_DIR}/lib/jpg/libjpeg.a
	${CMAKE_CURRENT_SOURCE_DIR}/lib/g711/libg711.a
	${CMAKE_CURRENT_SOURCE_DIR}/lib/resample/libsamplerate.a
-Wl,--end-group)
