// CrecvFile.cpp: 实现文件
//

#include "pch.h"
#include "FileSocket.h"
#include "afxdialogex.h"
#include "CrecvFile.h"
#include "resource.h"
// CrecvFile 对话框

IMPLEMENT_DYNAMIC(CrecvFile, CDialogEx)

CrecvFile::CrecvFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVICE, pParent)
{

}

CrecvFile::~CrecvFile()
{
}

BOOL CrecvFile::OnInitDialog()
{
	CheckRadioButton(IDC_RADIO2, IDC_RADIO3, IDC_RADIO2);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);//打开连接按钮
	GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"未连接");
	GetDlgItem(IDC_EDIT3)->SetWindowTextW(L"9090");

	Thread = AfxBeginThread(
		sendNameThread,
		this,
		0,
		0,
		0
	);
	return 0;
}

void CrecvFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CrecvFile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CrecvFile::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CrecvFile::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO3, &CrecvFile::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO2, &CrecvFile::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CrecvFile 消息处理程序


void CrecvFile::OnBnClickedButton2()//点击连接
{
	if (!GetDlgItem(IDC_EDIT4)->GetWindowTextLengthW())
	{
		MessageBox(L"请选择文件保存的路径", L"提示", MB_ICONINFORMATION | MB_OK);
		OnBnClickedButton1();
	}	
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	SetTimer(1, 500, NULL);
	GetDlgItem(IDC_EDIT3)->GetWindowTextW(Port);
	Thread->ResumeThread();//解除
}

void CrecvFile::OnBnClickedButton1()//文件目录
{
	loop:
	//选择目录
	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc) == NOERROR) {
		BROWSEINFO   bi;
		WCHAR pszBuffer[MAX_PATH];
		LPITEMIDLIST   pidl;
		bi.hwndOwner = GetSafeHwnd();
		bi.pidlRoot = NULL;
		bi.pszDisplayName = pszBuffer;
		bi.lpszTitle = _T("⚠必须选择文件点击确定");
		bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
		bi.lpfn = NULL;
		bi.lParam = 0;
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)
		{
			if (::SHGetPathFromIDListW(pidl, pszBuffer))
			{
				filePath = pszBuffer;
				SetDlgItemText(IDC_EDIT4, filePath);
			}
			pMalloc->Free(pidl);
		}
		pMalloc->Release();
	}
	if (!filePath.GetLength()) {
		goto loop;
	}
	if (MessageBox(L"确定吗？", L"提示", MB_YESNO) == IDNO) {
		goto loop;
	}
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);//关闭自己
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);//打开连接按钮
	GetDlgItem(IDC_EDIT3)->GetWindowTextW(Port);	
}

UINT CrecvFile::sendNameThread(void* param)
{
	CrecvFile* recv = (CrecvFile*)param;
	CSocketFile* server = new CSocketFile(recv);
	server->GetOWnIp(recv);
	recv->Thread->SuspendThread();//挂起
	bool nRet;
	nRet = server->CreateServer();
	nRet = server->BindOwnIp(recv->Port);
	nRet = server->LisentClient();
	nRet = server->RecvFileName();
	nRet = server->RecvFileSize();
	nRet = server->RecvFileData();
	if (nRet == false)
	{
		recv->MessageBox(L"出错!", L"警告", MB_ICONINFORMATION | MB_OK);
	}
	return 0;
}


void CrecvFile::OnTimer(UINT_PTR nIDEvent)
{
	static int idx = 0;
	if (nIDEvent == 1)
	{
		switch (idx)
		{
		case 0:
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
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"接收中");
			break;
		case 1:
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"接收中.");
			break;
		case 2:
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"接收中..");
			break;
		case 3:
			GetDlgItem(IDC_EDIT6)->SetWindowTextW(L"接收中...");
			idx = -1;
			break;
		}
		idx++;
	}
	CDialogEx::OnTimer(nIDEvent);
}



void CrecvFile::OnBnClickedRadio3()
{
	if (MessageBox(L"可能会出现卡顿,传输变慢的情况", L"提示", MB_YESNO) == IDNO) {
		OnBnClickedRadio2();
	}
}


void CrecvFile::OnBnClickedRadio2()
{
	CheckRadioButton(IDC_RADIO2, IDC_RADIO3, IDC_RADIO2);
}
