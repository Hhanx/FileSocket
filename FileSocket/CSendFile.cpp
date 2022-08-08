// CSendFile.cpp: 实现文件
//

#include "pch.h"
#include "FileSocket.h"
#include "afxdialogex.h"
#include "CSendFile.h"

// CSendFile 对话框

IMPLEMENT_DYNAMIC(CSendFile, CDialogEx)

CSendFile::CSendFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT, pParent)
{
	
}

CSendFile::~CSendFile()
{
	delete(this);
}

BOOL CSendFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CheckRadioButton(IDC_RADIO2, IDC_RADIO3, IDC_RADIO2);
	GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"未连接");
	GetDlgItem(IDC_BUTTON3)->SetWindowTextW(L"未连接暂不开放");
	GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"9090");
	GetDlgItem(IDC_IPADDRESS2)->SetWindowTextW(L"192.168.");	
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	m_combobox.AddString(L"500kb/s");
	m_combobox.AddString(L"1kb/s");
	m_combobox.AddString(L"64byte/s");
	m_combobox.AddString(L"1byte/s");
	GetDlgItem(IDC_COMBO1)->SetWindowTextW(L"500kb/s");
	return 0;
}

void CSendFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combobox);
}

BEGIN_MESSAGE_MAP(CSendFile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CSendFile::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CSendFile::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CSendFile::OnBnClickedButton3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO3, &CSendFile::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO2, &CSendFile::OnBnClickedRadio2)
END_MESSAGE_MAP()

// CSendFile 消息处理程序

void CSendFile::OnBnClickedButton2()//连接
{
	GetDlgItem(IDC_IPADDRESS2)->GetWindowTextW(IP);
	GetDlgItem(IDC_EDIT3)->GetWindowTextW(port);
	if (IP.Left(8) != L"192.168." || IP.Right(2) == ".0" || port.GetAllocLength() == 0) {
		MessageBox(L"请输入IP地址参数", L"警告", MB_ICONINFORMATION | MB_OK);
		return;
	}
	SetTimer(1, 500, NULL);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	Thread = AfxBeginThread(
		sendNameThread,
		this,
		0,
		0,
		0
	);
}

void CSendFile::OnBnClickedButton1()//打开文件选择文件
{
	
loop:
	GetDlgItem(IDC_COMBO1)->GetWindowTextW(sendSpeedstr);
	if(sendSpeedstr == L"500kb/s") {
		sendSpeed = 1;
	}else if (sendSpeedstr == L"1kb/s") {
		sendSpeed = 2; 
	}else if (sendSpeedstr == L"64byte/s") {
		sendSpeed = 3;
	}else if (sendSpeedstr == L"1byte/s") {
		sendSpeed = 4;
	}

	CFileDialog* WinFile = new CFileDialog(
		TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		NULL,
		this,
		0,
		TRUE
	);
	WinFile->DoModal();

	pathName = WinFile->GetPathName();//完整路径
	fileName = WinFile->GetFileName();//文件名

	if (fileName.GetLength() == 0 || 
		fileName.GetLength() == 0 || 
		MessageBox(L"确定吗?", L"提示", MB_YESNO) == IDNO) {
		goto loop;
	}
	GetDlgItem(IDC_COMBO1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT4)->SetWindowTextW(pathName);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
}

void CSendFile::OnBnClickedButton3() //发送数据函数
{
	if (fileName.GetLength() == 0)
	{
		MessageBox(L"请选择你要发送的文件!");
		OnBnClickedButton1();
	}
	if (pathName.GetLength() == 0)
	{
		MessageBox(L"发送失败！");
		delete(this);
	}
	this->Thread->ResumeThread();
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
}

void CSendFile::OnTimer(UINT_PTR nIDEvent)
{
	static int idx = 0;
	if (nIDEvent == 1)
	{
		switch (idx)
		{
		case 0 :
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"连接中");
			break;
		case 1:
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"连接中.");
			break;
		case 2:
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"连接中..");
			break;
		case 3:
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"连接中...");
			idx = -1;
			break;
		}
		idx++;
	}
	if (nIDEvent == 2)
	{
		switch (idx)
		{
		case 0:
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"发送中");
			break;
		case 1:
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"发送中.");
			break;
		case 2:
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"发送中..");
			break;
		case 3:
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"发送中...");
			idx = -1;
			break;
		}
		idx++;
	}
	CDialogEx::OnTimer(nIDEvent);
}

UINT CSendFile::sendNameThread(void* param)
{
	CSendFile* classSendFile = (CSendFile*)param;
	bool nRet;
	CSocketFile* client = new CSocketFile(classSendFile);
	client->CreateClient();
	client->SetServer(classSendFile->IP, classSendFile->port);
	nRet = client->ConnectServer();//循环中
	if (nRet == true)
	{
		//连接成功以后
		//1.定时器1关掉 
		//2.把输入框设置为"连接成功"
		classSendFile->KillTimer(1);
		classSendFile->GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"连接成功");
		classSendFile->GetDlgItem(IDC_BUTTON3)->EnableWindow(true);//打开按钮
		classSendFile->GetDlgItem(IDC_BUTTON3)->SetWindowTextW(L"发送数据");
		classSendFile->Thread->SuspendThread();//停止
		classSendFile->SetTimer(2, 500, NULL);
		client->SendName(classSendFile->fileName);
		Sleep(1500);
	}
	nRet = client->SendFile(classSendFile->pathName);
	return 0;
}

void CSendFile::OnBnClickedRadio3()
{
	if (MessageBox(L"可能会出现卡顿,传输变慢的情况", L"提示", MB_YESNO) == IDNO) {
		OnBnClickedRadio2();
	}
}

void CSendFile::OnBnClickedRadio2()
{
	CheckRadioButton(IDC_RADIO2, IDC_RADIO3, IDC_RADIO2);
}


