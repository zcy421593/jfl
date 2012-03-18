// StockManage.cpp : 实现文件
//

#include "stdafx.h"
#include "银江数据接收.h"
#include "StockManageDlg.h"
#include "afxdialogex.h"
#include "AddStockDlg.h"
#include "DBInterface.h"
// CStockManage 对话框

IMPLEMENT_DYNAMIC(CStockManageDlg, CDialog)

CStockManageDlg::CStockManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStockManageDlg::IDD, pParent)
{

}

CStockManageDlg::~CStockManageDlg()
{
}

void CStockManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstAllStock);
	DDX_Control(pDX, IDC_LIST3, m_lstAllLine);
	DDX_Control(pDX, IDC_BUTTON1, m_btnAddStock);
	DDX_Control(pDX, IDC_BUTTON4, m_btnDeleteStock);
	DDX_Control(pDX, IDC_BUTTON3, m_btnMountStock);
	DDX_Control(pDX, IDC_BUTTON5, m_btnUnmountStock);
}


BEGIN_MESSAGE_MAP(CStockManageDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON8, &CStockManageDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON6, &CStockManageDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON1, &CStockManageDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CStockManageDlg::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON4, &CStockManageDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

BOOL CStockManageDlg::OnInitDialog()
{
		CDialog::OnInitDialog();
		m_lstAllStock.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
		m_lstAllLine.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
		m_lstAllStock.InsertColumn(0,"指标名称",0,100);
		m_lstAllStock.InsertColumn(1,"是否挂载",0,100);
		m_lstAllStock.InsertColumn(2,"dll路径",0,200);
		m_lstAllLine.InsertColumn(0,"名称",0,100);
		m_lstAllLine.InsertColumn(1,"颜色",0,100);
		m_lstAllLine.InsertColumn(2,"线宽",0,100);
		UpdateAllStocks();
		return TRUE;
}


// CStockManage 消息处理程序


void CStockManageDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CStockManageDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CStockManageDlg::OnBnClickedButton1()
{
	CAddStockDlg addStockDlg;
	if(IDOK==addStockDlg.DoModal())
	{
		CString strErr;
		if(!CDBInterface::GetDBInterface()->AddStockInfo(addStockDlg.m_strStockName,addStockDlg.m_strDllPath,addStockDlg.m_vecLines,strErr))
		{
			AfxMessageBox(strErr);
			return;
		}
		else
		{
			UpdateAllStocks();
		}
	}
	// TODO: 在此添加控件通知处理程序代码
}

void CStockManageDlg::UpdateAllStocks()
{
	CDBInterface::GetDBInterface()->GetAllStocks(m_vecStockInfo);
	m_lstAllStock.DeleteAllItems();
	for(int i=0;i<m_vecStockInfo.size();i++)
	{
		
		m_lstAllStock.InsertItem(m_lstAllStock.GetItemCount(),"");
		m_lstAllStock.SetItemText(i,0,m_vecStockInfo[i].m_strStockName);
		m_lstAllStock.SetItemText(i,1,m_vecStockInfo[i].m_bIsLoad?"是":"否");
		m_lstAllStock.SetItemText(i,2,m_vecStockInfo[i].m_strDllPath);
	}
}


void CStockManageDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int i=pNMItemActivate->iItem;
	if(i==-1||i>=m_lstAllStock.GetItemCount())return;
	UpdateAllLines(m_vecStockInfo[i].m_strStockName);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CStockManageDlg::UpdateAllLines(CString strStockName)
{
	CDBInterface::GetDBInterface()->GetStockLines(strStockName,m_vecLineInfo);
	m_lstAllLine.DeleteAllItems();

	for(int i=0;i<m_vecLineInfo.size();i++)
	{
		CString strLineWidth,strColor;
		strColor.Format("%d",m_vecLineInfo[i].m_dwColor);
		strLineWidth.Format("%d",m_vecLineInfo[i].m_iWidth);
		int iItem=m_lstAllLine.InsertItem(m_lstAllLine.GetItemCount(),"");
		m_lstAllLine.SetItemText(iItem,0,m_vecLineInfo[i].m_strName);
		m_lstAllLine.SetItemText(iItem,1,strLineWidth);
		m_lstAllLine.SetItemText(iItem,2,strColor);
	}
}


void CStockManageDlg::OnBnClickedButton4()
{
	
	POSITION pos=m_lstAllStock.GetFirstSelectedItemPosition();
	int i=m_lstAllStock.GetNextSelectedItem(pos);
	while(i!=-1)
	{
		CDBInterface::GetDBInterface()->DeleteStock(m_vecStockInfo[i].m_strStockName);
		i=m_lstAllStock.GetNextSelectedItem(pos);
	}
	UpdateAllStocks();
	// TODO: 在此添加控件通知处理程序代码
}
