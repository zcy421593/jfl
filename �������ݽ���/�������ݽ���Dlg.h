// 银江数据接收Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <map>
#include <string>
#include "StockManageDlg.h"
#include "stockdrv.h"
#include "DBInterface.h"
#import "C:\Program Files\common files\system\ado\msado15.dll"	no_namespace rename("EOF","adoEOF")

#define STKDATA (WM_USER+124)
#define WM_TIMERUser (WM_USER+125)
// C银江数据接收Dlg 对话框



class C银江数据接收Dlg : public CDialog
{
// 构造
public:
	C银江数据接收Dlg(CWnd* pParent = NULL);	// 标准构造函数
	~C银江数据接收Dlg();
// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

//------------------------------------------ 我自己定义的变量
	_ConnectionPtr m_pConnection;
     _RecordsetPtr m_pRecordset;
	_CommandPtr m_pCommand;
	std::map<std::string,int> m_mapStockData;
	RCV_REPORT_STRUCTExV3* m_pCurDataBuffer;
	RCV_REPORT_STRUCTExV3* m_pPreDataBuffer;
	int m_iUsedCount;
	CFile m_hLogFile;
	CFile m_hDataFile;
	CFile m_hIndexFile;
	CStockManageDlg m_stockManageDlg;
	BOOL m_bStockManageDlgVisable;
//------------------------------------------我自己定义的消息映射函数
	LRESULT OnStkReport(WPARAM wParam,LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam,LPARAM lParam);
//----------------------------------------- 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	
public:
	afx_msg void OnClose();
	CEdit m_edtIP;
	CEdit m_edtUID;
	CEdit m_edtPsw;
	CEdit m_edtDataBase;

	CListBox m_lstLog;

	CString m_strServIP;
	CString m_strDatabaseName;
	int m_iInterVal;
	CString m_strUID;
	CString m_strPWD;

	afx_msg void OnBnClickedButton2();
};

