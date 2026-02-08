#!/bin/bash

build_usage()
{
    clear
	echo "========================================================"
	echo "build Usage : "
	echo "  $0 -a           编译打包"
	echo "  $0 -c           清除缓存"
    echo ""
	echo "========================================================"
}

build_all()
{
    IPS_SCREEN=$(grep -E '^SET\(IPS_SCREEN' CMakeLists.txt | head -1 | awk -F'[() ]+' '{print $3}')

    IPC_APP_VERSION=$(grep -A5 -B5 'CMAKE_FRENCH_ENABLE' CMakeLists.txt | grep 'IPC_APP_VERSION' | tail -1 | sed 's/.*IPC_APP_VERSION="\([^"]*\)".*/\1/')

    mkdir -p build
    cd build
    cmake ..
    make -j16
    make
    if [ $? -ne 0 ]; then
        echo "编译失败"
        exit 1
    fi
    cd -

    cp -rf src/layout/resource/onvif ./upgrade/app/
    cp -rf src/layout/resource/rings ./upgrade/app/
    cp -rf src/layout/resource/language/* ./upgrade/app/
    cp -rf src/layout/resource/scripts/* ./upgrade/app/
    cp -rf src/layout/resource/rom.bin ./upgrade/app/
    cp -rf build/src/ANYKA37E.BIN ./upgrade/app/

    # # 拷贝到SDK
    # rm -rf ../../../SDK/AK37E_SDK_V1.05/rootfs/rootfs/app/app
    # cp -rf ./upgrade/app ../../../SDK/AK37E_SDK_V1.05/rootfs/rootfs/app/
    # cp -f ./upgrade/platform/anyka_logo.rgb ../../../SDK/AK37E_SDK_V1.05/image/anyka_logo.rgb

    cd ./upgrade
    cp -rf dtb/EVB_CBDM_AK3760E_V1.0.1-$IPS_SCREEN寸.dtb platform/EVB_CBDM_AK3760E_V1.0.1.dtb
    ./make_image.sh
    cd -
    rm -rf 五棵树-$IPS_SCREEN寸*
    zip 五棵树-$IPS_SCREEN寸$IPC_APP_VERSION-$(date +%y%m%d%H%M%S).ZIP SAT_ANYKAOS_*
    echo "屏幕:$IPS_SCREEN寸  版本:$IPC_APP_VERSION"
    if [ -d "/home/wxj/nfs" ]; then
        cp -rf build/src/ANYKA37E.BIN /home/wxj/nfs
    fi
}

clean_cache()
{
    rm -rf build
}

main() {
    case $option1 in
        "-a")
            echo "编译打包"
            build_all
            ;;
        "-c")
            echo "清除缓存"
            clean_cache
            ;;
        *)
            build_usage
            ;;
    esac
}

option1=$1
main

