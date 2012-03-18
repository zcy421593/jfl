// 银江数据接收Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "银江数据接收.h"
#include "银江数据接收Dlg.h"

#include "regread.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-------------------------声明数据处理函数
pStock_Init Stock_Init=0;
pStock_Quit Stock_Quit=0;
pSetupReceiver SetupReceiver=0;
pGetStockDrvInfo GetStockDrvInfo=0;



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// C银江数据接收Dlg 对话框




C银江数据接收Dlg::C银江数据接收Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(C银江数据接收Dlg::IDD, pParent)
	,m_pCurDataBuffer(NULL)
	,m_pPreDataBuffer(NULL)
	,m_iUsedCount(0)
	,m_bStockManageDlgVisable(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

C银江数据接收Dlg::~C银江数据接收Dlg()

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


void C银江数据接收Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_edtIP);
	DDX_Control(pDX, IDC_EDIT1, m_edtUID);
	DDX_Control(pDX, IDC_LIST1, m_lstLog);
	DDX_Control(pDX, IDC_EDIT_PWD, m_edtPsw);
	DDX_Control(pDX, IDC_EDIT4, m_edtDataBase);
}

BEGIN_MESSAGE_MAP(C银江数据接收Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(STKDATA,&C银江数据接收Dlg::OnStkReport)
	ON_MESSAGE(WM_TIMER,&C银江数据接收Dlg::OnTimer)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &C银江数据接收Dlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &C银江数据接收Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// C银江数据接收Dlg 消息处理程序

BOOL C银江数据接收Dlg::OnInitDialog()
{
	char szCodeBuf[30]={0};
	//--------------------系统自动生成
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
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	//-------------------------------------- 申请缓冲区
	m_pCurDataBuffer=new RCV_REPORT_STRUCTExV3[3000];
	if(m_pCurDataBuffer==NULL)
	{
		AfxMessageBox("无法获取足够缓冲");
		exit(0);
	}
	memset(m_pCurDataBuffer,0,3000*sizeof(RCV_REPORT_STRUCTExV3));
	m_pPreDataBuffer=new RCV_REPORT_STRUCTExV3[3000];
	if(m_pPreDataBuffer==NULL)
	{
		AfxMessageBox("无法获取足够缓冲");
		exit(0);
	}
	memset(m_pPreDataBuffer,0,3000*sizeof(RCV_REPORT_STRUCTExV3));
	//-------------------------------------获取可执行文件目录
	CString   strPath=AfxGetApp()->m_pszHelpFilePath;   
	CString   str=AfxGetApp()->m_pszExeName;   
	strPath=strPath.Left(strPath.GetLength()-str.GetLength()-4);  

	//-------------------------------------打开数据文件
	
	BOOL bRet;
	if(!m_hDataFile.Open(strPath+"data.dat",CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyWrite) ||
	!m_hIndexFile.Open(strPath+"index.dat",CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyWrite) ||
	!m_hLogFile.Open(strPath+"log.txt",CFile::modeReadWrite|CFile::modeCreate|CFile::shareDenyWrite))
	{
		AfxMessageBox("无法获打开数据文件");
		exit(0);
	}
	
	//---------------------------------------初始化代码目录

	int i=0;
	
	FILE* pfile=fopen(strPath+"codes.csv","r");
	if(NULL==pfile)
	{
		AfxMessageBox("无法获打开代码文件");
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

//-------------------------------------------读取配置
	CStdioFile fileConfig;
	CString strFileConfigPath=strPath+"config.txt";
	BOOL bIsOpen=fileConfig.Open(strFileConfigPath,CStdioFile::modeRead);
	if(!bIsOpen)
	{
		AfxMessageBox("错误！无法打开配置文件");
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


//--------------------------------------------连接数据库
	CDBInterface::InitDBInterface(m_strServIP,m_strDatabaseName,m_strUID,m_strPWD);
		



	//-----------------------------------------初始化银江链接库
	
	// 读取安装路径
	CString strDllRegPath=ReadRegisteTable(_T("HKEY_LOCAL_MACHINE"),_T("SOFTWARE\\StockDrv"),_T("Driver"));
	// 加载动态库
	HINSTANCE hInst=LoadLibrary(strDllRegPath);
	if(!hInst)
	{
		AfxMessageBox(_T("无法打开链接库"));
		exit(0);
	}

	// 读取导出函数
	if(0==(Stock_Init=(pStock_Init)GetProcAddress(hInst,"Stock_Init") )||
	0==(Stock_Quit= (pStock_Quit)GetProcAddress(hInst,"Stock_Quit")) ||
	0==(SetupReceiver=(pSetupReceiver)GetProcAddress(hInst,"SetupReceiver") )||
	0==(GetStockDrvInfo= (pGetStockDrvInfo)GetProcAddress(hInst,"GetStockDrvInfo")))
	{
		AfxMessageBox(_T("无法加载函数"));
		exit(0);
	}
	// 开始接收
	Stock_Init(this->m_hWnd,STKDATA,RCV_WORK_SENDMSG);	
	

	//------------------------------------------初始化定时器
	SetTimer(1,30000,NULL);

	return FALSE;  
}

void C银江数据接收Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
LRESULT C银江数据接收Dlg::OnTimer(WPARAM wParam,LPARAM lParam)
{
	COleDateTime  tTime;
	COleDateTime  tTime1,tTime2,tTime3,tTime4;
	CString strTime;
	double fHighVol=0;
	double fLowVol=0;
	int iHighAcculate=0;
	int iLowAcculate=0;

	//初始化当前时间与界限
	tTime=COleDateTime::GetCurrentTime();

	tTime1.SetDateTime(tTime.GetYear(),tTime.GetMonth(),tTime.GetDay(),9,30,0);
	tTime2.SetDateTime(tTime.GetYear(),tTime.GetMonth(),tTime.GetDay(),11,30,0);
	tTime3.SetDateTime(tTime.GetYear(),tTime.GetMonth(),tTime.GetDay(),13,0,0);
	tTime4.SetDateTime(tTime.GetYear(),tTime.GetMonth(),tTime.GetDay(),15,0,0);
	//非交易时间，退出
	if(!(tTime.GetDayOfWeek()>=2&&tTime.GetDayOfWeek()<=6))
	{
		return 1;
	}
	if(!((tTime>=tTime1&&tTime<=tTime2)||(tTime>=tTime3&&tTime<=tTime4)))
	{
		return 1;
	}

	// 将原始数据写入文件
	m_hDataFile.Write(m_pCurDataBuffer,this->m_iUsedCount*sizeof(RCV_REPORT_STRUCTExV3));
	m_hDataFile.Flush();

	// 统计新高，新低，成交额
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

	// 新数据覆盖旧数据
	memcpy(m_pPreDataBuffer,m_pCurDataBuffer,m_iUsedCount*sizeof(RCV_REPORT_STRUCTExV3));
	
	// 获取系统时间
	strTime.Format("%d-%d-%d %d:%d:%d"
											,tTime.GetYear(),
											tTime.GetMonth(),
											tTime.GetDay(),
											tTime.GetHour(),
											tTime.GetMinute(),
											tTime.GetSecond());


	// 更新数据到数据库
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
	// 更新界面
	m_lstLog.InsertString(0,CString("[")+strTime+"]插入成功");
	return 1;
}
LRESULT C银江数据接收Dlg::OnStkReport(WPARAM wParam,LPARAM lParam)
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
	
	case RCV_REPORT:					//股票行情
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
			//------------------------------- 该股票存在
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

	case RCV_FILEDATA:					//文件
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:				// 补日线数据
			break;
		case FILE_MINUTE_EX:				// 补分钟线数据
			break;
		case FILE_5MINUTE_EX:				// 补5分钟线数据
			break;
		case FILE_POWER_EX:				// 补充除权数据
			break;
		case FILE_BASE_EX:				// 基本资料文件,m_szFileName仅包含文件名
			break;
		case FILE_NEWS_EX:				// 新闻类,其类型由m_szFileName中子目录名来定
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
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C银江数据接收Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR C银江数据接收Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void C银江数据接收Dlg::OnBnClickedButton1()
{
	CString   strPath=AfxGetApp()->m_pszHelpFilePath;   
	CString   str=AfxGetApp()->m_pszExeName;   
	strPath=strPath.Left(strPath.GetLength()-str.GetLength()-4);  
	CStdioFile fileConfig;
	CString strFileConfigPath=strPath+"config.txt";
	BOOL bIsOpen=fileConfig.Open(strFileConfigPath,CStdioFile::modeWrite | CStdioFile::modeCreate);
	if(!bIsOpen)
	{
		AfxMessageBox("错误！无法打开配置文件");
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
	// TODO: 在此添加控件通知处理程序代码
}

void C银江数据接收Dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Stock_Quit(this->m_hWnd);
	CDialog::OnClose();
}


void C银江数据接收Dlg::OnBnClickedButton2()
{
	m_stockManageDlg.DoModal();
	// TODO: 在此添加控件通知处理程序代码
}
