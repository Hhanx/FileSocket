// Cziliao.cpp: 实现文件
//

#include "pch.h"
#include "FileSocket.h"
#include "afxdialogex.h"
#include "Cziliao.h"


// Cziliao 对话框

IMPLEMENT_DYNAMIC(Cziliao, CDialogEx)

Cziliao::Cziliao(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ZILIAO, pParent)
{

}

Cziliao::~Cziliao()
{
}

BOOL Cziliao::OnInitDialog()
{
	
	return 0;
}

void Cziliao::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Cziliao, CDialogEx)
END_MESSAGE_MAP()


// Cziliao 消息处理程序
