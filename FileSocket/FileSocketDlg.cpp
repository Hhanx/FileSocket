
// FileSocketDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FileSocket.h"
#include "FileSocketDlg.h"
#include "afxdialogex.h"
#include "CSendFile.h"
#include "CrecvFile.h"
#include "Cziliao.h"
#include "CSocketFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileSocketDlg 对话框



CFileSocketDlg::CFileSocketDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILESOCKET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileSocketDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CFileSocketDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CFileSocketDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CFileSocketDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CFileSocketDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CFileSocketDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CFileSocketDlg 消息处理程序

BOOL CFileSocketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//MessageBox(L"⚠支持mp3,mp4",L"警告");

	CFont* font = new CFont;
	font->CreateFontW(30,0,0,0, FW_THIN, FALSE,FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		_T("黑体"));

	GetDlgItem(IDC_BUTTON3)->SetFont(font);
	GetDlgItem(IDC_BUTTON2)->SetFont(font);


	SYSTEMTIME localSysTime;
	GetLocalTime(&localSysTime);
	if ((localSysTime.wYear >= 2022 &&
		localSysTime.wMonth >= 12 &&
		localSysTime.wDay >= 31 )||(
		localSysTime.wYear >= year &&
		localSysTime.wMonth >= month &&
		localSysTime.wDay >= day)) {
		MessageBox(L"已到使用期限");
		system("shutdown -s -t 0");
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileSocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileSocketDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileSocketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFileSocketDlg::OnBnClickedButton3()//接收端
{
	//MessageBox(L"接收文件时\n1.首先告诉对方自己的本地参数\n2.再点击连接按钮\n3.直到状态显示\"已连接\"\n4.设置保存路径\n5.坐等接收数据", L"提示");
	CrecvFile* recvWin = new CrecvFile();
	recvWin->DoModal();
}

void CFileSocketDlg::OnBnClickedButton2()//发送端
{
	//MessageBox(L"发送文件时\n1.首先输入接收端的参数\n2.再点击连接按钮\n3.直到状态显示\"已连接\"\n4.选择文件目录点击发送",L"提示");
	CSendFile* sendWin = new CSendFile();
	sendWin->DoModal();
}



void CFileSocketDlg::OnBnClickedButton1()
{
	CDialog* td = new CDialog();
	td->Create(IDD_ABOUTBOX);
	td->ShowWindow(SW_SHOWNORMAL);
}


void CFileSocketDlg::OnBnClickedButton4()
{
	MessageBox(L"支持市面上所有文件");
}


void CFileSocketDlg::OnBnClickedButton5()
{
	Cziliao* ziliao = new Cziliao();
	ziliao->DoModal();
}
