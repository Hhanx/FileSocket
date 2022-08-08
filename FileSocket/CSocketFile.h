#pragma once
#include "CSendFile.h"
#include "CrecvFile.h"
#include "resource.h"
class CSocketFile
{
private:
	
	//文件指针
	FILE* pf;
	//win中进度条声名
	CProgressCtrl* myProCtrl2;
	//进度条移动距离加长度
	int proCtrlen;
	int proCtrlenMax = 0;
	//标志位
	u_char Server_Client;//发送和接收 时封装在一起的 标志位用来清除内存
	//发送/接收缓冲区
	//char* BUFF_1MB;//传输1MB / 接收缓冲区
	char BUFF_500kb[1024 * 500];//传输500KB / 接收缓冲区
	char BUFF_1kb[1024];//传输1KB
	char BUFF_64byte[64];
	char BUFF_1byte;//传输1byte
	char fileName[1024];//文件名
	char filePathName[1024];//全路径
	//接收端
	SOCKET Server;
	SOCKADDR_IN addrServer;
	//发送端
	SOCKET Client;
	SOCKADDR_IN addrClient;
	////win指针
	void* param;
	//文件总大小
	__int64 nSize4;
	//发送总次数
    int c;
	//server本机IP/用户名称信息
	char szHostName[MAX_PATH] = { 0 };
	char* lpLocalIP;//本地IP
	PHOSTENT hostinfo;
public:
	CSocketFile(void* param);//在初始化的时候传递win指针
	~CSocketFile();
	BOOL GetOWnIp(void* param);
	BOOL CreateServer();//创建socket 意思就是 创建本地套接字
	BOOL BindOwnIp(CString port);
	BOOL LisentClient();
	BOOL RecvFileName();
	BOOL RecvFileSize();
	BOOL RecvFileData();
	BOOL CreateClient();//创建socket 意思就是 创建本地套接字
	BOOL SetServer(CString ip, CString port); //绑定服务端IP，端口
	BOOL ConnectServer();//连接服务端
	BOOL SendName(CString name);//发送文件名称
	BOOL SendFile(CString pathName);//发送文件
private:
	//返回剩余长度,带参数传递最后的长度
	//void SendFileTo1MB(long long count);
	void SendFileTo500KB(long long count);
	void SendFileTo1KB(long long count);
	void SendFileTo64byte(long long count);
	void SendFileTo1byte(long long count);
	
};

