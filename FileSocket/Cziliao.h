#pragma once
#include "afxdialogex.h"

// Cziliao 对话框

class Cziliao : public CDialogEx
{
	DECLARE_DYNAMIC(Cziliao)

public:
	Cziliao(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Cziliao();
	BOOL OnInitDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZILIAO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
};
