```
编译命令（会在当前目录下生成SAT_ANYKAOS_XX的升级包和压缩包）
./make.sh -a
清除缓存命令
./make.sh -c

注：编译前请将工具链安装到/opt目录下，并添加到环境变量
```
```
├── build           // 编译缓存
├── doc             // 文档
├── src             // 源码
├── upgrade         // 升级文件打包
├── CMakeLists.txt  // cmake
├── make.sh         // 编译脚本
├── README.md       // 工程说明
└── 修改日志.txt
```