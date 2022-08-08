// CService.cpp: 实现文件
//

#include "pch.h"
#include "FileSocket.h"
#include "afxdialogex.h"
#include "CService.h"


// CService 对话框

IMPLEMENT_DYNAMIC(CService, CDialogEx)

CService::CService(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT, pParent)
{

}

CService::~CService()
{
}

void CService::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CService, CDialogEx)
END_MESSAGE_MAP()


// CService 消息处理程序
