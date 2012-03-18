// �������ݽ���Dlg.h : ͷ�ļ�
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
// C�������ݽ���Dlg �Ի���



class C�������ݽ���Dlg : public CDialog
{
// ����
public:
	C�������ݽ���Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	~C�������ݽ���Dlg();
// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

//------------------------------------------ ���Լ�����ı���
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
//------------------------------------------���Լ��������Ϣӳ�亯��
	LRESULT OnStkReport(WPARAM wParam,LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam,LPARAM lParam);
//----------------------------------------- ���ɵ���Ϣӳ�亯��
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

