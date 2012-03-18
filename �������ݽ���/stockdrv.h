//*��������ӿڹ淶V1.02
//****************************************************************************************/
//*	��������װ��,��ϵͳע�����ע����������Ϣ,�������ͨ����ע����Ϣ�������涯̬��*
//*	HKEY_LOCAL_MACHINE/SOFTWARE/StockDrv ��						* 
//*	Driver  = "����DLLȫ·��"							*
//****************************************************************************************/

#ifndef __STOCKDRV_H__
#define __STOCKDRV_H__


// ������ʽ���Ͷ���
#define RCV_WORK_SENDMSG	4

/////////////////////////////////////////////////////////////////////////////////////////

// ֤ȯ�г�
#define		SH_MARKET_EX			'HS'		// �Ϻ�
#define		SZ_MARKET_EX			'ZS'		// ����

// �ļ���������
// �ṹ������ʽ���ļ�����
#define		FILE_HISTORY_EX			2		// ����������
#define		FILE_MINUTE_EX			4		// ������������
#define		FILE_POWER_EX			6		// �����Ȩ����
#define		FILE_5MINUTE_EX			81		// ��5����������

#define		FILE_BASE_EX			0x1000		// Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
#define		FILE_NEWS_EX			0x1002		// ������,��������m_szFileName����Ŀ¼������

// ��Ϣ������
#define		News_Sha_Ex			2		// ��֤��Ϣ
#define		News_Szn_Ex			4		// ��֤��Ϣ
#define		News_Fin_Ex			6		// �ƾ�����
#define		News_Unknown_Ex			-1		// δ֪�ṩ��

//Definition For nInfo of Function GetStockDrvInfo(int nInfo,void * pBuf);
#define		RI_IDSTRING			1			// ��������,����(LPCSTR)������
#define		RI_IDCODE			2			// ����
#define		RI_VERSION			3			// ��������汾

#define		RI_NETPCID			1000			//  - ���ص���Ӳ�����

#define		STKLABEL_LEN			10			// �ɺ����ݳ���,�����г��ɺű������Ǯ��
#define		STKNAME_LEN			32			// ��������
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��������(�汾���ݽṹRCV_WORK_SENDMSG)
typedef struct tagRCV_REPORT_STRUCTExV2	
{
	WORD	m_cbSize;							// �ṹ��С
	time_t	m_time;								// �ɽ�ʱ��
	WORD	m_wMarket;							// ��Ʊ�г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
	char	m_szName[STKNAME_LEN];						// ��Ʊ����,��'\0'��β
	
	float	m_fLastClose;							// ����
	float	m_fOpen;							// ��
	float	m_fHigh;							// ���
	float	m_fLow;								// ���
	float	m_fNewPrice;							// ����
	float	m_fVolume;							// �ɽ���
	float	m_fAmount;							// �ɽ���
	float	m_fBuyPrice[3];							// �����1,2,3
	float	m_fBuyVolume[3];						// ������1,2,3
	float	m_fSellPrice[3];						// ������1,2,3
	float	m_fSellVolume[3];						// ������1,2,3

	float	m_fBuyPrice4;							// �����4
	float	m_fBuyVolume4;							// ������4
	float	m_fSellPrice4;							// ������4
	float	m_fSellVolume4;							// ������4
} RCV_REPORT_STRUCTExV2;

//////////////////////////////////////////////////////////////////////////////////
//��������(�汾3���ݽṹRCV_WORK_SENDMSG������RCV_REPORT_STRUCTExV2�ṹ��ʹ��m_cbSize���ֲ�ͬ�ṹ)
typedef struct tagRCV_REPORT_STRUCTExV3	
{
	WORD	m_cbSize;							// �ṹ��С
	
	unsigned int	m_time;								// �ɽ�ʱ��
	WORD	m_wMarket;							// ��Ʊ�г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
	char	m_szName[STKNAME_LEN];						// ��Ʊ����,��'\0'��β
	
	float	m_fLastClose;							// ����
	float	m_fOpen;							// ��
	float	m_fHigh;							// ���
	float	m_fLow;								// ���
	float	m_fNewPrice;							// ����
	float	m_fVolume;							// �ɽ���
	float	m_fAmount;							// �ɽ���
	float	m_fBuyPrice[3];							// �����1,2,3
	float	m_fBuyVolume[3];						// ������1,2,3
	float	m_fSellPrice[3];						// ������1,2,3
	float	m_fSellVolume[3];						// ������1,2,3

	float	m_fBuyPrice4;							// �����4
	float	m_fBuyVolume4;							// ������4
	float	m_fSellPrice4;							// ������4
	float	m_fSellVolume4;							// ������4

	float	m_fBuyPrice5;							// �����5
	float	m_fBuyVolume5;							// ������5
	float	m_fSellPrice5;							// ������5
	float	m_fSellVolume5;							// ������5
} RCV_REPORT_STRUCTExV3;

//////////////////////////////////////////////////////////////////////////////////
//��������ͷ
//����ͷ m_dwHeadTag == EKE_HEAD_TAG 
#define EKE_HEAD_TAG	0xffffffff

typedef struct	tagRCV_EKE_HEADEx
{
	DWORD	m_dwHeadTag;							// = EKE_HEAD_TAG
	WORD	m_wMarket;							// �г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����
}RCV_EKE_HEADEx;

//������������
typedef union tagRCV_HISTORY_STRUCTEx
{
	struct
	{
		time_t	m_time;				//UCT
		float	m_fOpen;			//����
		float	m_fHigh;			//���
		float	m_fLow;				//���
		float	m_fClose;			//����
		float	m_fVolume;			//��
		float	m_fAmount;			//��
		WORD	m_wAdvance;			//����,��������Ч
		WORD	m_wDecline;			//����,��������Ч
	};
	RCV_EKE_HEADEx	m_head;
}RCV_HISTORY_STRUCTEx;

//�����ʱ������
typedef union tagRCV_MINUTE_STRUCTEx
{
	struct{
		time_t	m_time;				// UCT
		float	m_fPrice;
		float	m_fVolume;
		float	m_fAmount;
	};
	RCV_EKE_HEADEx	m_head; 
}RCV_MINUTE_STRUCTEx;

//�����Ȩ����
typedef union tagRCV_POWER_STRUCTEx
{
	struct
	{
		time_t	m_time;				// UCT
		float	m_fGive;			// ÿ����
		float	m_fPei;				// ÿ����
		float	m_fPeiPrice;			// ��ɼ�,���� m_fPei!=0.0f ʱ��Ч
		float	m_fProfit;			// ÿ�ɺ���
	};
	RCV_EKE_HEADEx	m_head;
}RCV_POWER_STRUCTEx;


//�ֱ�����///////////////////////////////////
typedef struct tagYJFenbi 
{				
	long	m_lTime;			// hhmmss ����93056 ����9:30:56���̿�����

	float	m_fHigh;			// ���
	float	m_fLow;				// ���
	float	m_fNewPrice;			// ����
	float	m_fVolume;			// �ɽ���
	float	m_fAmount;			// �ɽ���

	long	m_lStroke;			// ����
	float	m_fBuyPrice[5];			// �����1,2,3,4,5
	float	m_fBuyVolume[5];		// ������1,2,3,4,5
	float	m_fSellPrice[5];		// ������1,2,3,4,5
	float	m_fSellVolume[5];		// ������1,2,3,4,5
} RCV_FENBI_STRUCTEx;

typedef struct YJ_Fenbi
{
	WORD	m_wMarket;			// ��Ʊ�г����͡������������������� ͬͨ�ӹ淶����
	char	m_szLabel[STKLABEL_LEN];	// ��Ʊ����,��'\0'��β
	long	m_lDate;			// �ֱ����ݵ����� FORMAT: 20050808,  0��ʾ�������������յ���ϸ����
	float	m_fLastClose;			// ����
	float	m_fOpen;			// ��
	WORD	m_nCount;			//m_Data���������ֱʱ���(2�ֽ�smallint)
	RCV_FENBI_STRUCTEx	m_Data[1];	//����Ϊm_nCount*sizeof(RCV_FENBI_STRUCTEx)�ֽ�	
} RCV_FENBI;
/////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// �ļ��������ݰ�ͷ
//  עһ:
//	m_wDataType == FILE_BASE_EX
//				m_dwAttrib = ��Ʊ֤ȯ�г�,m_szFileName�������ļ���
//	m_wDataType == FILE_NEWS_EX
//				m_dwAttrib = ��Ϣ��Դ,m_szFileName ����Ŀ¼���ļ���,Ŀ¼��Ϊ��Ϣ��Դ
//				��:  "�Ͻ�����Ϣ\\SH0001.TXT","��Ա����\\000001.TXT"
typedef struct tagRCV_FILE_HEADEx
{
	DWORD	m_dwAttrib;					// �ļ�������
	DWORD	m_dwLen;					// �ļ�����
	DWORD	m_dwSerialNo;					// ���к�
	char	m_szFileName[MAX_PATH];				// �ļ��� or URL
} RCV_FILE_HEADEx;

//////////////////////////////////////////////////////////////////////////////////
// ����֪ͨ��Ϣ
//ֱ����������֪ͨ��Ϣ
//	wParam = RCV_WPARAM;
//	lParam ָ�� RCV_DATA�ṹ;
//	���� 1 �Ѿ�����, 0 δ������ܴ���

#define RCV_REPORT		0x3f001234
#define RCV_FILEDATA		0x3f001235
#define RCV_FENBIDATA		0x3f001301


// עһ:
//	  ��¼����ʾ�������ݺͲ�������(���� Header)�����ݰ���,���ļ���������, = 1
// ע��:
//	  �� m_bDISK = FALSE, m_pData Ϊ���ݻ�����ָ��
//		 ******** ���ݹ���,�����޸����� **********
//		 m_bDISK = TRUE,  m_pData Ϊ���ļ��Ĵ����ļ���,һ�����ֻ��
//		 ��������ȴ��ļ��ô��̷�ʽ
typedef struct tagRCV_DATA
{
	int					m_wDataType;		// �ļ�����
	int					m_nPacketNum;		// ��¼��,�μ�עһ
	RCV_FILE_HEADEx		m_File;					// �ļ��ӿ�
	BOOL				m_bDISK;			// �ļ��Ƿ��Ѵ��̵��ļ�
	union
	{
		RCV_REPORT_STRUCTExV2*	m_pReportV2;
		RCV_REPORT_STRUCTExV3*	m_pReportV3;
		RCV_HISTORY_STRUCTEx *	m_pDay;
		RCV_MINUTE_STRUCTEx  *	m_pMinute;
		RCV_POWER_STRUCTEx   *	m_pPower;
		void		     *	m_pData;		// �μ�ע��
	};
} RCV_DATA;

/* ��Ϣ������� DEMO
LONG OnStkDataOK(UINT wParam,LONG lParam)
{
	RCV_DATA * pHeader;
	pHeader = (RCV_DATA *)lParam;
	switch( wParam )
	{
	case RCV_REPORT:					//��Ʊ����
		for(i=0; i<pHeader->m_nPacketNum; i++)
		{
			pHeader->m_pReportV3[i]...// ���ݴ���


			//CTime tm(pHeader->m_pReportV3[i].m_time);
			//CString jc=pHeader->m_pReportV3[i].m_szName;
			//CString jg;
			//jg.Format("%.2f",pHeader->m_pReportV3[i].m_fNewPrice);
			//AfxMessageBox(tm.Format ("%Y%m%d %H:%M:%S")+ " " + jc+ " " + jg);//��ʾʱ�� ��� ���¼�
		}
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
	case RCV_FENBIDATA
		{
			RCV_FENBI*	pFb;
			pFb = (RCV_FENBI *)lParam;
			for ( i=0; i<pFb->m_nCount; i++ )
			{
				//���ݴ���
			}			
		}
		break;
	default:
		return 0;					// unknown data
	}
	return 1;
}
*/


//////////////////////////////////////////////////////////////////////////////////
//APIs
#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////////////
// ע�ắ��

// ��Ʊ��ʼ��
// ��ڲ���:
//		hWnd			������Ϣ�Ĵ��ھ��
//		Msg				�û��Զ�����Ϣ
//		nWorkMode		�ӿڹ�����ʽ,�μ�������ʽ���Ͷ���
// ���ز���:
//		 1				�ɹ�	
//		-1				ʧ��
// ע:
//		ע���,��������������ڷ�����Ϣ 
//		����Ʊ����û����,��������Ʊ���ճ���
typedef int (WINAPI	* pStock_Init)(HWND hWnd,UINT Msg,int nWorkMode);

// �˳�,ֹͣ������Ϣ
// ��ڲ���:
//		hWnd			������Ϣ�Ĵ��ھ��,ͬ Stock_Init �ĵ�����ڲ���
//	���ز���:
//		 1				�ɹ�	
//		-1				ʧ��
typedef int (WINAPI *	pStock_Quit)(HWND hWnd);

// ������ճ���,��������
// ��ڲ���:
//			bSetup		TRUE		��ʾ����,��������
//						FALSE		��������
// ���ز���:
//			 1			�ɹ�
//			-1			ʧ��
typedef int	(WINAPI*  pSetupReceiver)(BOOL bSetup);

//	ȡ�ù�Ʊ������Ϣ
//	��ڲ���:
//			nInfo		����
//			pBuf		������
//	���ڲ���:
//			nInfo == RI_IDSTRING,	���������ַ�������, pBuf Ϊ�����ַ���
//									��:	"��������"
//			nInfo == RI_IDCODE,		������Ϣ�� ID ��, pBuf Ϊ�ַ�����ʽ�� ID ��
//									��:	0x78001234	  "78001234"
//			nInfo == RI_VERSION,	������Ϣ���汾��, pBuf Ϊ�ַ����汾
//									��:  1.00		  "1.00"		
//			nInfo == RI_V2SUPPORT,	�����Ƿ�֧������SJS��ṹ, pBuf��Ч

//			nInfo == RI_NETPCID	�����ַ������ȣ� pBuf����Ӳ����ţ�������֤�Ϸ��û���


//			�����ӿ��ϵ��Զ������ʱ����ѡ���ʱ���ӿڻ���ݷ�������ı����ϵĹ�Ʊ�����Զ�����ùɷ�ʱ����
typedef DWORD (WINAPI*  pGetStockDrvInfo)(int nInfo,void * pBuf);
#ifdef __cplusplus
}
#endif


#endif // __STOCKDRV_H__
