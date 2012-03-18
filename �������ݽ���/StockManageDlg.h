#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
#include "CommonDataStruct.h"
using namespace std;

// CStockManage �Ի���
struct CStockInfo;
class CStockManageDlg : public CDialog
{
	DECLARE_DYNAMIC(CStockManageDlg)

public:
	CStockManageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStockManageDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_STOCKMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstAllStock;
	CListCtrl m_lstAllLine;
	CButton m_btnAddStock;
	CButton m_btnDeleteStock;
	CButton m_btnMountStock;
	CButton m_btnUnmountStock;
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton1();
private:
	vector<CStockInfo> m_vecStockInfo;
	vector<CLineInfo> m_vecLineInfo;
	void UpdateAllStocks();
	void UpdateAllLines(CString strStockName);
public:
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton4();
};


