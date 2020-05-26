# libavcombine
基于ffmpeg开发的高效音视频合成库，可以算是ffmpeg的一个小example，封装成DLL用起来更省事。

## IAVCom

实现了音频视频合成算法的接口

## AVCom

音频视频合成实现类

类方法：

void setAudio(LPCSTR audio); //指定音频文件路径

void setVideo(LPCSTR video); //指定视频文件路径

void setOutPath(LPCSTR path); //指定输出文件路径(要求拓展名为已知且合法的媒体格式)

virtual bool OpenStream(); //打开文件流

virtual bool WriteFile(); //写入文件
