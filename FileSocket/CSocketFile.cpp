
#include "pch.h"
#include "CSocketFile.h"

CSocketFile::CSocketFile(void* param)
{
	this->param = param;//对方类传递过来了
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata)) {
		MessageBox(NULL,L"初始化错误", L"提示", MB_ICONINFORMATION | MB_OK);
		delete(this);
	}
	//BUFF_1MB = new char[1024 * 1024];
}

CSocketFile::~CSocketFile()
{
	if (Server_Client == 'S'){
		closesocket(Server);
	}
	else if(Server_Client == 'C') {
		closesocket(Client);
	}
	WSACleanup();
	delete(this);
	//delete(this->BUFF_1MB);
}

BOOL CSocketFile::GetOWnIp(void* param)
{
	USES_CONVERSION;
	//获取本机名称
	gethostname(szHostName, sizeof(szHostName));
	//获取本机IP
	hostinfo = gethostbyname(szHostName);
	lpLocalIP = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
	CString ip = A2W(lpLocalIP);
	CrecvFile* server = (CrecvFile*)param;
	server->GetDlgItem(IDC_IPADDRESS2)->SetWindowTextW(ip);
	WSACleanup();
	return 0;
}

BOOL CSocketFile::CreateServer()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata)) {
		MessageBox(NULL, L"初始化错误", L"提示", MB_ICONINFORMATION | MB_OK);
		delete(this);
	}
	this->Server_Client = 'S';
	Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Server == INVALID_SOCKET) {
		MessageBox(NULL, L"创建失败！",
			L"提示",
			MB_ICONINFORMATION | MB_OK);
		closesocket(Server);
		WSACleanup();
		delete(this);
		return false;
	}
	return true;
}

BOOL CSocketFile::BindOwnIp(CString port)
{
	CrecvFile* server = (CrecvFile*)param;
	u_short Port = StrToInt(server->Port);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(Port);
	addrServer.sin_addr.S_un.S_addr = inet_addr(lpLocalIP);
	//绑定身份
	int nRet = bind(Server,(sockaddr*)&addrServer,sizeof(addrServer));
	if (nRet == SOCKET_ERROR) { 
		server->MessageBox(L"初始化失败!"); 
		closesocket(Server); 
		WSACleanup();
		delete(this);
		return false;
	}
	return true;
}

BOOL CSocketFile::LisentClient()
{
	//监听连过来的客户机
	CrecvFile* server = (CrecvFile*)param;
	int nRet = listen(Server,1);
	if (nRet == SOCKET_ERROR) {
		server->MessageBox(L"错误");
		closesocket(Server);
		WSACleanup();
		delete(this);
		return false;
	}
	//成功以后 就关掉定时
	int len = sizeof(addrClient);
	Client = accept(Server,(sockaddr*)&addrClient,&len);
	//int a = WSAGetLastError();
	//CString strin;
	//strin.Format(L"%d", a);
	//server->MessageBox(strin);
	if (Client == INVALID_SOCKET){
		server->MessageBox(L"错误");
		closesocket(Server);		WSACleanup();
		delete(this);
		return false;
	}
	else {//连接通过以后
		char* ClientIp = inet_ntoa(addrClient.sin_addr);//把对方ip显示在Win窗口上面
		USES_CONVERSION;
		CString ClientIpStr = A2W(ClientIp);
		server->KillTimer(1);
		server->GetDlgItem(IDC_IPADDRESS3)->SetWindowTextW(ClientIpStr);
		server->GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"连接成功");
	}
	return true;
}

BOOL CSocketFile::RecvFileName()
{
	//接收文件名称
	USES_CONVERSION;
	CrecvFile* server = (CrecvFile*)param;
	memset(filePathName,0,sizeof(filePathName));
	recv(Client, filePathName, sizeof(filePathName), 0);
	server->SetTimer(2, 500, NULL);
	CString filename;
	filename.Format(_T("\\%s"), (CStringW)filePathName);
	server->filePath = server->filePath + filename;
	strcpy(filePathName,W2A(server->filePath));
	CString str;
	str += "文件名称:  ";
	filename.TrimLeft('\\');
	str += filename;
	server->GetDlgItem(IDC_EDIT1)->SetWindowTextW(str);

	return true;
}

BOOL CSocketFile::RecvFileSize()
{
	USES_CONVERSION;
	CrecvFile* server = (CrecvFile*)param;
	memset(BUFF_1kb, 0, sizeof(BUFF_1kb));
	recv(Client, BUFF_1kb, sizeof(BUFF_1kb), 0);	
	CString str;
	server->GetDlgItem(IDC_EDIT1)->GetWindowTextW(str);
	str += "\r\n";
	str += "文件长度:  ";
	str += BUFF_1kb;
	server->GetDlgItem(IDC_EDIT1)->SetWindowTextW(str);
	nSize4 = atoll(BUFF_1kb);
	memset(BUFF_1kb, 0, sizeof(BUFF_1kb));
	return true;
}

BOOL CSocketFile::RecvFileData()
{
	CrecvFile* server = (CrecvFile*)param;
	myProCtrl2 = (CProgressCtrl*)server->GetDlgItem(IDC_PROGRESS2);
	myProCtrl2->SetRange32(0, 100000);
	pf = fopen(filePathName,"wb");
	if (pf == NULL) {
		::fclose(pf);
		MessageBox(NULL, L"打开文件失败！", L"提示", MB_ICONINFORMATION | MB_OK);
		delete(this);
		return false;
	}
	long long pos500KB = 0, pos1KB = 0, pos64byte = 0, pos1byte = 0, count = 0;
	long long fileSize = nSize4;

	if (fileSize >= (1024 * 500)) {
		pos500KB = fileSize / (1024 * 500); //几次？
		fileSize -= ((1024 * 500) * pos500KB);//剩下？
	}
	if (fileSize >= 1024 && fileSize <= (1024 * 500)) {
		pos1KB = fileSize / 1024; //几次？
		fileSize -= (1024 * pos1KB);//剩下？
	}
	if (fileSize >= 64 && fileSize <= 1024) {
		pos64byte = fileSize / 64; //几次？
		fileSize -= (64 * pos64byte);//剩下？
	}
	if (fileSize <= 64)
	{
		pos1byte = fileSize;//几次？
	}
	count += pos500KB;
	count += pos1KB;
	count += pos64byte;
	count += pos1byte;//总数
	server->poslen = (100000 / count);

	CString str;
	int filelength=0;
	int pos = 0;
	while (true)
	{	
		memset(BUFF_500kb, 0, sizeof(BUFF_500kb));
		filelength = recv(Client, BUFF_500kb, sizeof(BUFF_500kb), 0);
		if (!(filelength > 0)) {
			break;
		}
		pos += (int)server->poslen;
		myProCtrl2->SetPos(pos);
		fwrite(BUFF_500kb, 1, filelength, pf);
		
		if (((CButton*)server->GetDlgItem(IDC_RADIO3))->GetCheck()) {

			server->GetDlgItem(IDC_EDIT1)->GetWindowTextW(str);
			str += "\r\n";
			str += BUFF_500kb;
			server->GetDlgItem(IDC_EDIT1)->SetWindowTextW(str);
		}
	}
	::fclose(pf);
	server->KillTimer(2);
	server->GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"接收完毕");
	myProCtrl2->SetPos(100000);
	closesocket(Server);
	WSACleanup();
	return true;
}

BOOL CSocketFile::CreateClient()
{
	Server_Client = 'C';
	Client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Client == INVALID_SOCKET) { 
		MessageBox(NULL, L"创建失败！", 
			L"提示", 
			MB_ICONINFORMATION | MB_OK); 
		closesocket(Client);
		WSACleanup();
		delete(this);
		return false; 
	}
	return true;
}

BOOL CSocketFile::SetServer(CString ip,CString port)
{
	USES_CONVERSION;
	char* pIp = W2A(ip);
	u_short Port;
	Port = StrToInt(port);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(Port);
	addrServer.sin_addr.S_un.S_addr = inet_addr(pIp);
return true;
}

BOOL CSocketFile::ConnectServer()
{
	int nRet;
	while (true)
	{
		nRet = connect(Client, (sockaddr*)&addrServer, sizeof(addrServer));
		if (nRet != -1) {
			return true;
		}
		Sleep(10);
	}
	return false;
}

BOOL CSocketFile::SendName(CString name)
{
	CSendFile* classSendFile = (CSendFile*)param;
	CString str;
	str += "文件名称:  ";
	str += name;
	classSendFile->GetDlgItem(IDC_EDIT1)->SetWindowTextW(str);
	memset(fileName, 0, sizeof(fileName));
	USES_CONVERSION;
	char* b;
	LPWSTR a = name.GetBuffer();
	b = W2A(a); //CString 转 LPWSTR  => LPWSTR 转 char*
	sprintf(fileName,"%s",b);

	int nRet = send(Client, fileName,sizeof(fileName),0);
	if (nRet == SOCKET_ERROR) {
		MessageBox(NULL, L"发送失败！",L"提示",MB_ICONINFORMATION | MB_OK);
		closesocket(Client);
		WSACleanup();
		delete(this);
		return false;
	}
	return true;
}

int CSocketFile::SendFile(CString pathName)
{
	//初始化数组
	CSendFile* classSendFile = (CSendFile*)param;
	myProCtrl2 = (CProgressCtrl*)classSendFile->GetDlgItem(IDC_PROGRESS2);
	myProCtrl2->SetRange32(0,100000);
	memset(filePathName, 0, sizeof(filePathName));
	memset(BUFF_500kb, 0, sizeof(BUFF_500kb));
	memset(BUFF_1kb, 0, sizeof(BUFF_1kb));
	//把文件路径名字转换一下
	//CString 转 LPWSTR  => LPWSTR 转 char*
	USES_CONVERSION;
	char* b;
	LPWSTR a = pathName.GetBuffer();
	b = W2A(a);
	sprintf(filePathName, "%s", b);
	//打开文件
	pf = fopen(filePathName, "rb");
	if (pf == NULL) {
		fclose(pf);
		MessageBox(NULL, L"打开文件失败！",L"提示",MB_ICONINFORMATION | MB_OK);
		delete(this);
		return false;
	}
	//获取文件的大小
	_fseeki64(pf, 0, SEEK_END);
	nSize4 = _ftelli64(pf);
	sprintf(BUFF_1kb,"%lld", nSize4);
	send(Client, BUFF_1kb, sizeof(BUFF_1kb), 0);
	_fseeki64(pf, 0, SEEK_SET);

	CString str;
	classSendFile->GetDlgItem(IDC_EDIT1)->GetWindowTextW(str);
	str += "\r\n";
	str += "文件长度:  ";
	str += BUFF_1kb;
	classSendFile->GetDlgItem(IDC_EDIT1)->SetWindowTextW(str);
	
	//开始发送数据
	long long /*pos1MB = 0, */pos500KB = 0, pos1KB = 0, pos64byte = 0, pos1byte = 0, count = 0;
	long long fileSize = nSize4;

	switch (classSendFile->sendSpeed)
	{
	case 1:
		if (fileSize >= (1024 * 500)) {
			pos500KB = fileSize / (1024 * 500); //几次？
			fileSize -= ((1024 * 500) * pos500KB);//剩下？
		}
		if (fileSize >= 1024 &&
			fileSize <= (1024 * 500)) {
			pos1KB = fileSize / 1024; //几次？
			fileSize -= (1024 * pos1KB);//剩下？
		}
		if (fileSize >= 64 &&
			fileSize <= 1024) {
			pos64byte = fileSize / 64; //几次？
			fileSize -= (64 * pos64byte);//剩下？
		}
		if (fileSize <= 64) {
			pos1byte = fileSize;//几次？
		}
		count += (/*pos1MB + */pos500KB);//总数
		count += (pos1KB + pos64byte);
		count += pos1byte;
		proCtrlen = (int)(100000 / count);//每一次移动的距离
		break;
	case 2:
		classSendFile->MessageBox(L"慢不死你");
		if (fileSize >= 1024 &&
			fileSize <= (1024 * 500)) {
			pos1KB = fileSize / 1024; //几次？
			fileSize -= (1024 * pos1KB);//剩下？
		}
		if (fileSize >= 64 &&
			fileSize <= 1024) {
			pos64byte = fileSize / 64; //几次？
			fileSize -= (64 * pos64byte);//剩下？
		}
		if (fileSize <= 64) {
			pos1byte = fileSize;//几次？
		}
		count += (pos1KB + pos64byte);
		count += pos1byte;
		proCtrlen = (int)(100000 / count);//每一次移动的距离
		break;
	case 3:
		classSendFile->MessageBox(L"慢死你");
		if (fileSize >= 64 &&
			fileSize <= 1024) {
			pos64byte = fileSize / 64; //几次？
			fileSize -= (64 * pos64byte);//剩下？
		}
		if (fileSize <= 64) {
			pos1byte = fileSize;//几次？
		}
		count += pos64byte;
		count += pos1byte;
		proCtrlen = (int)(100000 / count);//每一次移动的距离
		break;
	case 4:
		classSendFile->MessageBox(L"慢死你");
		if (fileSize <= 64) {
			pos1byte = fileSize;//几次？
		}
		count += pos1byte;
		proCtrlen = (int)(100000 / count);//每一次移动的距离
		break;
	}
	



	if (classSendFile->sendSpeed == 1)
	{
		SendFileTo500KB(pos500KB);
		SendFileTo1KB(pos1KB);
		SendFileTo64byte(pos64byte);
		SendFileTo1byte(pos1byte);
	}
	else if (classSendFile->sendSpeed == 2) {
		SendFileTo1KB(pos1KB);
		SendFileTo64byte(pos64byte);
		SendFileTo1byte(pos1byte);
	}
	else if (classSendFile->sendSpeed == 3) {
		SendFileTo64byte(pos64byte);
		SendFileTo1byte(pos1byte);
	}
	else if (classSendFile->sendSpeed == 4) {
		SendFileTo1byte(pos1byte);
	}
	fclose(pf);
	closesocket(Client);
	WSACleanup();
	classSendFile->KillTimer(2);
	myProCtrl2->SetPos(100000);
	classSendFile->GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"发送完成");
	return 0;
}

void CSocketFile::SendFileTo500KB(long long count)
{
	CSendFile* classSendFile = (CSendFile*)param;
	CString str;
	for (__int64 i = count; i > 0; i--) {
		fread(BUFF_500kb, 1, 512000, pf);
		if (((CButton*)classSendFile->GetDlgItem(IDC_RADIO3))->GetCheck()) {
		
			classSendFile->GetDlgItem(IDC_EDIT1)->GetWindowTextW(str);
			str += "\r\n";
			str += BUFF_1kb;
			classSendFile->GetDlgItem(IDC_EDIT1)->SetWindowTextW(str);
			if (str.GetLength() >= 521000)
				str = " ";
		}
		send(Client, BUFF_500kb, sizeof(BUFF_500kb), 0);
		proCtrlenMax += proCtrlen;
		 myProCtrl2->SetPos(proCtrlenMax);
	}
}

void CSocketFile::SendFileTo1KB(long long count)
{
	CSendFile* classSendFile = (CSendFile*)param;
	CString str;
	for (__int64 i = count; i > 0; i--) {
		fread(BUFF_1kb, 1, 1024, pf);
		if (((CButton*)classSendFile->GetDlgItem(IDC_RADIO3))->GetCheck()) {
			
			classSendFile->GetDlgItem(IDC_EDIT1)->GetWindowTextW(str);
			str += "\r\n";
			str += BUFF_1kb;
			classSendFile->GetDlgItem(IDC_EDIT1)->SetWindowTextW(str);
			if (str.GetLength() >= 521000)
				str = " ";
		}
		send(Client, BUFF_1kb, sizeof(BUFF_1kb), 0);
		proCtrlenMax += proCtrlen;
		myProCtrl2->SetPos(proCtrlenMax);
	}
}

void CSocketFile::SendFileTo64byte(long long count)
{
	CSendFile* classSendFile = (CSendFile*)param;
	CString str;
	for (__int64 i = count; i > 0; i--) {
		fread(BUFF_64byte, 1, 64, pf);
		if (((CButton*)classSendFile->GetDlgItem(IDC_RADIO3))->GetCheck()) {
			
			classSendFile->GetDlgItem(IDC_EDIT1)->GetWindowTextW(str);
			str += "\r\n";
			str += BUFF_1kb;
			classSendFile->GetDlgItem(IDC_EDIT1)->SetWindowTextW(str);
			if (str.GetLength() >= 521000)
				str = " ";
		}
		send(Client, BUFF_64byte, sizeof(BUFF_64byte), 0);
		proCtrlenMax += proCtrlen;
		myProCtrl2->SetPos(proCtrlenMax);
	}
}

void CSocketFile::SendFileTo1byte(long long count)
{
	CSendFile* classSendFile = (CSendFile*)param;
	CString str;
	for (__int64 i = count; i > 0; i--) {
		fread(&BUFF_1byte, 1, 1, pf);
		if (((CButton*)classSendFile->GetDlgItem(IDC_RADIO3))->GetCheck()) {
			
			classSendFile->GetDlgItem(IDC_EDIT1)->GetWindowTextW(str);
			str += "\r\n";
			str += BUFF_1kb;
			classSendFile->GetDlgItem(IDC_EDIT1)->SetWindowTextW(str);
			if (str.GetLength() >= 521000)
				str = " ";
		}
		send(Client, &BUFF_1byte, sizeof(char), 0);
		proCtrlenMax += proCtrlen;
		myProCtrl2->SetPos(proCtrlenMax);
	}
}
