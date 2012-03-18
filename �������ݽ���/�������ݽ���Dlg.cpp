// �������ݽ���Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "�������ݽ���.h"
#include "�������ݽ���Dlg.h"

#include "regread.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-------------------------�������ݴ�����
pStock_Init Stock_Init=0;
pStock_Quit Stock_Quit=0;
pSetupReceiver SetupReceiver=0;
pGetStockDrvInfo GetStockDrvInfo=0;



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// C�������ݽ���Dlg �Ի���




C�������ݽ���Dlg::C�������ݽ���Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(C�������ݽ���Dlg::IDD, pParent)
	,m_pCurDataBuffer(NULL)
	,m_pPreDataBuffer(NULL)
	,m_iUsedCount(0)
	,m_bStockManageDlgVisable(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

C�������ݽ���Dlg::~C�������ݽ���Dlg()

{
	if(m_pCurDataBuffer)
	{
		delete[] m_pCurDataBuffer;
	}
	if(m_pPreDataBuffer)
	{
		delete[] m_pPreDataBuffer;
	}
	
	m_hIndexFile.Close();
	m_hLogFile.Close();
	m_hDataFile.Close();
}


void C�������ݽ���Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_edtIP);
	DDX_Control(pDX, IDC_EDIT1, m_edtUID);
	DDX_Control(pDX, IDC_LIST1, m_lstLog);
	DDX_Control(pDX, IDC_EDIT_PWD, m_edtPsw);
	DDX_Control(pDX, IDC_EDIT4, m_edtDataBase);
}

BEGIN_MESSAGE_MAP(C�������ݽ���Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(STKDATA,&C�������ݽ���Dlg::OnStkReport)
	ON_MESSAGE(WM_TIMER,&C�������ݽ���Dlg::OnTimer)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &C�������ݽ���Dlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &C�������ݽ���Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// C�������ݽ���Dlg ��Ϣ�������

BOOL C�������ݽ���Dlg::OnInitDialog()
{
	char szCodeBuf[30]={0};
	//--------------------ϵͳ�Զ�����
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	//-------------------------------------- ���뻺����
	m_pCurDataBuffer=new RCV_REPORT_STRUCTExV3[3000];
	if(m_pCurDataBuffer==NULL)
	{
		AfxMessageBox("�޷���ȡ�㹻����");
		exit(0);
	}
	memset(m_pCurDataBuffer,0,3000*sizeof(RCV_REPORT_STRUCTExV3));
	m_pPreDataBuffer=new RCV_REPORT_STRUCTExV3[3000];
	if(m_pPreDataBuffer==NULL)
	{
		AfxMessageBox("�޷���ȡ�㹻����");
		exit(0);
	}
	memset(m_pPreDataBuffer,0,3000*sizeof(RCV_REPORT_STRUCTExV3));
	//-------------------------------------��ȡ��ִ���ļ�Ŀ¼
	CString   strPath=AfxGetApp()->m_pszHelpFilePath;   
	CString   str=AfxGetApp()->m_pszExeName;   
	strPath=strPath.Left(strPath.GetLength()-str.GetLength()-4);  

	//-------------------------------------�������ļ�
	
	BOOL bRet;
	if(!m_hDataFile.Open(strPath+"data.dat",CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyWrite) ||
	!m_hIndexFile.Open(strPath+"index.dat",CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyWrite) ||
	!m_hLogFile.Open(strPath+"log.txt",CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyWrite))
	{
		AfxMessageBox("�޷���������ļ�");
		exit(0);
	}
	
	//---------------------------------------��ʼ������Ŀ¼

	int i=0;
	
	FILE* pfile=fopen(strPath+"codes.csv","r");
	if(NULL==pfile)
	{
		AfxMessageBox("�޷���򿪴����ļ�");
		exit(0);
	}
	do
	{
		
		memset(szCodeBuf,0,30);
		fgets(szCodeBuf,30,pfile);
		if(strlen(szCodeBuf)-1>=30)
			continue;
		szCodeBuf[strlen(szCodeBuf)-1]=0;
		m_mapStockData.insert(make_pair(string(szCodeBuf),i));
		i++;
	}while(!feof(pfile));
	fclose(pfile);
	m_iUsedCount=i-1;

//-------------------------------------------��ȡ����
	CStdioFile fileConfig;
	CString strFileConfigPath=strPath+"config.txt";
	BOOL bIsOpen=fileConfig.Open(strFileConfigPath,CStdioFile::modeRead);
	if(!bIsOpen)
	{
		AfxMessageBox("�����޷��������ļ�");
		exit(0);
	}
	fileConfig.ReadString(m_strServIP);
	fileConfig.ReadString(m_strDatabaseName);
	fileConfig.ReadString(m_strUID);
	fileConfig.ReadString(m_strPWD);
	m_edtIP.SetWindowText(m_strServIP);
	m_edtDataBase.SetWindowText(m_strDatabaseName);
	m_edtPsw.SetWindowTextA(m_strPWD);
	m_edtUID.SetWindowTextA(m_strUID);
	fileConfig.Close();


//--------------------------------------------�������ݿ�
	CDBInterface::InitDBInterface(m_strServIP,m_strDatabaseName,m_strUID,m_strPWD);
		



	//-----------------------------------------��ʼ���������ӿ�
	
	// ��ȡ��װ·��
	CString strDllRegPath=ReadRegisteTable(_T("HKEY_LOCAL_MACHINE"),_T("SOFTWARE\\StockDrv"),_T("Driver"));
	// ���ض�̬��
	HINSTANCE hInst=LoadLibrary(strDllRegPath);
	if(!hInst)
	{
		AfxMessageBox(_T("�޷������ӿ�"));
		exit(0);
	}

	// ��ȡ��������
	if(0==(Stock_Init=(pStock_Init)GetProcAddress(hInst,"Stock_Init") )||
	0==(Stock_Quit= (pStock_Quit)GetProcAddress(hInst,"Stock_Quit")) ||
	0==(SetupReceiver=(pSetupReceiver)GetProcAddress(hInst,"SetupReceiver") )||
	0==(GetStockDrvInfo= (pGetStockDrvInfo)GetProcAddress(hInst,"GetStockDrvInfo")))
	{
		AfxMessageBox(_T("�޷����غ���"));
		exit(0);
	}
	// ��ʼ����
	Stock_Init(this->m_hWnd,STKDATA,RCV_WORK_SENDMSG);	
	

	//------------------------------------------��ʼ����ʱ��
	SetTimer(1,30000,NULL);

	return FALSE;  
}

void C�������ݽ���Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
LRESULT C�������ݽ���Dlg::OnTimer(WPARAM wParam,LPARAM lParam)
{
	COleDateTime  tTime;
	COleDateTime  tTime1,tTime2,tTime3,tTime4;
	CString strTime;
	double fHighVol=0;
	double fLowVol=0;
	int iHighAcculate=0;
	int iLowAcculate=0;

	//��ʼ����ǰʱ�������
	tTime=COleDateTime::GetCurrentTime();

	tTime1.SetDateTime(tTime.GetYear(),tTime.GetMonth(),tTime.GetDay(),9,30,0);
	tTime2.SetDateTime(tTime.GetYear(),tTime.GetMonth(),tTime.GetDay(),11,30,0);
	tTime3.SetDateTime(tTime.GetYear(),tTime.GetMonth(),tTime.GetDay(),13,0,0);
	tTime4.SetDateTime(tTime.GetYear(),tTime.GetMonth(),tTime.GetDay(),15,0,0);
	//�ǽ���ʱ�䣬�˳�
	if(!(tTime.GetDayOfWeek()>=2&&tTime.GetDayOfWeek()<=6))
	{
		return 1;
	}
	if(!((tTime>=tTime1&&tTime<=tTime2)||(tTime>=tTime3&&tTime<=tTime4)))
	{
		return 1;
	}

	// ��ԭʼ����д���ļ�
	m_hDataFile.Write(m_pCurDataBuffer,this->m_iUsedCount*sizeof(RCV_REPORT_STRUCTExV3));
	m_hDataFile.Flush();

	// ͳ���¸ߣ��µͣ��ɽ���
	for(int i=0;i<m_iUsedCount;i++)
	{
		if(m_pPreDataBuffer[i].m_fNewPrice==0 || m_pPreDataBuffer[i].m_fVolume==0)
		{
			continue;
		}
		if(m_pCurDataBuffer[i].m_fNewPrice>m_pPreDataBuffer[i].m_fHigh)
		{
			iHighAcculate++;
			fHighVol+=m_pCurDataBuffer[i].m_fVolume-m_pPreDataBuffer[i].m_fVolume;
		}
		if(m_pCurDataBuffer[i].m_fNewPrice<m_pPreDataBuffer[i].m_fLow)
		{
			iLowAcculate++;
			fLowVol+=m_pCurDataBuffer[i].m_fVolume-m_pPreDataBuffer[i].m_fVolume;
		}
	}

	// �����ݸ��Ǿ�����
	memcpy(m_pPreDataBuffer,m_pCurDataBuffer,m_iUsedCount*sizeof(RCV_REPORT_STRUCTExV3));
	
	// ��ȡϵͳʱ��
	strTime.Format("%d-%d-%d %d:%d:%d"
											,tTime.GetYear(),
											tTime.GetMonth(),
											tTime.GetDay(),
											tTime.GetHour(),
											tTime.GetMinute(),
											tTime.GetSecond());


	// �������ݵ����ݿ�
	m_pRecordset->Open("select TOP 1 * from allcount",m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	m_pRecordset->AddNew();
	m_pRecordset->PutCollect("sumhighvol",fHighVol);
	m_pRecordset->PutCollect("sumlowvol",fLowVol);
	m_pRecordset->PutCollect("highaculate",iHighAcculate);
	m_pRecordset->PutCollect("lowaculate",iLowAcculate);
	m_pRecordset->PutCollect("date",(_bstr_t)strTime);
	m_pRecordset->Update();
	m_pRecordset->Close();
	//m_pCommand->Parameters->GetItem(0)->get_Value
	m_pCommand->GetPrepared();
	// ���½���
	m_lstLog.InsertString(0,CString("[")+strTime+"]����ɹ�");
	return 1;
}
LRESULT C�������ݽ���Dlg::OnStkReport(WPARAM wParam,LPARAM lParam)
{
	RCV_DATA * pHeader;
	pHeader = (RCV_DATA *)lParam;
	char szCode[10]={0};
	CString strLabel;
	CString strLogLine="";
	CString strTime;
	CTime tTime;
	switch( wParam)
	{
	
	case RCV_REPORT:					//��Ʊ����
		strLogLine="";
		
		tTime=CTime::GetCurrentTime();
		strTime.Format("%d-%d-%d %d:%d:%d"
											,tTime.GetYear(),
											tTime.GetMonth(),
											tTime.GetDay(),
											tTime.GetHour(),
											tTime.GetMinute(),
											tTime.GetSecond());
		strLogLine+=strTime+"  :";
		for(int i=0; i<pHeader->m_nPacketNum; i++)
		{
			char szMarket[3]={0};
			memcpy(szMarket,&(pHeader->m_pReportV3[i].m_wMarket),2);
		
			if(!strcmp(szMarket,"SH"))
			{
				strLabel=CString("sh")+pHeader->m_pReportV3[i].m_szLabel;
			}
			else		if(!strcmp(szMarket,"SZ"))
			{
				strLabel=CString("sz")+pHeader->m_pReportV3[i].m_szLabel;
			}
			else
			{
				continue;
			}
			sprintf(szCode,"%s",strLabel);			
			//------------------------------- �ù�Ʊ����
			if(m_mapStockData.find(szCode)!=m_mapStockData.end())
			{
				memcpy(m_mapStockData[szCode]+m_pCurDataBuffer,&(pHeader->m_pReportV3[i]),sizeof(RCV_REPORT_STRUCTExV3));
				strLogLine+=CString(szCode)+",";
			}
			memset(szCode,0,10);
			
		
		}
		strLogLine+="\n";
		this->m_hLogFile.Write(strLogLine,strLogLine.GetLength());
		this->m_hLogFile.Flush();
		break; 

	case RCV_FILEDATA:					//�ļ�
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:				// ����������
			break;
		case FILE_MINUTE_EX:				// ������������
			break;
		case FILE_5MINUTE_EX:				// ��5����������
			break;
		case FILE_POWER_EX:				// �����Ȩ����
			break;
		case FILE_BASE_EX:				// ���������ļ�,m_szFileName�������ļ���
			break;
		case FILE_NEWS_EX:				// ������,��������m_szFileName����Ŀ¼������
			break;
		}
		break;
	case RCV_FENBIDATA:
		{
	
		}
		break;
	default:
		return 0;					// unknown data
	}
return 1;
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C�������ݽ���Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR C�������ݽ���Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void C�������ݽ���Dlg::OnBnClickedButton1()
{
	CString   strPath=AfxGetApp()->m_pszHelpFilePath;   
	CString   str=AfxGetApp()->m_pszExeName;   
	strPath=strPath.Left(strPath.GetLength()-str.GetLength()-4);  
	CStdioFile fileConfig;
	CString strFileConfigPath=strPath+"config.txt";
	BOOL bIsOpen=fileConfig.Open(strFileConfigPath,CStdioFile::modeWrite | CStdioFile::modeCreate);
	if(!bIsOpen)
	{
		AfxMessageBox("�����޷��������ļ�");
		exit(0);
	}
	m_edtIP.GetWindowText(m_strServIP);
	m_edtDataBase.GetWindowText(m_strDatabaseName);
	m_edtPsw.GetWindowTextA(m_strPWD);
	m_edtUID.GetWindowTextA(m_strUID);
	fileConfig.WriteString(m_strServIP+"\n");
	fileConfig.WriteString(m_strDatabaseName+"\n");
	fileConfig.WriteString(m_strUID+"\n");
	fileConfig.WriteString(m_strPWD+"\n");
	fileConfig.Flush();
	fileConfig.Close();
	//	SetupReceiver(true);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void C�������ݽ���Dlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Stock_Quit(this->m_hWnd);
	CDialog::OnClose();
}


void C�������ݽ���Dlg::OnBnClickedButton2()
{
	m_stockManageDlg.DoModal();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
