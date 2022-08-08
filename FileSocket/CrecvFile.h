#pragma once
#include "afxdialogex.h"
#include "CSocketFile.h"

// CrecvFile 对话框

class CrecvFile : public CDialogEx
{
	DECLARE_DYNAMIC(CrecvFile)

public:
	CrecvFile(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CrecvFile();
	BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVICE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	CString Port;//端口
	CWinThread* Thread;//声明线程
	static UINT sendNameThread(void* param);//声明线程函数
	CString filePath;
	long long poslen;//每次移动的距离
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio2();
};
