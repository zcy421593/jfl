#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxcolorbutton.h"
#include "resource.h"
#include <vector>
#include "CommonDataStruct.h"
using namespace std;
// CAddStockDlg 对话框
struct CLineInfo;

class CAddStockDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddStockDlg)

public:
	CAddStockDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddStockDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDSTOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstAllLine;
	CEdit m_edtStockName;
	CEdit m_edtLineName;
	CMFCColorButton m_cboColor;
	afx_msg void OnBnClickedButton7();
	CButton m_btnDelete;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton6();
	CComboBox m_cboLineWidth;

public:
	void UpdateAllLinesList();
	virtual BOOL OnInitDialog();
public:
	vector<CLineInfo> m_vecLines;
	CString m_strStockName;
	CString m_strDllPath;
	afx_msg void OnBnClickedButton1();
	CEdit m_edtDllPath;
	CComboBox m_cboDlls;
};
