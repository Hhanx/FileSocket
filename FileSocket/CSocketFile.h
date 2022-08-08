#pragma once
#include "CSendFile.h"
#include "CrecvFile.h"
#include "resource.h"
class CSocketFile
{
private:
	
	//�ļ�ָ��
	FILE* pf;
	//win�н���������
	CProgressCtrl* myProCtrl2;
	//�������ƶ�����ӳ���
	int proCtrlen;
	int proCtrlenMax = 0;
	//��־λ
	u_char Server_Client;//���ͺͽ��� ʱ��װ��һ��� ��־λ��������ڴ�
	//����/���ջ�����
	//char* BUFF_1MB;//����1MB / ���ջ�����
	char BUFF_500kb[1024 * 500];//����500KB / ���ջ�����
	char BUFF_1kb[1024];//����1KB
	char BUFF_64byte[64];
	char BUFF_1byte;//����1byte
	char fileName[1024];//�ļ���
	char filePathName[1024];//ȫ·��
	//���ն�
	SOCKET Server;
	SOCKADDR_IN addrServer;
	//���Ͷ�
	SOCKET Client;
	SOCKADDR_IN addrClient;
	////winָ��
	void* param;
	//�ļ��ܴ�С
	__int64 nSize4;
	//�����ܴ���
    int c;
	//server����IP/�û�������Ϣ
	char szHostName[MAX_PATH] = { 0 };
	char* lpLocalIP;//����IP
	PHOSTENT hostinfo;
public:
	CSocketFile(void* param);//�ڳ�ʼ����ʱ�򴫵�winָ��
	~CSocketFile();
	BOOL GetOWnIp(void* param);
	BOOL CreateServer();//����socket ��˼���� ���������׽���
	BOOL BindOwnIp(CString port);
	BOOL LisentClient();
	BOOL RecvFileName();
	BOOL RecvFileSize();
	BOOL RecvFileData();
	BOOL CreateClient();//����socket ��˼���� ���������׽���
	BOOL SetServer(CString ip, CString port); //�󶨷����IP���˿�
	BOOL ConnectServer();//���ӷ����
	BOOL SendName(CString name);//�����ļ�����
	BOOL SendFile(CString pathName);//�����ļ�
private:
	//����ʣ�೤��,�������������ĳ���
	//void SendFileTo1MB(long long count);
	void SendFileTo500KB(long long count);
	void SendFileTo1KB(long long count);
	void SendFileTo64byte(long long count);
	void SendFileTo1byte(long long count);
	
};

