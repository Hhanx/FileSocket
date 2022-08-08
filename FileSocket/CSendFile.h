#pragma once
#include "afxdialogex.h"
#include "CSocketFile.h"
// CSendFile 对话框

class CSendFile : public CDialogEx
{
	DECLARE_DYNAMIC(CSendFile)

public:
	CSendFile(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSendFile();
	BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
public:
	CString IP;//IP号
	CString port;//端口号
	CString pathName;//文件路径
	CString fileName;//文件名

	afx_msg void OnBnClickedButton1();//选择路径
	afx_msg void OnBnClickedButton2();//连接
	afx_msg void OnBnClickedButton3();//发送
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	char ch = 'S';
	CWinThread* Thread;//声明线程
	static UINT sendNameThread(void* param);//声明线程函数
	int sendSpeed;//传输速度
	int poslen;//tmp
	CString sendSpeedstr;
	CComboBox m_combobox;
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio2();
};
