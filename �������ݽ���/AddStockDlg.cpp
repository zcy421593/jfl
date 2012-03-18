// AddStockDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "银江数据接收.h"
#include "AddStockDlg.h"
#include "afxdialogex.h"
#include "windows.h"
#include "Common.h"
// CAddStockDlg 对话框

IMPLEMENT_DYNAMIC(CAddStockDlg, CDialog)
	CString GetCurrentPath()
{
	TCHAR szPath[260]={0};
	GetModuleFileName(NULL,szPath,260);
	CString strModulePath=szPath;
	return strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
}
CAddStockDlg::CAddStockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddStockDlg::IDD, pParent)
{

}

CAddStockDlg::~CAddStockDlg()
{
}

void CAddStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_lstAllLine);
	DDX_Control(pDX, IDC_EDIT1, m_edtStockName);
	DDX_Control(pDX, IDC_EDIT2, m_edtLineName);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, m_cboColor);
	DDX_Control(pDX, IDC_BUTTON7, m_btnDelete);
	DDX_Control(pDX, IDC_COMBO2, m_cboLineWidth);
	//	DDX_Control(pDX, IDC_EDIT3, m_edtDllPath);
	DDX_Control(pDX, IDC_COMBO3, m_cboDlls);
}


BEGIN_MESSAGE_MAP(CAddStockDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON7, &CAddStockDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON2, &CAddStockDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CAddStockDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddStockDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAddStockDlg 消息处理程序


void CAddStockDlg::OnBnClickedButton7()
{
	POSITION pos=m_lstAllLine.GetFirstSelectedItemPosition();
	int iIndex=m_lstAllLine.GetNextSelectedItem(pos);
	if(iIndex==-1)return;
	m_vecLines.erase(m_vecLines.begin()+iIndex);
	UpdateAllLinesList();
	// TODO: 在此添加控件通知处理程序代码
}


void CAddStockDlg::OnBnClickedButton2()
{
	
	m_edtStockName.GetWindowText(m_strStockName);
	m_cboDlls.GetWindowText(m_strDllPath);
	if(m_strStockName=="")
	{
		AfxMessageBox("指标名不能为空");
		return;
	}
	if(m_vecLines.size()==0)
	{
		AfxMessageBox("至少需要一条数据线");
		return;
	}
		OnOK();
	// TODO: 在此添加控件通知处理程序代码
}


void CAddStockDlg::OnBnClickedButton6()
{
	OnCancel();
	// TODO: 在此添加控件通知处理程序代码
}

void CAddStockDlg::UpdateAllLinesList()
{
	m_lstAllLine.DeleteAllItems();
	for(int i=0;i<m_vecLines.size();i++)
	{
		CString strLineWidth;
		CString strLineColor;
		CString strDllPath;
		strLineWidth.Format("%d",m_vecLines[i].m_iWidth);
		strLineColor.Format("(%d,%d,%d)",m_vecLines[i].m_dwColor&0xff,(m_vecLines[i].m_dwColor&0xff00)>>8,(m_vecLines[i].m_dwColor&0xff0000)>>16);
		m_lstAllLine.InsertItem(i,"");
		
		m_lstAllLine.SetItemText(i,0,m_vecLines[i].m_strName);
		m_lstAllLine.SetItemText(i,1,strLineColor);
		m_lstAllLine.SetItemText(i,2,strLineWidth);
		m_lstAllLine.SetItemText(i,3,m_vecLines[i].m_strDllPath);
	}
}

BOOL CAddStockDlg::OnInitDialog()

{
	CFileFind fileFind;
	CString strFindPath;
	CDialog::OnInitDialog();
	CString strLineWidth;
	for(int i=1;i<6;i++)
	{
		strLineWidth.Format("%d",i);
		m_cboLineWidth.InsertString(i-1,strLineWidth);
	}
	m_lstAllLine.InsertColumn(0,"名称",0,100);
	m_lstAllLine.InsertColumn(1,"颜色",0,100);
	m_lstAllLine.InsertColumn(2,"线宽",0,100);
	
	m_lstAllLine.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	strFindPath=GetCurrentPath()+"\\stocks\\*.dll";
	fileFind.FindFile(strFindPath);
	while(fileFind.FindNextFile())
	{
		m_cboDlls.InsertString(m_cboDlls.GetCount(),fileFind.GetFileTitle());
	}
	m_cboDlls.SetCurSel(0);
	m_cboLineWidth.SetCurSel(0);
	return TRUE;
}



void CAddStockDlg::OnBnClickedButton1()
{
	CString strLineName,strDllPath;
	DWORD dwColor ;
	int iLineWidth;
	m_edtLineName.GetWindowText(strLineName);
	m_edtDllPath.GetWindowText(strDllPath);
	dwColor=m_cboColor.GetColor();
	iLineWidth=m_cboLineWidth.GetCurSel()+1;
	if(strLineName=="")
	{
		AfxMessageBox("名称不能为空");
		return;
	}
	CLineInfo lineInfo={strLineName,dwColor,iLineWidth,strDllPath};
	m_vecLines.push_back(lineInfo);
	UpdateAllLinesList();
	// TODO: 在此添加控件通知处理程序代码
}
